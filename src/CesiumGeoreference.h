//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_CESIUMGEOREFERENCE_H
#define GODOT_CESIUMGEOREFERENCE_H

#include <godot_cpp/classes/node3d.hpp>
#include <CesiumGeospatial/LocalHorizontalCoordinateSystem.h>
#include <optional>
#include "GodotTransforms.h"

using namespace godot;
namespace CesiumForGodot {

    enum class CesiumGeoreferenceOriginAuthority {
        LongitudeLatitudeHeight,
        EarthCenteredEarthFixed
	};

    class CesiumGeoreference : public Node3D {
        GDCLASS(CesiumGeoreference, Node3D);

    public:
        CesiumGeoreference();
        ~CesiumGeoreference();

        void Initialize();

        Transform3D ComputeLocalToEarthCenteredEarthFixedTransformation();

        const CesiumGeospatial::LocalHorizontalCoordinateSystem &getCoordinateSystem(); 

        void MoveOrigin();

        CesiumGeoreferenceOriginAuthority get_origin_authority() const
        {
            return this->_originAuthority;
        }
        void set_originAuthority( CesiumGeoreferenceOriginAuthority value );

    protected:
        void _notification( int p_what );
		static void _bind_methods();

    private:
        bool _isInitialized = false;

        CesiumGeoreferenceOriginAuthority _originAuthority = CesiumGeoreferenceOriginAuthority::LongitudeLatitudeHeight;

        double _latitude = 39.736401;

        double _longitude = -105.25737;

        double _height = 2250.0;

        double _ecefX = 6378137.0;

        double _ecefY = 0.0;

        double _ecefZ = 0.0;

        double _scale = 1.0;

        std::optional<CesiumGeospatial::LocalHorizontalCoordinateSystem> _coordinateSystem;

        Transform3D _localToEcef;
        Transform3D _ecefToLocal;

        void UpdateOtherCoordinates();
        void UpdateTransformations();
    };

} // CesiumForGodot

#endif //GODOT_CESIUMGEOREFERENCE_H
