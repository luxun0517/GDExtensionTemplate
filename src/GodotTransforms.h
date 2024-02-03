//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_GODOTTRANSFORMS_H
#define CESIUM_GODOT_GODOTTRANSFORMS_H

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>

using namespace godot;
namespace CesiumForGodot {

    class GodotTransforms {
    public:
        static Vector3 toGodot( const glm::dvec3 &vector );

        static glm::dmat4 fromGodot(const Transform3D &transform);

        static Transform3D toGodotMathematics( const glm::dmat4 &transform );

    };

} // CesiumForGodot

#endif //CESIUM_GODOT_GODOTTRANSFORMS_H
