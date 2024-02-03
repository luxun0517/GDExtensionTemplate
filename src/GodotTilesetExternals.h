//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_GODOTTILESETEXTERNALS_H
#define CESIUM_GODOT_GODOTTILESETEXTERNALS_H

#include <Cesium3DTilesSelection/TilesetExternals.h>
#include "GD3DTileset.h"

namespace Cesium3DTilesSelection
{
    class TilesetExternals;
    class CreditSystem;
} // namespace Cesium3DTilesSelection

namespace CesiumAsync
{
    class AsyncSystem;
    class IAssetAccessor;
    class ITaskProcessor;
} // namespace CesiumAsync

namespace CesiumForGodot {
    const std::shared_ptr<CesiumAsync::IAssetAccessor> &getAssetAccessor();
    const std::shared_ptr<CesiumAsync::ITaskProcessor> &getTaskProcessor();
    CesiumAsync::AsyncSystem getAsyncSystem();

    Cesium3DTilesSelection::TilesetExternals createTilesetExternals(
        const Ref<GD3DTileset> &tileset );

} // CesiumForGodot

#endif //CESIUM_GODOT_GODOTTILESETEXTERNALS_H
