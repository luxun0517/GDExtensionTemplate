//
// Created by Harris.Lu on 2024/1/31.
//

#include "CesiumGeoreference.h"
#include <godot_cpp/core/class_db.hpp>

namespace CesiumForGodot {

	CesiumGeoreference::CesiumGeoreference()
    {
    
	}

	CesiumGeoreference::~CesiumGeoreference()
    {
    }

	void CesiumGeoreference::set_longitude(double longitude)
	{
		_longitude = longitude;
	}

	void CesiumGeoreference::set_latitude(double latitude)
	{
		_latitude = latitude;
	}

	void CesiumGeoreference::set_height(double height)
	{
		_height = height;
	}

	void CesiumGeoreference::set_ecefX(double ecefX)
	{
		_ecefX = ecefX;
	}

	void CesiumGeoreference::set_ecefY(double ecefY)
	{
		_ecefY = ecefY;
	}

	void CesiumGeoreference::set_ecefZ(double ecefZ)
	{
		_ecefZ = ecefZ;
	}

	void CesiumGeoreference::_notification(int p_what)
	{
		
	}

	void CesiumGeoreference::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_longitude", "longitude"), &CesiumGeoreference::set_longitude);
		ClassDB::bind_method(D_METHOD("get_longitude"), &CesiumGeoreference::get_longitude);
		ClassDB::bind_method(D_METHOD("set_latitude", "latitude"), &CesiumGeoreference::set_latitude);
		ClassDB::bind_method(D_METHOD("get_latitude"), &CesiumGeoreference::get_latitude);
		ClassDB::bind_method(D_METHOD("set_height", "height"), &CesiumGeoreference::set_height);
		ClassDB::bind_method(D_METHOD("get_height"), &CesiumGeoreference::get_height);

		ClassDB::bind_method(D_METHOD("set_ecefX", "ecefX"), &CesiumGeoreference::set_ecefX);
		ClassDB::bind_method(D_METHOD("get_ecefX"), &CesiumGeoreference::get_ecefX);
		ClassDB::bind_method(D_METHOD("set_ecefY", "ecefY"), &CesiumGeoreference::set_ecefY);
		ClassDB::bind_method(D_METHOD("get_ecefY"), &CesiumGeoreference::get_ecefY);
		ClassDB::bind_method(D_METHOD("set_ecefZ", "ecefZ"), &CesiumGeoreference::set_ecefZ);
		ClassDB::bind_method(D_METHOD("get_ecefZ"), &CesiumGeoreference::get_ecefZ);

		ADD_GROUP("Origin", "Longitude Latitude Height");
        ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "longitude", PROPERTY_HINT_RANGE, "-180, 180, 0.01" ),
                      "set_longitude", "get_longitude" );
        ADD_PROPERTY(
            PropertyInfo( Variant::FLOAT, "latitude", PROPERTY_HINT_RANGE, "-90, 90, 0.01" ),
            "set_latitude", "get_latitude" );
		ADD_PROPERTY(
			PropertyInfo( Variant::FLOAT, "height"),
			"set_height", "get_height" );

		ADD_GROUP( "Position", "Earth-Centered, Earth-Fixed" );
        ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ecefX" ), "set_ecefX", "get_ecefX" );
		ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ecefY" ), "set_ecefY", "get_ecefY" );
		ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "ecefZ" ), "set_ecefZ", "get_ecefZ" );
	}

} // CesiumForGodot