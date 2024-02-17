//
// Created by Harris.Lu on 2024/1/30.
//

#ifndef GDCESIUM_GODOTASSETACCESSOR_H
#define GDCESIUM_GODOTASSETACCESSOR_H

#include "GD3DTileset.h"

#include <CesiumAsync/AsyncSystem.h>
#include <CesiumAsync/IAssetAccessor.h>
#include <CesiumAsync/IAssetResponse.h>
#include <CesiumAsync/IAssetRequest.h>
#include <CesiumAsync/HttpHeaders.h>

using namespace CesiumAsync;
namespace
{
    class GodotAssetResponse : public IAssetResponse
    {
    public:
        GodotAssetResponse(uint16_t statusCode, PackedByteArray&& data)
        : _statusCode( statusCode ),
        _data( data ){}

        virtual uint16_t statusCode() const override { return _statusCode; }

        virtual std::string contentType() const override { return _contentType; }

        virtual const HttpHeaders& headers() const override { return _headers; }

        virtual gsl::span<const std::byte> data() const override {
            gsl::span<const std::byte> responseData = gsl::span<const std::byte>(
             reinterpret_cast<const std::byte*>(this->_data.ptr()),
             this->_data.size());
            return responseData;
            // return this->_data;
        }
    private:
        uint16_t _statusCode;
        std::string _contentType;
        HttpHeaders _headers;
        PackedByteArray _data;
        // std::vector<std::byte> _data;
    };

    class GodotAssetRequest : public IAssetRequest
    {
    public:
        GodotAssetRequest(
            const HttpHeaders& headers,
            const std::string &url,
            uint16_t statusCode,
            PackedByteArray data)
                :_headers( headers ),
                _url( url ),
                _response( statusCode, std::move(data) ){}

        virtual const std::string& method() const override { return _method; }

        virtual const std::string& url() const override { return _url; }

        virtual const HttpHeaders& headers() const override { return _headers; }

        virtual const IAssetResponse* response() const override { return &_response; }

    private:
        std::string _method = "GET";
        std::string _url;
        HttpHeaders _headers;
        GodotAssetResponse _response;
    };
}

namespace CesiumForGodot {

   class GodotAssetAccessor : public CesiumAsync::IAssetAccessor
   {
    public:
        GodotAssetAccessor(GD3DTileset* tileset);

        virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> get(
            const CesiumAsync::AsyncSystem &asyncSystem,
            const std::string &url,
            const std::vector<THeader> &headers = {} ) override;

        virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> request(
            const CesiumAsync::AsyncSystem &asyncSystem,
            const std::string &verb,
            const std::string &url,
            const std::vector<CesiumAsync::IAssetAccessor::THeader> &headers,
            const gsl::span<const std::byte> &contentPayload ) override;
        
        virtual void tick() noexcept override;

    private:
       CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> getFromFile(
         const CesiumAsync::AsyncSystem& asyncSystem,
         const std::string& url,
         const std::vector<CesiumAsync::IAssetAccessor::THeader>& headers);
       
        CesiumAsync::HttpHeaders _cesiumRequestHeaders;
        GD3DTileset* _tileset;
    };

} // Cesium

#endif //GDCESIUM_GODOTASSETACCESSOR_H
