//
// Created by Harris.Lu on 2024/1/31.
//

#include "CameraManager.h"
#include "CesiumGeoreference.h"
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_script.hpp>
#include <godot_cpp/classes/v_box_container.hpp> 
#include <godot_cpp/classes/viewport.hpp>
#include <CesiumGeospatial/Ellipsoid.h>
#include <CesiumGeospatial/GlobeTransforms.h>
#include <CesiumUtility/Math.h>
#include <glm/trigonometric.hpp>
#include "GodotTransforms.h"
#include "CesiumGeoreference.h"

using namespace Cesium3DTilesSelection;
using namespace CesiumGeospatial;
using namespace CesiumUtility;

namespace CesiumForGodot {
	
	ViewState godotCameraToViewState( 
		const LocalHorizontalCoordinateSystem *pCoordinateSystem,
        const glm::dmat4 &godotWorldToTileset,
		Camera3D& camera
	)
    {
        Transform3D transform = camera.get_transform();
        Vector3 cameraPositionGodot = transform.get_origin();

        glm::dvec3 cameraPosition = glm::dvec3(
            godotWorldToTileset * glm::dvec4( cameraPositionGodot.x, cameraPositionGodot.y,
                                              cameraPositionGodot.z, 1.0 ) );

        Vector3 cameraDirectionGodot = transform.basis.xform( Vector3( 0, 0, -1 ) );
        glm::dvec3 cameraDirection = glm::dvec3(
            godotWorldToTileset * glm::dvec4( cameraDirectionGodot.x, cameraDirectionGodot.y,
                                              cameraDirectionGodot.z, 0.0 ) );

        Vector3 cameraUpGodot = transform.basis.xform( Vector3( 0, 1, 0 ) );

        glm::dvec3 cameraUp =
            glm::dvec3( godotWorldToTileset *
                        glm::dvec4( cameraUpGodot.x, cameraUpGodot.y, cameraUpGodot.z, 0.0 ) );

        if ( pCoordinateSystem )
        {
            cameraPosition = pCoordinateSystem->localPositionToEcef( cameraPosition );
            cameraDirection = pCoordinateSystem->localDirectionToEcef( cameraDirection );
            cameraUp = pCoordinateSystem->localDirectionToEcef( cameraUp );
        }
        double actualAspectRatio;
        actualAspectRatio = camera.get_camera_projection().get_aspect();

        Vector2 size = camera.get_camera_projection().get_viewport_half_extents();

        double verticalFOV = CesiumUtility::Math::degreesToRadians( camera.get_fov() );
        double horizontalFOV = 2 * glm::atan( actualAspectRatio * glm::tan( verticalFOV * 0.5 ) );

        return ViewState::create(
            cameraPosition, glm::normalize( cameraDirection ), glm::normalize( cameraUp ), glm::dvec2( size.x * 2, size.y * 2 ),
                                  horizontalFOV, verticalFOV );
	}

    void _find_cameras( TypedArray<Node> from_nodes, Node *excluded_node,
                                       TypedArray<Camera3D> &cam_array )
    {
        for ( int i = 0; i < from_nodes.size(); i++ )
        {
            Node *node = Object::cast_to<Node>( from_nodes[i] );
            if ( node != excluded_node )
            {
                _find_cameras( node->get_children(), excluded_node, cam_array );
            }
            if ( node->is_class( "Camera3D" ) )
            {
                WARN_PRINT( vformat( "Found a Camera3D at: %s", node->get_path() ) );
                cam_array.push_back( node );
            }
        }
    }

	std::vector<ViewState> CameraManager::getAllCameras( 
		const Ref<GD3DTileset> &context
	)
    {
        const LocalHorizontalCoordinateSystem *pCoordinateSystem = nullptr;

		glm::dmat4 godotWorldToTileset = GodotTransforms::fromGodot( context->get_transform() );

		Ref<CesiumGeoreference> georeference =
            static_cast<CesiumGeoreference*>( context->get_parent_node_3d() ); // CesiumGeoreference
        if ( georeference != nullptr )
        {
            pCoordinateSystem = &georeference->getCoordinateSystem();
        }

		std::vector<ViewState> result;
        Camera3D *_camera;
		if ( Engine::get_singleton()->is_editor_hint() )
        {
            EditorScript temp_editor_script;
            EditorInterface *editor_interface = temp_editor_script.get_editor_interface();
            TypedArray<Camera3D> cam_array = TypedArray<Camera3D>();
            _find_cameras( editor_interface->get_editor_main_screen()->get_children(),
                           editor_interface->get_edited_scene_root(), cam_array );
            if ( !cam_array.is_empty() )
            {
                WARN_PRINT( "Connecting to the first editor camera" );
                //LOG( DEBUG, "Connecting to the first editor camera" );
                _camera = Object::cast_to<Camera3D>( cam_array[0] );
            }
		}
        else
        {
            _camera = context->get_viewport()->get_camera_3d();
        }

        result.emplace_back(
            godotCameraToViewState( pCoordinateSystem, godotWorldToTileset, *_camera )
        );
	}

} // CesiumForGodot