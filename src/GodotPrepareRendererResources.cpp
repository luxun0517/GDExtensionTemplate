//
// Created by Harris.Lu on 2024/1/31.
//

#include "GodotPrepareRendererResources.h"
#include "constants.h"
#include <Cesium3DTilesSelection/Tile.h>
#include <Cesium3DTilesSelection/Tileset.h>
#include <CesiumGeometry/Transforms.h>
#include <CesiumGeospatial/Ellipsoid.h>
#include <CesiumGltf/AccessorView.h>
#include <CesiumGltf/ExtensionExtMeshFeatures.h>
#include <CesiumGltf/ExtensionKhrMaterialsUnlit.h>
#include <CesiumGltf/ExtensionModelExtFeatureMetadata.h>
#include <CesiumGltf/ExtensionModelExtStructuralMetadata.h>
#include <CesiumGltfContent/GltfUtilities.h>
#include <CesiumGltfReader/GltfReader.h>
#include <CesiumUtility/ScopeGuard.h>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/rid.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/templates/vector.hpp>
#include "LoadGltfResult.h"

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
        int32_t indexCount = 0;
        switch ( primitive.mode )
        {
            case MeshPrimitive::Mode::TRIANGLES:
            case MeshPrimitive::Mode::POINTS:
                indexCount = static_cast<int32_t>( indicesView.size() );
                break;
            case MeshPrimitive::Mode::TRIANGLE_STRIP:
            case MeshPrimitive::Mode::TRIANGLE_FAN:
				indexCount = static_cast<int32_t>( 3 * indicesView.size() - 2 );
				break;
            default:
                WARN_PRINT( "Unsupported primitive mode" );
                break;
        }

        if (indexCount < 3 && primitive.mode != MeshPrimitive::Mode::POINTS)
		{
			WARN_PRINT( "Invalid index count" );
			return;
		}

        //Vector<int> indices;
        PackedInt32Array indices;
        indices.resize( indexCount );

        meshData[RenderingServer::ARRAY_INDEX] = indices;

        if ( primitive.mode == MeshPrimitive::Mode::TRIANGLES ||
             primitive.mode == MeshPrimitive::Mode::POINTS )
        {
            for ( int64_t i = 0; i < indicesView.size() - 2; ++i )
            {
                indices[i] = indicesView[i];
            }
        }
        else if ( primitive.mode == MeshPrimitive::Mode::TRIANGLE_STRIP )
        {
            for ( int64_t i = 0; i < indicesView.size() - 2; ++i )
            {
                if ( i % 2 )
                {
                    indices[3 * i] = indicesView[i];
                    indices[3 * i + 1] = indicesView[i + 2];
                    indices[3 * i + 2] = indicesView[i + 1];
                }
                else
                {
                    indices[3 * i] = indicesView[i];
                    indices[3 * i + 1] = indicesView[i + 1];
                    indices[3 * i + 2] = indicesView[i + 2];
                }
            }
        }

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

    GodotPrepareRendererResources::GodotPrepareRendererResources(
        const Ref<GD3DTileset> &tileset ) :
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

        //loadModel( *pModel, transform );

         int32_t numberOfPrimitives = countPrimitives( *pModel );

        

         return asyncSystem.runInMainThread( [numberOfPrimitives]() {
             Vector<RID> _meshes;
             _meshes.resize( numberOfPrimitives );
             _meshes.fill( RS->mesh_create() );
             return _meshes;
         } );

        /*return asyncSystem.createResolvedFuture(
        Cesium3DTilesSelection::TileLoadResultAndRenderResources{ std::move( tileLoadResult ),
                                                                    pHalf.Release() } );*/
    }

} // CesiumForGodot