//
// Created by Harris.Lu on 2024/1/31.
//
#pragma warning(disable : 4189)
#include "TestRendering.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include "constants.h"
#include "GodotExternals.h"
#include "Cesium3DTilesSelection/TilesetExternals.h"
#include <godot_cpp/classes/world3d.hpp>
#include "GodotPrepareRendererResources.h"

namespace CesiumForGodot {

    TestRendering::TestRendering() {

    }

    void TestRendering::__ready() {
        RID scenario = get_world_3d()->get_scenario();
        RID test_cube = RS->get_test_cube();

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

        //获取shader参数列表
        //param_list = RS->get_shader_parameter_list( _shader );

        RS->material_set_shader( _material, _shader );

        RS->mesh_surface_set_material( test_cube, 0, _material );

        //实例化到场景中
        RS->instance_create2(test_cube, scenario);

        WARN_PRINT("TestRendering::__ready");


        const std::shared_ptr<CesiumAsync::IAssetAccessor>& pAssetAccessor = getAssetAccessor();
        const CesiumAsync::AsyncSystem& asyncSystem = getAsyncSystem();

        Cesium3DTilesSelection::TilesetExternals externals{
            pAssetAccessor,
            std::make_shared<GodotPrepareRendererResources>( this ),
            asyncSystem,
            
        };
        
	}

    void TestRendering::_notification(int p_what) {
        switch ( p_what )
        {
            case NOTIFICATION_READY:
            {
                __ready();
                break;
            }
            default:
                break;
        }

    }

    void TestRendering::_bind_methods() {

    }

} // CesiumForGodot