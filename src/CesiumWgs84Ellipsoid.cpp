//
// Created by Harris.Lu on 2024/2/3.
//

#include "CesiumWgs84Ellipsoid.h"
#include <CesiumGeospatial/Ellipsoid.h>
#include <CesiumUtility/Math.h>

using namespace CesiumGeospatial;
using namespace CesiumUtility;

namespace CesiumForGodot {

	Vector3 CesiumWgs84Ellipsoid::GetRadii()
    {
        const glm::dvec3 radii = Ellipsoid::WGS84.getRadii();
        return Vector3(radii.x, radii.y, radii.z);
    }

    std::optional<Vector3> CesiumWgs84Ellipsoid::ScaleToGeodeticSurface(
        Vector3 earthCenteredEarthFixed )
    {
        const glm::dvec3 cartesian( 
            earthCenteredEarthFixed.x, 
            earthCenteredEarthFixed.y,
            earthCenteredEarthFixed.z 
        );
        auto result = Ellipsoid::WGS84.scaleToGeodeticSurface( cartesian );
        if ( result )
        {
            return Vector3( result->x, result->y, result->z );
        }

        return std::nullopt;
    }

    Vector3 CesiumWgs84Ellipsoid::LongitudeLatitudeHeightToEarthCenteredEarthFixed(
        Vector3 longitudeLatitudeHeight )
    {
        glm::dvec3 cartesian = Ellipsoid::WGS84.cartographicToCartesian( Cartographic::fromDegrees(
            longitudeLatitudeHeight.x, longitudeLatitudeHeight.y, longitudeLatitudeHeight.z ) );
        return Vector3( cartesian.x, cartesian.y, cartesian.z );
    }

    Vector3 CesiumWgs84Ellipsoid::EarthCenteredEarthFixedToLongitudeLatitudeHeight(
        Vector3 earthCenteredEarthFixed )
    {
        std::optional<Cartographic> result = Ellipsoid::WGS84.cartesianToCartographic( glm::dvec3(
            earthCenteredEarthFixed.x, earthCenteredEarthFixed.y, earthCenteredEarthFixed.z ) );
        if ( result )
        {
            return Vector3( CesiumUtility::Math::radiansToDegrees( result->longitude ),
                            CesiumUtility::Math::radiansToDegrees( result->latitude ),
                            result->height );
        }
        else
        {
            return Vector3( 0.0, 0.0, 0.0 );
        }
    }

} // CesiumForGodot