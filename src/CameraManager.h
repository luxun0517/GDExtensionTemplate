//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_CAMERAMANAGER_H
#define GODOT_CAMERAMANAGER_H

#include <Cesium3DTilesSelection/ViewState.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <vector>
#include "GD3DTileset.h"

using namespace godot;
namespace CesiumForGodot {

    class CameraManager {

    public:

        static std::vector<Cesium3DTilesSelection::ViewState> getAllCameras(
            const GD3DTileset &context );
    };

} // CesiumForGodot

#endif //GODOT_CAMERAMANAGER_H
