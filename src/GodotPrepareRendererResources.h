//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_GODOTPREPARERENDERERRESOURCES_H
#define GODOT_GODOTPREPARERENDERERRESOURCES_H

#include <Cesium3DTilesSelection/IPrepareRendererResources.h>
#include "CesiumShaderProperties.h"
#include <godot_cpp/variant/rid.hpp>

namespace CesiumForGodot {

    //gltf 原始图元转换为Godot的Mesh数据
    struct CesiumPrimitiveInfo
    {
        //是否为POINTS渲染
        bool containsPoints = false;

        bool isTranslucent = false;

        bool isUnlit = false;
    
        /**
         * @brief Maps a texture coordinate index i (TEXCOORD_<i>) to the
         * corresponding Godot texture coordinate index.
         */
        std::unordered_map<uint32_t, uint32_t> uvIndexMap{};

        /**
         * @brief Maps an overlay texture coordinate index i (_CESIUMOVERLAY_<i>) to
         * the corresponding Godot texture coordinate index.
         */
        std::unordered_map<uint32_t, uint32_t> rasterOverlayUvIndexMap{};
    };

    struct CesiumGltfRID
    {
        std::unique_ptr<RID> pGameObject{};
        std::vector<CesiumPrimitiveInfo> primitiveInfos{};
    };

    class GodotPrepareRendererResources : public Cesium3DTilesSelection::IPrepareRendererResources {

    public:
        GodotPrepareRendererResources(const RID &tileset);

    private:
        RID _tileset;
        CesiumShaderProperties _shaderProperty;
    };

} // CesiumForGodot

#endif //GODOT_GODOTPREPARERENDERERRESOURCES_H
