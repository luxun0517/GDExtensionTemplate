//
// Created by Harris.Lu on 2024/1/30.
//

#ifndef GDCESIUM_GODOTASSETACCESSOR_H
#define GDCESIUM_GODOTASSETACCESSOR_H

#include <CesiumAsync/IAssetAccessor.h>

namespace CesiumForGodot {

   class GodotAssetAccessor : public CesiumAsync::IAssetAccessor
   {
    public:
        GodotAssetAccessor();

        virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> get(
            const CesiumAsync::AsyncSystem &asyncSystem, const std::string &url,
            const std::vector<THeader> &headers = {} ) override;

        virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> request(
            const CesiumAsync::AsyncSystem &asyncSystem, const std::string &verb,
            const std::string &url,
            const std::vector<CesiumAsync::IAssetAccessor::THeader> &headers,
            const gsl::span<const std::byte> &contentPayload ) override;

        virtual void tick() noexcept override;

    private:
        CesiumAsync::HttpHeaders _cesiumRequestHeaders;
    };

} // Cesium

#endif //GDCESIUM_GODOTASSETACCESSOR_H
