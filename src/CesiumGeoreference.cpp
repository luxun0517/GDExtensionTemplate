//
// Created by Harris.Lu on 2024/1/31.
//
#pragma warning(disable : 4834)
#include "CesiumGeoreference.h"
#include <godot_cpp/core/class_db.hpp>
#include "CesiumWgs84Ellipsoid.h"

using namespace CesiumGeospatial;
using namespace CesiumUtility;
namespace CesiumForGodot {

    LocalHorizontalCoordinateSystem createCoordinateSystem(
        const CesiumGeoreference &georeference )
    {
        if ( georeference.get_origin_authority() ==
             CesiumGeoreferenceOriginAuthority::LongitudeLatitudeHeight )
        {
            return LocalHorizontalCoordinateSystem( 
                Cartographic::fromDegrees(
				    georeference.get_longitude(), 
                    georeference.get_latitude(),
				    georeference.get_height() 
                ),
                LocalDirection::East,
                LocalDirection::Up,
                LocalDirection::North,
                1.0 / georeference.get_scale()
		    );
		}
		else
		{
            return LocalHorizontalCoordinateSystem(
                glm::dvec3( georeference.get_ecefX(), georeference.get_ecefY(),
                            georeference.get_ecefZ() ),
                LocalDirection::East, LocalDirection::Up, LocalDirection::North,
                1.0 / georeference.get_scale() );
        
        }
	}

	CesiumGeoreference::CesiumGeoreference()
    {
    
	}

	CesiumGeoreference::~CesiumGeoreference()
    {
    }
    
	void CesiumGeoreference::set_originAuthority( CesiumGeoreferenceOriginAuthority value )
    {
        this->_originAuthority = value;
        this->MoveOrigin();
    }

    void CesiumGeoreference::set_latitude( double value )
    {
        this->_latitude = value;
		this->set_originAuthority( CesiumGeoreferenceOriginAuthority::LongitudeLatitudeHeight );
    }

    void CesiumGeoreference::set_longitude( double value )
	{
		this->_longitude = value;
        this->set_originAuthority( CesiumGeoreferenceOriginAuthority::LongitudeLatitudeHeight);
	}

    void CesiumGeoreference::set_height( double value )
    {
        this->_height = value;
		this->set_originAuthority( CesiumGeoreferenceOriginAuthority::LongitudeLatitudeHeight );
    }

    void CesiumGeoreference::set_ecefX( double value )
	{
		this->_ecefX = value;
		this->set_originAuthority( CesiumGeoreferenceOriginAuthority::EarthCenteredEarthFixed );
	}

    void CesiumGeoreference::set_ecefY( double value )
    {
        this->_ecefY = value;
		this->set_originAuthority( CesiumGeoreferenceOriginAuthority::EarthCenteredEarthFixed );
    }

    void CesiumGeoreference::set_ecefZ( double value )
    {
        this->_ecefZ = value;
        this->set_originAuthority( CesiumGeoreferenceOriginAuthority::EarthCenteredEarthFixed );
    }

    void CesiumGeoreference::set_scale( double value )
	{
		this->_scale = value;
		this->MoveOrigin();
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

	Transform3D CesiumGeoreference::
        ComputeLocalToEarthCenteredEarthFixedTransformation()
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
            case NOTIFICATION_ENTER_TREE:
                WARN_PRINT( "NOTIFICATION_ENTER_TREE" );
                break;
            default:
                break;
        }
		
	}

	void CesiumGeoreference::_bind_methods() {
        ClassDB::bind_method( D_METHOD( "get_longitude" ), &CesiumGeoreference::get_longitude );
        ClassDB::bind_method( D_METHOD( "set_longitude", "value" ), &CesiumGeoreference::set_longitude );
        ClassDB::bind_method( D_METHOD( "get_latitude" ), &CesiumGeoreference::get_latitude );
        ClassDB::bind_method( D_METHOD( "set_latitude", "value" ), &CesiumGeoreference::set_latitude );
        ClassDB::bind_method( D_METHOD( "get_height" ), &CesiumGeoreference::get_height );
        ClassDB::bind_method( D_METHOD( "set_height", "value" ), &CesiumGeoreference::set_height );

        ClassDB::bind_method( D_METHOD( "get_ecefX" ), &CesiumGeoreference::get_ecefX );
        ClassDB::bind_method( D_METHOD( "set_ecefX", "value" ), &CesiumGeoreference::set_ecefX );
        ClassDB::bind_method( D_METHOD( "get_ecefY" ), &CesiumGeoreference::get_ecefY );
        ClassDB::bind_method( D_METHOD( "set_ecefY", "value" ), &CesiumGeoreference::set_ecefY );
        ClassDB::bind_method( D_METHOD( "get_ecefZ" ), &CesiumGeoreference::get_ecefZ );
        ClassDB::bind_method( D_METHOD( "set_ecefZ", "value" ), &CesiumGeoreference::set_ecefZ );
   
		ADD_GROUP("Origin", "Longitude Latitude Height");
        ADD_PROPERTY(
            PropertyInfo( Variant::FLOAT, "latitude", PROPERTY_HINT_RANGE, "-90, 90, 0.01" ),
            "set_latitude", "get_latitude" );
        ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "longitude", PROPERTY_HINT_RANGE, "-180, 180, 0.01" ),
                      "set_longitude", "get_longitude" );
		ADD_PROPERTY(
			PropertyInfo( Variant::FLOAT, "height"),
			"set_height", "get_height" );

		ADD_GROUP( "Position", "Earth-Centered, Earth-Fixed" );
        ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ECEF X" ), "set_ecefX", "get_ecefX" );
		ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ECEF Y" ), "set_ecefY", "get_ecefY" );
		ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ECEF Z" ), "set_ecefZ", "get_ecefZ" );
	}

} // CesiumForGodot