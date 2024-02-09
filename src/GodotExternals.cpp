//
// Created by Harris.Lu on 2024/1/29.
//

#include "GodotExternals.h"

#include "CesiumRuntimeSettings.h"

#include <Cesium3DTilesContent/registerAllTileContentTypes.h>
#include <CesiumAsync/CachingAssetAccessor.h>
#include <CesiumAsync/GunzipAssetAccessor.h>
#include <CesiumAsync/SqliteCache.h>
#include <CesiumAsync/AsyncSystem.h>
#include <CesiumAsync/IAssetAccessor.h>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include "GodotAssetAccessor.h"
#include "GodotTaskProcessor.h"

#include <spdlog/spdlog.h>

using namespace CesiumAsync;
using namespace godot;
// namespace {
//
//     std::shared_ptr<IAssetAccessor> pAccessor = nullptr;
//     std::optional<AsyncSystem> asyncSystem;
//
// } // namespace

namespace CesiumForGodot {

    std::string getCacheDatabaseName()
    {
        //     //OS::get_singleton()->get_cache_path()
        String cacheDBPath = ProjectSettings::get_singleton()->globalize_path("cesium-request-cache.sqlite");

        return cacheDBPath.utf8().get_data();
    }

    const std::shared_ptr<IAssetAccessor>& getAssetAccessor() {
        uint32_t RequestsPerCachePrune = CesiumRuntimeSettings::requestsPerCachePrune();
        static std::shared_ptr<CesiumAsync::IAssetAccessor> pAssetAccessor =
            std::make_shared<CesiumAsync::GunzipAssetAccessor>(
                std::make_shared<CesiumAsync::CachingAssetAccessor>(
                  spdlog::default_logger(),
                  std::make_shared<GodotAssetAccessor>(),
                  getCacheDatabase(),
                  RequestsPerCachePrune));
        return pAssetAccessor;
        // if(!pAccessor) {
        //     //OS::get_singleton()->get_cache_path()
        //     String cacheDBPath = ProjectSettings::get_singleton()->globalize_path("cesium-request-cache.sqlite");
        //
        // }
        // return pAccessor;
    }

    std::shared_ptr<CesiumAsync::ICacheDatabase>& getCacheDatabase()
    {
        uint64_t MaxCacheItems = CesiumRuntimeSettings::maxItems();

        static std::shared_ptr<CesiumAsync::ICacheDatabase> pCacheDatabase =
            std::make_shared<CesiumAsync::SqliteCache>(
                spdlog::default_logger(),
                getCacheDatabaseName(),
                MaxCacheItems);

        return pCacheDatabase;
    }

    CesiumAsync::AsyncSystem& getAsyncSystem() noexcept {
        static CesiumAsync::AsyncSystem asyncSystem(
            std::make_shared<GodotTaskProcessor>());
        return asyncSystem;
    }


} // Cesium