//
// Created by Harris.Lu on 2024/2/3.
//

#include "GodotTilesetExternals.h"
#include <godot_cpp/classes/os.hpp>
#include "CesiumRuntimeSettings.h"
#include <CesiumUtility/CreditSystem.h>
#include <CesiumAsync/CachingAssetAccessor.h>
#include <CesiumAsync/GunzipAssetAccessor.h>
#include <CesiumAsync/SqliteCache.h>
#include "GodotAssetAccessor.h"
#include "GodotExternals.h"
#include "GodotTaskProcessor.h"
#include "GodotPrepareRendererResources.h"

using namespace Cesium3DTilesSelection;
using namespace CesiumAsync;

using namespace godot;
namespace CesiumForGodot {

    // namespace
    // {
    //     std::shared_ptr<IAssetAccessor> pAccessor = nullptr;
    //     std::shared_ptr<ITaskProcessor> pTaskProcessor = nullptr;
    //     std::shared_ptr<CesiumUtility::CreditSystem> pCreditSystem = nullptr;
    //     std::optional<AsyncSystem> asyncSystem;
    //
    //     const std::shared_ptr<CesiumUtility::CreditSystem> &getCreditSystem(
    //         GD3DTileset *tileset )
    //     {
    //         //// Get the credit system associated with the tileset.
    //         //Cesium3DTilesetImpl &tilesetImpl = tileset.NativeImplementation();
    //         //CesiumForUnity::CesiumCreditSystem creditSystem = tilesetImpl.getCreditSystem();
    //         CesiumCreditSystem* creditSystem = tileset->getCreditSystem();
    //
    //         //// If the tileset does not already reference a credit system,
    //         //// get the default one.
    //         if ( creditSystem == nullptr )
    //         {
    //             creditSystem = CesiumCreditSystem::GetDefaultCreditSystem();
    //             tileset->setCreditSystem( creditSystem );
    //         }
    //
    //         //CesiumCreditSystemImpl &creditSystemImpl = creditSystem.NativeImplementation();
    //         pCreditSystem = creditSystem->GetExternalCreditSystem();
    //
    //         return pCreditSystem;
    //     }
    // }

	// const std::shared_ptr<IAssetAccessor> &getAssetAccessor()
 //    
 //    {
 //        if ( !pAccessor )
 //        {
 //            String tempPath = OS::get_singleton()->get_cache_dir();
 //            String cacheDBPath = tempPath + "/cesium-request-cache.sqlite";
 //
 //            uint32_t requestsPerCachePrune = CesiumRuntimeSettings::requestsPerCachePrune();
 //            uint64_t maxItems = CesiumRuntimeSettings::maxItems();
 //
 //            pAccessor = std::make_shared<GunzipAssetAccessor>(
 //                std::make_shared<CachingAssetAccessor>(
 //                    spdlog::default_logger(),
 //                    std::make_shared<GodotAssetAccessor>(),
 //                    std::make_shared<SqliteCache>( 
 //                        spdlog::default_logger(),
 //                        cacheDBPath.utf8().get_data(),
 //                        maxItems
 //                    ),
 //                    requestsPerCachePrune
 //                )
 //            );
 //        }
 //        return pAccessor;
 //    }

    // const std::shared_ptr<ITaskProcessor> &getTaskProcessor()
    // {
    //     if ( !pTaskProcessor )
    //     {
    //         pTaskProcessor = std::make_shared<GodotTaskProcessor>();
    //     }
    //     return pTaskProcessor;
    // }
    //
    // CesiumAsync::AsyncSystem getAsyncSystem()
    // {
    //     if ( !asyncSystem )
    //     {
    //         asyncSystem.emplace( getTaskProcessor() );
    //     }
    //     return *asyncSystem;
    // }

    Cesium3DTilesSelection::TilesetExternals createTilesetExternals(
        GD3DTileset *tileset )
    {
        return TilesetExternals{ 
            getAssetAccessor(tileset),
            std::make_shared<GodotPrepareRendererResources>( tileset ),
            getAsyncSystem(),
            nullptr,
            spdlog::default_logger() 
        };
    }

} // CesiumForGodot