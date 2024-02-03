//
// Created by Harris.Lu on 2024/1/31.
//

#include "CesiumGeoreference.h"
#include <godot_cpp/core/class_db.hpp>
#include "CesiumWgs84Ellipsoid.h"

namespace CesiumForGodot {

	CesiumGeospatial::LocalHorizontalCoordinateSystem createCoordinateSystem(
        const CesiumGeoreference &georeference )
    {
    
	}

	CesiumGeoreference::CesiumGeoreference()
    {
    
	}

	CesiumGeoreference::~CesiumGeoreference()
    {
    }

	void CesiumGeoreference::Initialize()
	{
        if ( !this->_isInitialized )
        {
            this->_isInitialized = true;
            this->UpdateOtherCoordinates();
            this->UpdateTransformations();
        }
	}

    void CesiumGeoreference::UpdateOtherCoordinates()
    {
        if ( this->_originAuthority == CesiumGeoreferenceOriginAuthority::LongitudeLatitudeHeight )
		{
            Vector3 ecef = CesiumWgs84Ellipsoid::LongitudeLatitudeHeightToEarthCenteredEarthFixed(Vector3(this->_longitude, this->_latitude, this->_height));
            this->_ecefX = ecef.x;
            this->_ecefY = ecef.y;
            this->_ecefZ = ecef.z;
        }
        else if (this->_originAuthority == CesiumGeoreferenceOriginAuthority::EarthCenteredEarthFixed)
        {
            Vector3 l1h = CesiumWgs84Ellipsoid::EarthCenteredEarthFixedToLongitudeLatitudeHeight(Vector3(this->_ecefX, this->_ecefY, this->_ecefZ));
            this->_longitude = l1h.x;
            this->_latitude = l1h.y;
            this->_height = l1h.z;
        }
    }

    void CesiumGeoreference::UpdateTransformations()
    {
        this->_localToEcef = this->ComputeLocalToEarthCenteredEarthFixedTransformation();
        this->_ecefToLocal = Transform3D(this->_localToEcef).inverse();
    }

	void CesiumGeoreference::MoveOrigin()
    {
        if ( !this->_isInitialized )
        {
            WARN_PRINT( "The origin of a CesiumGeoreference must not be set before its Initialize method is called, either explicitly or via OnEnable." );
        }

        if ( this->_scale < 1e-8 )
        {
            this->_scale = 1e-8;
        }

        Transform3D oldLocalToEcef = this->_localToEcef;

        this->UpdateOtherCoordinates();

        if (oldLocalToEcef == this->_localToEcef)
		{
			return;
		}

       /*  foreach ( CesiumGlobeAnchor anchor in this._globeAnchors )
        {
            if ( anchor == null )
            {
                continue;
            }

            anchor.Sync();
        }

        if ( this.changed != null )
        {
            this.changed();
        }*/

    }

	void CesiumGeoreference::set_originAuthority( CesiumGeoreferenceOriginAuthority value )
	{
        this->_originAuthority = value;
        this->MoveOrigin();
	}

	Transform3D CesiumGeoreference::ComputeLocalToEarthCenteredEarthFixedTransformation()
	{
        this->_coordinateSystem = createCoordinateSystem(*this);
        return GodotTransforms::toGodotMathematics(
            this->_coordinateSystem->getLocalToEcefTransformation() );
	}

	const CesiumGeospatial::LocalHorizontalCoordinateSystem &CesiumGeoreference::
        getCoordinateSystem()
	{
        if ( !this->_coordinateSystem )
        {
            this->ComputeLocalToEarthCenteredEarthFixedTransformation();
        }
        return *this->_coordinateSystem;
	}

	void CesiumGeoreference::_notification(int p_what)
	{
        switch ( p_what )
        {
            case NOTIFICATION_READY:
                this->Initialize();
				break;
            default:
                break;
        }
		
	}

	void CesiumGeoreference::_bind_methods() {
   
		//ADD_GROUP("Origin", "Longitude Latitude Height");
  //      ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "longitude", PROPERTY_HINT_RANGE, "-180, 180, 0.01" ),
  //                    "set_longitude", "get_longitude" );
  //      ADD_PROPERTY(
  //          PropertyInfo( Variant::FLOAT, "latitude", PROPERTY_HINT_RANGE, "-90, 90, 0.01" ),
  //          "set_latitude", "get_latitude" );
		//ADD_PROPERTY(
		//	PropertyInfo( Variant::FLOAT, "height"),
		//	"set_height", "get_height" );

		//ADD_GROUP( "Position", "Earth-Centered, Earth-Fixed" );
  //      ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ecefX" ), "set_ecefX", "get_ecefX" );
		//ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ecefY" ), "set_ecefY", "get_ecefY" );
		//ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ecefZ" ), "set_ecefZ", "get_ecefZ" );
	}

} // CesiumForGodot