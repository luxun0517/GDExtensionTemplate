//
// Created by Harris.Lu on 2024/1/31.
//
#pragma warning (disable : 4189)

#include "GodotPrepareRendererResources.h"
#include "constants.h"
#include <Cesium3DTilesSelection/Tile.h>
#include <Cesium3DTilesSelection/Tileset.h>
#include <CesiumGeometry/Transforms.h>
#include <CesiumGeospatial/Ellipsoid.h>
#include <CesiumGltf/AccessorView.h>
#include <CesiumGltf/ExtensionExtMeshFeatures.h>
#include <CesiumGltf/ExtensionKhrMaterialsUnlit.h>
#include <CesiumGltf/ExtensionModelExtStructuralMetadata.h>
#include <CesiumGltfContent/GltfUtilities.h>
#include <CesiumGltfReader/GltfReader.h>
#include <CesiumUtility/ScopeGuard.h>
#include <godot_cpp/templates/vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>
#include "LoadGltfResult.h"
#include "godot_cpp/classes/world3d.hpp"

using namespace CesiumRasterOverlays;
using namespace CesiumGltfContent;
using namespace Cesium3DTilesSelection;
using namespace CesiumGeometry;
using namespace CesiumGeospatial;
using namespace CesiumGltf;
using namespace CesiumUtility;
using namespace CesiumAsync;

using namespace godot;
namespace CesiumForGodot {

    // bool validateVertexColors(
    //     const Model &model,
    //     uint32_t accessorId,
    //     size_t vertexCount
    // )
    // {
    //     if ( accessorId >= model.accessors.size() )
    //     {
    //         return false;
    //     }
    //
    //     const Accessor &colorAccessor = model.accessors[accessorId];
    //     if ( colorAccessor.type != Accessor::Type::VEC3 &&
    //          colorAccessor.type != Accessor::Type::VEC4 )
    //     {
    //         return false;
    //     }
    //
    //     if ( colorAccessor.componentType != Accessor::ComponentType::UNSIGNED_BYTE &&
    //          colorAccessor.componentType != Accessor::ComponentType::UNSIGNED_SHORT &&
    //          colorAccessor.componentType != Accessor::ComponentType::FLOAT )
    //     {
    //         return false;
    //     }
    //
    //     if ( colorAccessor.count < vertexCount )
    //     {
    //         return false;
    //     }
    //
    //     return true;
    // }

    void GodotPrepareRendererResources::populateMeshDataArray(
        TileLoadResult &tileLoadResult
    )
    {
        CesiumGltf::Model *pModel = std::get_if<CesiumGltf::Model>( &tileLoadResult.contentKind );
        if ( !pModel )
        {
            return;
        }

        pModel->forEachPrimitiveInScene(
            -1,
            [this, pModel](
                const CesiumGltf::Model &gltf,
                const CesiumGltf::Node &node,
                const CesiumGltf::Mesh &mesh,
                const CesiumGltf::MeshPrimitive &primitive,
                const glm::dmat4 &transform ) {
                uint64_t flags = RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES;
                
                Array arrays;
                arrays.resize( godot::Mesh::ARRAY_MAX );
                
                godot::Mesh::PrimitiveType primitive_type = godot::Mesh::PRIMITIVE_TRIANGLES;

                switch ( primitive.mode )
                {
                    case MeshPrimitive::Mode::TRIANGLES:
                        primitive_type = godot::Mesh::PRIMITIVE_TRIANGLES;
						break;
                    case MeshPrimitive::Mode::TRIANGLE_STRIP:
                    case MeshPrimitive::Mode::TRIANGLE_FAN:
						primitive_type = godot::Mesh::PRIMITIVE_TRIANGLE_STRIP;
                        break;
                    case MeshPrimitive::Mode::POINTS:
                        primitive_type = godot::Mesh::PRIMITIVE_POINTS;
                        break;
                    case MeshPrimitive::Mode::LINES:
                    case MeshPrimitive::Mode::LINE_LOOP:
						primitive_type = godot::Mesh::PRIMITIVE_LINES;
						break;
                    case MeshPrimitive::Mode::LINE_STRIP:
                        primitive_type = godot::Mesh::PRIMITIVE_LINE_STRIP;
                        break;
                    default:
                        break;
                }

                int32_t vertex_num = 0;
                auto positionAccessorIt = primitive.attributes.find( "POSITION" );
                if ( positionAccessorIt == primitive.attributes.end() )
                {
                    // This primitive doesn't have a POSITION semantic, ignore it.
                    return;
                }

                int32_t positionAccessorID = positionAccessorIt->second;
                AccessorView<glm::vec3> positionView( gltf, positionAccessorID );
                PackedVector3Array vertices;
                vertices.resize( positionView.size() );
                Vector3 *vertices_ptr = vertices.ptrw();
                for (int i = 0; i < positionView.size(); ++i)
                {
                    vertices_ptr[i] = Vector3( positionView[i].x, positionView[i].y, positionView[i].z );
                }
                
                arrays[godot::Mesh::ARRAY_VERTEX] = vertices;

                //是否包含索引
                if(primitive.indices < 0 || primitive.indices >= gltf.accessors.size())
                {
                    WARN_PRINT( "no indices" );
                } else
                {
                    const Accessor& indexAccessorGltf = gltf.accessors[primitive.indices];
                    if(indexAccessorGltf.componentType == Accessor::ComponentType::UNSIGNED_BYTE)
                    {
                        // AccessorView<uint8_t> indexAccessor(gltf, primitive.indices);
                        WARN_PRINT( "UNSIGNED_BYTE" );
                    } else if(indexAccessorGltf.componentType == Accessor::ComponentType::UNSIGNED_SHORT)
                    {
                        AccessorView<uint16_t> indexAccessor(gltf, primitive.indices);

                        if(primitive_type == godot::Mesh::PRIMITIVE_TRIANGLES)
                        {
                            PackedInt32Array indices;
                            indices.resize( indexAccessor.size() );
                            for (int i = 0; i < indexAccessor.size(); ++i) {
                                indices[i] = indexAccessor[i];
                            }

                            arrays[godot::Mesh::ARRAY_INDEX] = indices;
                        }
                     
                    } else if(indexAccessorGltf.componentType == Accessor::ComponentType::UNSIGNED_INT)
                    {
                        WARN_PRINT( "UNSIGNED_INT" );
                    } else
                    {
                        WARN_PRINT( ">>>>>>> " );
                    }
                }

                RID _mesh = RS->mesh_create();
                RS->mesh_add_surface_from_arrays(_mesh, RenderingServer::PRIMITIVE_TRIANGLES, arrays);
                //创建材质
                  RID _material = RS->material_create();
                  RID _shader = RS->shader_create();

                  RS->shader_set_code( _shader, R"(
                    // Default 3D material shader.
                
                    shader_type spatial;
                
                    void vertex() {
        	            ROUGHNESS = 0.3;
                    }
                
                    void fragment() {
        	            ALBEDO = vec3(1., 1., 0.);
        	            ROUGHNESS = 0.2;
        	            METALLIC = 1.;
                    }
                )" );
                RS->material_set_shader( _material, _shader );
                RS->mesh_surface_set_material( _mesh, 0, _material );
                RID scenario = this->_tileset->get_world_3d()->get_scenario();
                //实例化到场景中
                RS->instance_create2(_mesh, scenario);
            });
    }

    template <typename TIndex, class TIndexAccessor>
    void loadPrimitive(
        Array meshData,
        CesiumPrimitiveInfo& primitiveInfo,
        const CesiumGltf::Model &gltf,
        const CesiumGltf::Node &node,
        const CesiumGltf::Mesh &mesh,
        const CesiumGltf::MeshPrimitive &primitive,
        const glm::dmat4 &transform,
        const TIndexAccessor &indicesView,
        const AccessorView<Vector3> &positionView
    )
    {


  //      int32_t indexCount = 0;
  //      switch ( primitive.mode )
  //      {
  //          case MeshPrimitive::Mode::TRIANGLES:
  //          case MeshPrimitive::Mode::POINTS:
  //              indexCount = static_cast<int32_t>( indicesView.size() );
  //              break;
  //          case MeshPrimitive::Mode::TRIANGLE_STRIP:
  //          case MeshPrimitive::Mode::TRIANGLE_FAN:
		//		indexCount = static_cast<int32_t>( 3 * indicesView.size() - 2 );
		//		break;
  //          default:
  //              WARN_PRINT( "Unsupported primitive mode" );
  //              break;
  //      }

  //      if (indexCount < 3 && primitive.mode != MeshPrimitive::Mode::POINTS)
		//{
		//	WARN_PRINT( "Invalid index count" );
		//	return;
		//}

  //      //Vector<int> indices;
  //      PackedInt32Array indices;
  //      indices.resize( indexCount );

  //      meshData[RenderingServer::ARRAY_INDEX] = indices;

  //      if ( primitive.mode == MeshPrimitive::Mode::TRIANGLES ||
  //           primitive.mode == MeshPrimitive::Mode::POINTS )
  //      {
  //          for ( int64_t i = 0; i < indicesView.size() - 2; ++i )
  //          {
  //              indices[i] = indicesView[i];
  //          }
  //      }
  //      else if ( primitive.mode == MeshPrimitive::Mode::TRIANGLE_STRIP )
  //      {
  //          for ( int64_t i = 0; i < indicesView.size() - 2; ++i )
  //          {
  //              if ( i % 2 )
  //              {
  //                  indices[3 * i] = indicesView[i];
  //                  indices[3 * i + 1] = indicesView[i + 2];
  //                  indices[3 * i + 2] = indicesView[i + 1];
  //              }
  //              else
  //              {
  //                  indices[3 * i] = indicesView[i];
  //                  indices[3 * i + 1] = indicesView[i + 1];
  //                  indices[3 * i + 2] = indicesView[i + 2];
  //              }
  //          }
  //      }

  //      Array VertexArrtibutes;
  //      VertexArrtibutes.resize( RenderingServer::ARRAY_MAX );

  //      VertexArrtibutes[RenderingServer::ARRAY_VERTEX] = positionView.toVector3Array();


  //      const CesiumGltf::Material* pMaterial = Model::getSafe(&gltf.materials, primitive.material);

  //      //�Ƿ����޹��ղ���
  //      primitiveInfo.isUnlit = pMaterial && pMaterial->hasExtension<ExtensionKhrMaterialsUnlit>();

  //      // NORMAL attribute
  //      bool hasNormals = false;
  //      bool computeFlatNormals = false;
  //      auto normalAccessorIt = primitive.attributes.find( "NORMAL" );
  //      AccessorView<Vector3> normalView;
  //      if (normalAccessorIt != primitive.attributes.end())
  //      {
		//	normalView = AccessorView<Vector3>( gltf, normalAccessorIt->second );
  //          hasNormals = normalView.status() == AccessorViewStatus::Valid;
		//}
  //      else if ( !primitiveInfo.isUnlit && primitive.mode != MeshPrimitive::Mode::POINTS )
  //      {
  //          computeFlatNormals = hasNormals = true;
		//}

  //      if ( hasNormals )
  //      {
  //          //VertexArrtibutes[RenderingServer::ARRAY_NORMAL] = normalView.toVector3Array();
  //      }

  //      // COLOR_0 attribute
  //      auto colorAccessorIt = primitive.attributes.find( "COLOR_0" );
  //      bool hasVertexColors =
  //          colorAccessorIt != primitive.attributes.end() &&
  //          validateVertexColors( gltf, colorAccessorIt->second, positionView.size() );
  //      if ( hasNormals )
  //      {
  //          //VertexArrtibutes[RenderingServer::ARRAY_COLOR] = normalView.toVector3Array();
  //          const int8_t numComponents =
  //              gltf.accessors[colorAccessorIt->second].computeNumberOfComponents();
  //          if ( numComponents == 4 )
  //          {
  //              primitiveInfo.isTranslucent = true;
  //          }
  //      }
  //      constexpr int MAX_TEX_COORDS = 8;
  //      int numTexCoords = 0;
  //      AccessorView<Vector2> texCoordViews[MAX_TEX_COORDS];

  //      // TEXCOORD attribute
  //    /*  for ( int i = 0; i < 8 && numTexCoords < MAX_TEX_COORDS; ++i )
  //      {
  //      }*/

  //      int32_t vertexCount = computeFlatNormals ? indexCount : static_cast<int32_t>(positionView.size());
    }

    int32_t countPrimitives( const CesiumGltf::Model& model )
	{
	    int32_t numberOfPrimitives = 0;
        model.forEachPrimitiveInScene(
            -1,
			[&numberOfPrimitives](
                const CesiumGltf::Model& gltf,
                const CesiumGltf::Node& node,
                const CesiumGltf::Mesh& mesh,
                const CesiumGltf::MeshPrimitive& primitive,
                const glm::dmat4& transform
            ) {
				++numberOfPrimitives;
			}
		);

        return numberOfPrimitives;
	}

    // void loadModel(
    //     CesiumGltf::Model& pModel,
    //     const glm::dmat4& transform)
    // {
    //    pModel.forEachPrimitiveInScene( -1, [pModel](const CesiumGltf::Model& gltf,
    //            const CesiumGltf::Node& node,
    //            const CesiumGltf::Mesh& mesh,
    //            const CesiumGltf::MeshPrimitive& primitive,
    //            const glm::dmat4& transform){
    //
    //             auto positionAccessorIt = primitive.attributes.find("POSITION");
    //             if (positionAccessorIt == primitive.attributes.end()) {
    //                // This primitive doesn't have a POSITION semantic, ignore it.
    //                return;
    //             }
    //             int32_t positionAccessorID = positionAccessorIt->second;
    //             const Accessor* pPositionAccessor = Model::getSafe(&pModel.accessors, positionAccessorID);
    //             if (!pPositionAccessor) {
    //                // Position accessor does not exist, so ignore this primitive.
    //                return;
    //             }
    //             
    //             AccessorView<Vector3> positionView(gltf, positionAccessorID);
    //             WARN_PRINT( vformat( "position size: %d", positionView.size() ) );
    //             for (uint32_t i = 0; i < positionView.size(); ++i) {
    //                 WARN_PRINT( vformat( "position: %d", positionView[i] ) );
    //             }
    //        
    //    });
    // }

    GodotPrepareRendererResources::GodotPrepareRendererResources(
        GD3DTileset* tileset ) :
        _tileset( tileset ),
        _shaderProperty()
    {
    }

    CesiumAsync::Future<Cesium3DTilesSelection::TileLoadResultAndRenderResources>
        GodotPrepareRendererResources::prepareInLoadThread(
            const CesiumAsync::AsyncSystem &asyncSystem,
            Cesium3DTilesSelection::TileLoadResult &&tileLoadResult,
            const glm::dmat4 &transform,
            const std::any &renderOptions )
    {
        CesiumGltf::Model* pModel = std::get_if<CesiumGltf::Model>(&tileLoadResult.contentKind);
        if ( !pModel )
        {
            return asyncSystem.createResolvedFuture(
                TileLoadResultAndRenderResources{ std::move(tileLoadResult), nullptr }
            );
        }

        // loadModel( *pModel, transform );

        int32_t numberOfPrimitives = countPrimitives( *pModel );

        return asyncSystem.runInMainThread( [numberOfPrimitives]() {
             Vector<RID> _meshes;
             _meshes.resize( numberOfPrimitives );
             _meshes.fill( RS->mesh_create() );
             return _meshes;
         })
        .thenInWorkerThread(
            [this, asyncSystem, tileLoadResult = std::move( tileLoadResult )]( Vector<RID> _meshes ) mutable
            {
                this->populateMeshDataArray( tileLoadResult );
                return asyncSystem.createResolvedFuture(
                    TileLoadResultAndRenderResources{ std::move(tileLoadResult), nullptr });
            });
         // .thenInWorkerThread(
         //     [tileLoadResult = std::move( tileLoadResult )]() mutable
         //     {
         //        // populateMeshDataArray( tileLoadResult );
         //     }
         // );
    }

    void* GodotPrepareRendererResources::prepareInMainThread(
      Cesium3DTilesSelection::Tile& tile,
      void* pLoadThreadResult)
    {
        WARN_PRINT( "prepareInMainThread" );
        return nullptr;
    }

    void GodotPrepareRendererResources::free( Cesium3DTilesSelection::Tile &tile,
        void *pLoadThreadResult, void *pMainThreadResult ) noexcept
    {
        WARN_PRINT( "free" );
    }

    void * GodotPrepareRendererResources::prepareRasterInLoadThread( CesiumGltf::ImageCesium &image,
        const std::any &rasterOverlayOptions )
    {
        WARN_PRINT( "prepareRasterInLoadThread" );
        return nullptr;
    }

    void * GodotPrepareRendererResources::prepareRasterInMainThread(
        CesiumRasterOverlays::RasterOverlayTile &rasterTile, void *pLoadThreadResult )
    {
        WARN_PRINT( "prepareRasterInMainThread" );
        return nullptr;
    }

    void GodotPrepareRendererResources::freeRaster(
        const CesiumRasterOverlays::RasterOverlayTile &rasterTile, void *pLoadThreadResult,
        void *pMainThreadResult ) noexcept
    {
        WARN_PRINT( "freeRaster" );
    }

    void GodotPrepareRendererResources::attachRasterInMainThread(
        const Cesium3DTilesSelection::Tile &tile, int32_t overlayTextureCoordinateID,
        const CesiumRasterOverlays::RasterOverlayTile &rasterTile,
        void *pMainThreadRendererResources, const glm::dvec2 &translation, const glm::dvec2 &scale )
    {
        WARN_PRINT( "attachRasterInMainThread" );
    }

    void GodotPrepareRendererResources::detachRasterInMainThread(
        const Cesium3DTilesSelection::Tile &tile, int32_t overlayTextureCoordinateID,
        const CesiumRasterOverlays::RasterOverlayTile &rasterTile,
        void *pMainThreadRendererResources ) noexcept
    {
        WARN_PRINT( "detachRasterInMainThread" );
    }

} // CesiumForGodot