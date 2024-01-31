//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_CESIUMGEOREFERENCE_H
#define GODOT_CESIUMGEOREFERENCE_H

#include <godot_cpp/classes/node3d.hpp>
#include <CesiumGeospatial/LocalHorizontalCoordinateSystem.h>
#include <optional>

using namespace godot;
namespace CesiumForGodot {

    class CesiumGeoreference : public Node3D {
        GDCLASS(CesiumGeoreference, Node3D);

    public:
        CesiumGeoreference();
        ~CesiumGeoreference();

        void set_longitude( double longitude );
        double get_longitude()
        {
            return _longitude;
        };

        void set_latitude( double latitude );
        double get_latitude()
        {
			return _latitude;
		};

        void set_height( double height );
        double get_height()
        {
            return _height;
        };

        void set_ecefX( double ecefX );
        double get_ecefX()
        {
			return _ecefX;
		};

        void set_ecefY( double ecefY );
		double get_ecefY()
        {
            return _ecefY;
        }

        void set_ecefZ( double ecefZ );
        double get_ecefZ()
        {
			return _ecefZ;
		}

    protected:
        void _notification( int p_what );
		static void _bind_methods();

    private:
        double _latitude = 39.736401;
        double _longitude = -105.25737;
        double _height = 2250.0;

        double _ecefX = 6378137.0;
        double _ecefY = 0.0;
        double _ecefZ = 0.0;

        std::optional<CesiumGeospatial::LocalHorizontalCoordinateSystem> _coordinateSystem;
    };

} // CesiumForGodot

#endif //GODOT_CESIUMGEOREFERENCE_H
