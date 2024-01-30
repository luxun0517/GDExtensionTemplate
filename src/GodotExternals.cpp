//
// Created by Harris.Lu on 2024/1/29.
//

#include "GodotExternals.h"
#include <CesiumAsync/CachingAssetAccessor.h>
#include <CesiumAsync/GunzipAssetAccessor.h>
#include <CesiumAsync/SqliteCache.h>
#include "godot_cpp/classes/dir_access.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/project_settings.hpp"

using namespace CesiumAsync;
using namespace godot;
namespace {

    std::shared_ptr<IAssetAccessor> pAccessor = nullptr;
    std::optional<AsyncSystem> asyncSystem;

} // namespace

namespace CesiumForGodot {

    const std::shared_ptr<IAssetAccessor>& getAssetAccessor() {
        if(!pAccessor) {
            //OS::get_singleton()->get_cache_path()
            String cacheDBPath = ProjectSettings::get_singleton()->globalize_path("cesium-request-cache.sqlite");

        }
        return pAccessor;
    }


} // Cesium