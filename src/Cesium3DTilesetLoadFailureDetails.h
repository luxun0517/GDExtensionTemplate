//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_CESIUM3DTILESETLOADFAILUREDETAILS_H
#define CESIUM_GODOT_CESIUM3DTILESETLOADFAILUREDETAILS_H

#include <cstdint>
#include <godot_cpp/classes/ref.hpp>
//#include "GD3DTileset.h"
class GD3DTileset;
using namespace godot;
namespace CesiumForGodot {

    enum class Cesium3DTilesetLoadType : uint8_t
    {
        /**
         * An unknown load error.
         */
        Unknown,

        /**
         * A Cesium ion asset endpoint.
         */
        CesiumIon,

        /**
         * A tileset.json.
         */
        TilesetJson
    };

    struct Cesium3DTilesetLoadFailureDetails
    {
        std::weak_ptr<GD3DTileset> tileset;

        Cesium3DTilesetLoadType type = Cesium3DTilesetLoadType::Unknown;

        uint32_t httpStatusCode = 0;

        String message;
    };

} // CesiumForGodot

#endif //CESIUM_GODOT_CESIUM3DTILESETLOADFAILUREDETAILS_H
