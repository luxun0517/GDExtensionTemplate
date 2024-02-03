//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_CESIUMWGS84ELLIPSOID_H
#define CESIUM_GODOT_CESIUMWGS84ELLIPSOID_H

#include <godot_cpp/variant/vector3.hpp>
#include <optional>

using namespace godot;
namespace CesiumForGodot {

    class CesiumWgs84Ellipsoid {

    public:
        static Vector3 GetRadii();

        static std::optional<Vector3> ScaleToGeodeticSurface( Vector3 earthCenteredEarthFixed );

        static Vector3 LongitudeLatitudeHeightToEarthCenteredEarthFixed(
            Vector3 longitudeLatitudeHeight );

        static Vector3 EarthCenteredEarthFixedToLongitudeLatitudeHeight(
            Vector3 earthCenteredEarthFixed );

    };

} // CesiumForGodot

#endif //CESIUM_GODOT_CESIUMWGS84ELLIPSOID_H
