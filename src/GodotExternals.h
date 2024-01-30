//
// Created by Harris.Lu on 2024/1/29.
//

#ifndef GDCESIUM_GODOTEXTERNALS_H
#define GDCESIUM_GODOTEXTERNALS_H

#include <memory>

namespace CesiumAsync {
    class AsyncSystem;
    class IAssetAccessor;
}

namespace CesiumForGodot {

    const std::shared_ptr<CesiumAsync::IAssetAccessor>& getAssetAccessor();

    const CesiumAsync::AsyncSystem& getAsyncSystem();
}


#endif //GDCESIUM_GODOTEXTERNALS_H
