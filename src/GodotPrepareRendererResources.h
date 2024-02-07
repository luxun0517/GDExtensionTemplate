//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_GODOTPREPARERENDERERRESOURCES_H
#define GODOT_GODOTPREPARERENDERERRESOURCES_H

#include <godot_cpp/variant/rid.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <Cesium3DTilesSelection/IPrepareRendererResources.h>
#include "CesiumShaderProperties.h"
#include "GD3DTileset.h"

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

    struct CesiumGltfGameObject
    {
        std::unique_ptr<RID> pGameObject{};
        std::vector<CesiumPrimitiveInfo> primitiveInfos{};
    };

    class GodotPrepareRendererResources : public Cesium3DTilesSelection::IPrepareRendererResources {

    public:
        GodotPrepareRendererResources( const Ref<GD3DTileset> &tileset );

        virtual CesiumAsync::Future<Cesium3DTilesSelection::TileLoadResultAndRenderResources>
            prepareInLoadThread(
                const CesiumAsync::AsyncSystem& asyncSystem,
                Cesium3DTilesSelection::TileLoadResult&& tileLoadResult,
                const glm::dmat4& transform,
                const std::any& renderOptions) override;

        virtual void *prepareInMainThread( Cesium3DTilesSelection::Tile &tile,
                                           void *pLoadThreadResult ) override;

        virtual void free(
            Cesium3DTilesSelection::Tile& tile,
            void* pLoadThreadResult,
            void* pMainThreadResult) noexcept override;

        virtual void* prepareRasterInLoadThread(
            CesiumGltf::ImageCesium& image,
            const std::any& rasterOverlayOptions) override;

        virtual void* prepareRasterInMainThread(
            CesiumRasterOverlays::RasterOverlayTile &rasterTile,
            void *pLoadThreadResult) override;

        virtual void freeRaster(
            const CesiumRasterOverlays::RasterOverlayTile& rasterTile,
            void* pLoadThreadResult,
            void* pMainThreadResult
        ) noexcept override;

        virtual void attachRasterInMainThread(
			const Cesium3DTilesSelection::Tile& tile,
			int32_t overlayTextureCoordinateID,
			const CesiumRasterOverlays::RasterOverlayTile& rasterTile,
			void* pMainThreadRendererResources,
            const glm::dvec2& translation,
            const glm::dvec2& scale
		) override;

        virtual void detachRasterInMainThread(
            const Cesium3DTilesSelection::Tile& tile,
            int32_t overlayTextureCoordinateID,
            const CesiumRasterOverlays::RasterOverlayTile& rasterTile,
            void* pMainThreadRendererResources
        ) noexcept override;

    private:
        Ref<GD3DTileset> _tileset;
        CesiumShaderProperties _shaderProperty;
    };

} // CesiumForGodot

#endif //GODOT_GODOTPREPARERENDERERRESOURCES_H
