//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_CAMERAMANAGER_H
#define GODOT_CAMERAMANAGER_H

#include <Cesium3DTilesSelection/ViewState.h>
#include <godot_cpp/classes/camera3d.hpp>
#include "GD3DTileset.h"

using namespace godot;
namespace CesiumForGodot {

    class CameraManager {

    public:

        static std::vector<Cesium3DTilesSelection::ViewState> getAllCameras(
            const Ref<GD3DTileset> &context );
    };

    
///**
//     * If running in the editor, recurses into the editor scene tree to find the editor cameras and
//     * grabs the first one. The edited_scene_root is excluded in case the user already has a
//     * Camera3D in their scene.
//     */
//    void Terrain3D::_grab_camera()
//    {
//        if ( Engine::get_singleton()->is_editor_hint() )
//        {
//            EditorScript temp_editor_script;
//            EditorInterface *editor_interface = temp_editor_script.get_editor_interface();
//            TypedArray<Camera3D> cam_array = TypedArray<Camera3D>();
//            _find_cameras( editor_interface->get_editor_main_screen()->get_children(),
//                           editor_interface->get_edited_scene_root(), cam_array );
//            if ( !cam_array.is_empty() )
//            {
//                LOG( DEBUG, "Connecting to the first editor camera" );
//                _camera = Object::cast_to<Camera3D>( cam_array[0] );
//            }
//        }
//        else
//        {
//            LOG( DEBUG, "Connecting to the in-game viewport camera" );
//            _camera = get_viewport()->get_camera_3d();
//        }
//        if ( !_camera )
//        {
//            set_process( false );
//            LOG( ERROR, "Cannot find active camera. Stopping _process()" );
//        }
//    }
//
//    /**
//     * Recursive helper function for _grab_camera().
//     */
//    void Terrain3D::_find_cameras( TypedArray<Node> from_nodes, Node *excluded_node,
//                                   TypedArray<Camera3D> &cam_array )
//    {
//        for ( int i = 0; i < from_nodes.size(); i++ )
//        {
//            Node *node = Object::cast_to<Node>( from_nodes[i] );
//            if ( node != excluded_node )
//            {
//                _find_cameras( node->get_children(), excluded_node, cam_array );
//            }
//            if ( node->is_class( "Camera3D" ) )
//            {
//                LOG( DEBUG, "Found a Camera3D at: ", node->get_path() );
//                cam_array.push_back( node );
//            }
//        }
//    }


} // CesiumForGodot

#endif //GODOT_CAMERAMANAGER_H
