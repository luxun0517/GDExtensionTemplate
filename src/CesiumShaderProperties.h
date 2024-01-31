//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_CESIUMSHADERPROPERTIES_H
#define GODOT_CESIUMSHADERPROPERTIES_H

#include <vector>
#include <godot_cpp/variant/string.hpp>

using namespace godot;
namespace CesiumForGodot {

    class CesiumShaderProperties {

    public:
        CesiumShaderProperties();

        const String getBaseColorFactorID() const
        {
            return baseColorFactorID;
        }
        const String getMetallicRoughnessFactorID() const
        {
            return metallicRoughnessFactorID;
        }
        const String getBaseColorTextureID() const
        {
            return baseColorTextureID;
        }
        const String getBaseColorTextureCoordinateIndexID() const
        {
            return baseColorTextureCoordinateIndexID;
        }
        const String getMetallicRoughnessTextureID() const
        {
            return metallicRoughnessTextureID;
        }
        const String getMetallicRoughnessTextureCoordinateIndexID() const
        {
            return metallicRoughnessTextureCoordinateIndexID;
        }
        const String getNormalMapTextureID() const
        {
            return normalMapTextureID;
        }
        const String getNormalMapTextureCoordinateIndexID() const
        {
            return normalMapTextureCoordinateIndexID;
        }
        const String getNormalMapScaleID() const
        {
            return normalMapScaleID;
        }
        const String getOcclusionTextureID() const
        {
            return occlusionTextureID;
        }
        const String getOcclusionTextureCoordinateIndexID() const
        {
            return occlusionTextureCoordinateIndexID;
        }
        const String getOcclusionStrengthID() const
        {
            return occlusionStrengthID;
        }
        const String getEmissiveFactorID() const
        {
            return emissiveFactorID;
        }
        const String getEmissiveTextureID() const
        {
            return emissiveTextureID;
        }
        const String getEmissiveTextureCoordinateIndexID() const
        {
            return emissiveTextureCoordinateIndexID;
        }

        const String getOverlayTextureCoordinateIndexID( int32_t index )
        {
            return overlayTextureCoordinateIndexID[index];
        }
        const String getOverlayTextureID( int32_t index )
        {
            return overlayTextureID[index];
        }
        const String getOverlayTranslationAndScaleID( int32_t index )
        {
            return overlayTranslationAndScaleID[index];
        }

    private:
        String baseColorFactorID;
        String metallicRoughnessFactorID;
        String baseColorTextureID;
        String baseColorTextureCoordinateIndexID;
        String metallicRoughnessTextureID;
        String metallicRoughnessTextureCoordinateIndexID;
        String normalMapTextureID;
        String normalMapTextureCoordinateIndexID;
        String normalMapScaleID;
        String occlusionTextureID;
        String occlusionTextureCoordinateIndexID;
        String occlusionStrengthID;
        String emissiveFactorID;
        String emissiveTextureID;
        String emissiveTextureCoordinateIndexID;

        std::vector<String> overlayTextureCoordinateIndexID;
        std::vector<String> overlayTextureID;
        std::vector<String> overlayTranslationAndScaleID;

    };

} // CesiumForGodot

#endif //GODOT_CESIUMSHADERPROPERTIES_H
