//
// Created by Harris.Lu on 2024/1/31.
//

#include "CesiumShaderProperties.h"

namespace CesiumForGodot {

	CesiumShaderProperties::CesiumShaderProperties()
    {
        baseColorFactorID = "_baseColorFactor";
        metallicRoughnessFactorID = "_metallicRoughnessFactor";
        baseColorTextureID = "_baseColorTexture";
        baseColorTextureCoordinateIndexID = "_baseColorTextureCoordinateIndex";
        metallicRoughnessTextureID = "_metallicRoughnessTexture";
        metallicRoughnessTextureCoordinateIndexID = "_metallicRoughnessTextureCoordinateIndex";
        normalMapTextureID = "_normalMapTexture";
        normalMapTextureCoordinateIndexID = "_normalMapTextureCoordinateIndex";
        normalMapScaleID = "_normalMapScale";
        occlusionTextureID = "_occlusionTexture";
        occlusionTextureCoordinateIndexID = "_occlusionTextureCoordinateIndex";
        occlusionStrengthID = "_occlusionStrength";
        emissiveFactorID = "_emissiveFactor";
        emissiveTextureID = "_emissiveTexture";
        emissiveTextureCoordinateIndexID = "_emissiveTextureCoordinateIndex";

        overlayTextureCoordinateIndexID = {
            "_overlay0TextureCoordinateIndex",
            "_overlay1TextureCoordinateIndex",
            "_overlay2TextureCoordinateIndex",
            "_overlay3TextureCoordinateIndex",
        };

        overlayTextureID = {
			"_overlay0Texture",
			"_overlay1Texture",
			"_overlay2Texture",
			"_overlay3Texture",
		};

        overlayTranslationAndScaleID = { 
            "_overlay0TranslationAndScale",
            "_overlay1TranslationAndScale",
            "_overlay2TranslationAndScale",
            "_overlay3TranslationAndScale",
        };
    }

} // CesiumForGodot