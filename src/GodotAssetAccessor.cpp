//
// Created by Harris.Lu on 2024/1/30.
//

#include "GodotAssetAccessor.h"
#include "Cesium.h"
#include "godot_cpp/classes/project_settings.hpp"
#include "godot_cpp/classes/os.hpp"

using namespace CesiumAsync;

using namespace godot;
namespace CesiumForGodot {

	GodotAssetAccessor::GodotAssetAccessor() : _cesiumRequestHeaders() {
        std::string version = Cesium::version + " " + Cesium::commit;

        String projectName = ProjectSettings::get_singleton()->get_setting("application/config/name");
        String engine =
            String("Godot Engine ") + String(ProjectSettings::get_singleton()->get_setting( "application/config/version" ));
        //Project Name : cesium - godot, Engine:
        String osVersion = OS::get_singleton()->get_name();
        
        this->_cesiumRequestHeaders.insert( { "X-Cesium-Client", "Cesium For Unity" } );
        this->_cesiumRequestHeaders.insert( { "X-Cesium-Client-Version", version } );
        this->_cesiumRequestHeaders.insert(
            { "X-Cesium-Client-Project", projectName.utf8().get_data() } );
        this->_cesiumRequestHeaders.insert(
            { "X-Cesium-Client-Engine", engine.utf8().get_data() } );
        this->_cesiumRequestHeaders.insert( { "X-Cesium-Client-OS", osVersion.utf8().get_data() } );
	}

    CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> 
        GodotAssetAccessor::get(
            const CesiumAsync::AsyncSystem& asyncSystem,
            const std::string& url,
            const std::vector<THeader>& headers) {
        WARN_PRINT(vformat("GodotAssetAccessor::get: %s", url.c_str()));
        return asyncSystem.createFuture<std::shared_ptr<CesiumAsync::IAssetRequest>>( 
            [&url, &headers, &cesiumRequestHeaders = this->_cesiumRequestHeaders]() {
            

        });
	}
	

} // Cesium