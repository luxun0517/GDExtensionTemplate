//
// Created by Harris.Lu on 2024/1/28.
//

#include "GD3DTileset.h"
#include <godot_cpp/core/class_db.hpp>

namespace Cesium {

	GD3DTileset::GD3DTileset() {

	}

	GD3DTileset::~GD3DTileset() {

	}
		
	/// <summary>
    /// …Ë÷√3dtilesetµƒurl
    /// </summary>
    /// <param name="url"></param>
    void GD3DTileset::set_url( String url )
    {
        _url = url;
    }

    void GD3DTileset::Start()
	{
		
	}

    void GD3DTileset::Update( double delta )
	{

        if ( !this->_pTileset )
        {
            this->LoadTileset();
            if ( !this->_pTileset )
            {
                return;
            }
        }
		
	}

	void GD3DTileset::LoadTileset()
    {
        if ( this->_pTileset )
        {
            // Tileset already loaded, do nothing.
            return;
        }
    }

	void GD3DTileset::_bind_methods() {
        ClassDB::bind_method( D_METHOD( "set_url", "url"), &GD3DTileset::set_url);
        ClassDB::bind_method( D_METHOD( "get_url" ), &GD3DTileset::get_url);

		ADD_GROUP( "Renderer", "render_" );
        ADD_PROPERTY( PropertyInfo( Variant::STRING, "Tileset Url", PROPERTY_HINT_TYPE_STRING ),
                      "set_url", "get_url" );
	}

	void GD3DTileset::_notification(int p_what) {
        switch ( p_what )
        {
            case NOTIFICATION_READY:
            {
                Start();
                WARN_PRINT( "NOTIFICATION_READY" );
                break;
            }
            case NOTIFICATION_PROCESS:
			{
                Update(get_process_delta_time());
				break;
			}
            case NOTIFICATION_ENTER_TREE:
			{
				WARN_PRINT( "NOTIFICATION_ENTER_TREE" );
				break;
			}
            default:
                break;
        }
	}

} // Cesium

///**
// * If running in the editor, recurses into the editor scene tree to find the editor cameras and
// * grabs the first one. The edited_scene_root is excluded in case the user already has a Camera3D in
// * their scene.
// */
//void Terrain3D::_grab_camera()
//{
//    if ( Engine::get_singleton()->is_editor_hint() )
//    {
//        EditorScript temp_editor_script;
//        EditorInterface *editor_interface = temp_editor_script.get_editor_interface();
//        TypedArray<Camera3D> cam_array = TypedArray<Camera3D>();
//        _find_cameras( editor_interface->get_editor_main_screen()->get_children(),
//                       editor_interface->get_edited_scene_root(), cam_array );
//        if ( !cam_array.is_empty() )
//        {
//            LOG( DEBUG, "Connecting to the first editor camera" );
//            _camera = Object::cast_to<Camera3D>( cam_array[0] );
//        }
//    }
//    else
//    {
//        LOG( DEBUG, "Connecting to the in-game viewport camera" );
//        _camera = get_viewport()->get_camera_3d();
//    }
//    if ( !_camera )
//    {
//        set_process( false );
//        LOG( ERROR, "Cannot find active camera. Stopping _process()" );
//    }
//}