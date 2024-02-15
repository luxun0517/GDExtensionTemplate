//
// Created by Harris.Lu on 2024/1/30.
//
#pragma warning(disable : 4189)
#include "GodotAssetAccessor.h"
#include "Cesium.h"
#include "Cesium3DTilesContent/GltfConverters.h"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/http_client.hpp"
#include "godot_cpp/classes/http_request.hpp"

#include <CesiumAsync/AsyncSystem.h>
#include <CesiumAsync/IAssetResponse.h>
#include <CesiumAsync/IAssetRequest.h>
#include <CesiumAsync/HttpHeaders.h>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/marshalls.hpp>
#include <uriparser/Uri.h>

using namespace CesiumAsync;

using namespace godot;

namespace 
{
    class SimpleAssetResponse : public CesiumAsync::IAssetResponse {
    public:
        SimpleAssetResponse(
            uint16_t statusCode,
            const std::string& contentType,
            const CesiumAsync::HttpHeaders& headers,
            const std::vector<std::byte>& data)
            : mockStatusCode{statusCode},
              mockContentType{contentType},
              mockHeaders{headers},
              mockData{data} {}

        virtual uint16_t statusCode() const override { return this->mockStatusCode; }

        virtual std::string contentType() const override {
            return this->mockContentType;
        }

        virtual const CesiumAsync::HttpHeaders& headers() const override {
            return this->mockHeaders;
        }

        virtual gsl::span<const std::byte> data() const override {
            return gsl::span<const std::byte>(mockData.data(), mockData.size());
        }

        uint16_t mockStatusCode;
        std::string mockContentType;
        CesiumAsync::HttpHeaders mockHeaders;
        std::vector<std::byte> mockData;
    };
    
    class SimpleAssetRequest : public CesiumAsync::IAssetRequest {
    public:
        SimpleAssetRequest(
            const std::string& method_,
            const std::string& url_,
            const CesiumAsync::HttpHeaders& headers_,
            std::unique_ptr<SimpleAssetResponse> pResponse_)
            : requestMethod{method_},
              requestUrl{url_},
              requestHeaders{headers_},
              pResponse{std::move(pResponse_)} {}

        virtual const std::string& method() const override {
            return this->requestMethod;
        }

        virtual const std::string& url() const override { return this->requestUrl; }

        virtual const CesiumAsync::HttpHeaders& headers() const override {
            return this->requestHeaders;
        }

        virtual const CesiumAsync::IAssetResponse* response() const override {
            return this->pResponse.get();
        }

        std::string requestMethod;
        std::string requestUrl;
        CesiumAsync::HttpHeaders requestHeaders;
        std::unique_ptr<SimpleAssetResponse> pResponse;
    };
}

namespace
{
    class GodotAssetResponse : public CesiumAsync::IAssetResponse
    {
    };

    /**
     * \brief read file from local file system
     */
    class GodotFileAssetRequestResponse : public CesiumAsync::IAssetRequest,
                                          public CesiumAsync::IAssetResponse
    {
    public:
        GodotFileAssetRequestResponse(
            std::string&& url,
            uint16_t statusCode,
            PackedByteArray&& data
        ): _url( std::move( url ) ), _statusCode( statusCode ), _data( data ) {}

        virtual const std::string& method() const { return getMethod; }

        virtual const std::string& url() const { return _url; }

        virtual const CesiumAsync::HttpHeaders& headers() const override { return emptyHeaders; }

        virtual const CesiumAsync::IAssetResponse* response() const override { return this; }

        virtual uint16_t statusCode() const override { return _statusCode; }

        virtual std::string contentType() const override { return std::string(); }

        virtual gsl::span<const std::byte> data() const override
        {
            gsl::span<const std::byte> responseData = gsl::span<const std::byte>(
                reinterpret_cast<const std::byte*>(this->_data.ptr()),
                this->_data.size());
            return responseData;
        }

    private:
        static const std::string getMethod;
        static const CesiumAsync::HttpHeaders emptyHeaders;
        
        std::string _url;
        uint16_t _statusCode;
        PackedByteArray _data;
    };

    const std::string GodotFileAssetRequestResponse::getMethod = "GET";
    const CesiumAsync::HttpHeaders GodotFileAssetRequestResponse::emptyHeaders{};
    
}

namespace
{
    std::string convertFileUriToFilename(const std::string& url)
    {
        // According to the uriparser docs, both uriUriStringToWindowsFilenameA and
        // uriUriStringToUnixFilenameA require an output buffer with space for at most
        // length(url)+1 characters.
        // https://uriparser.github.io/doc/api/latest/Uri_8h.html#a4afbc8453c7013b9618259bc57d81a39
        std::string result(url.size() + 1, '\0');

#ifdef _WIN32
        int errorCode = uriUriStringToWindowsFilenameA(url.c_str(), result.data());
#else
        int errorCode = uriUriStringToUnixFilenameA(url.c_str(), result.data());
#endif

        // Truncate the string if necessary by finding the first null character.
        size_t end = result.find('\0');
        if (end != std::string::npos) {
            result.resize(end);
        }

        // Remove query parameters from the URL if present, as they are no longer
        // ignored by Unreal.
        size_t pos = result.find("?");
        if (pos != std::string::npos) {
            result.erase(pos);
        }

        return result;
    }
}

namespace CesiumForGodot {

	GodotAssetAccessor::GodotAssetAccessor(GD3DTileset* tileset) : _cesiumRequestHeaders() {
	    this->_tileset = tileset;
        std::string version = Cesium::version + " " + Cesium::commit;

        String projectName = ProjectSettings::get_singleton()->get_setting("application/config/name");
        String engine =
            String("Godot Engine ") + String(ProjectSettings::get_singleton()->get_setting( "application/config/version" ));
        //Project Name : cesium - godot, Engine:
        String osVersion = OS::get_singleton()->get_name();
        
        this->_cesiumRequestHeaders.insert( { "X-Cesium-Client", "Cesium For Godot" } );
        this->_cesiumRequestHeaders.insert( { "X-Cesium-Client-Version", version } );
        this->_cesiumRequestHeaders.insert(
            { "X-Cesium-Client-Project", projectName.utf8().get_data() } );
        this->_cesiumRequestHeaders.insert(
            { "X-Cesium-Client-Engine", engine.utf8().get_data() } );
        this->_cesiumRequestHeaders.insert( { "X-Cesium-Client-OS", osVersion.utf8().get_data() } );
	}

    namespace
    {
	    const char fileProtocol[] = "file:///";
	    bool isFile(const std::string& url)
	    {
	        return url.compare(0, sizeof(fileProtocol) - 1, fileProtocol) == 0;
	    }
        
    }

    CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> 
        GodotAssetAccessor::get(
            const CesiumAsync::AsyncSystem& asyncSystem,
            const std::string& url,
            const std::vector<THeader>& headers) {

	    if(isFile( url ))
	    {
	        return getFromFile(asyncSystem, url, headers);
	    }

	    HttpHeaders cesiumRequestHeaders = this->_cesiumRequestHeaders;
	
	    return asyncSystem.createFuture<std::shared_ptr<CesiumAsync::IAssetRequest>>(
	        [&url, &headers, &cesiumRequestHeaders, &tileset = this->_tileset](const auto& promise) {
	        // Ref<HTTPClient> client = memnew( HTTPClient );
	        // client->connect_to_host( url.c_str() );
         //
	        HttpHeaders requestHeaders = cesiumRequestHeaders;
	        PackedStringArray _headers;
	        for (const auto& header : headers)
	        {
	            requestHeaders.insert( header );
	        }
	        for (const auto& header : requestHeaders) {
	            _headers.push_back( vformat( "%s : %s", header.first.c_str(), header.second.c_str() ) );
            }

	        tileset->request( url.c_str() );

	        // Error err = client->request( HTTPClient::METHOD_GET, url.c_str(), _headers );
	        // if(err != OK)
	        // {
	        //     WARN_PRINT( "request failed" );
	        // } else {
         //        WARN_PRINT( "request successed" );	            
	        // }
	        
	    } );

	    // WARN_PRINT( "file is url" );
	    // auto pMockCompletedResponse = std::make_unique<SimpleAssetResponse>(
     //      static_cast<uint16_t>(200),
     //      "doesn't matter",
     //      CesiumAsync::HttpHeaders{},
     //      std::move(std::vector<std::byte>()));
     //
	    // auto pMockCompletedRequest = std::make_shared<SimpleAssetRequest>(
     //        "GET",
     //        "tileset.json",
     //        CesiumAsync::HttpHeaders{},
     //        std::move(pMockCompletedResponse));
	    //
	    // return asyncSystem.createResolvedFuture(std::shared_ptr<CesiumAsync::IAssetRequest>(pMockCompletedRequest));
	}

    CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
        GodotAssetAccessor::getFromFile(
        const CesiumAsync::AsyncSystem& asyncSystem,
        const std::string& url,
        const std::vector<CesiumAsync::IAssetAccessor::THeader>& headers)
	{
	    return asyncSystem.createFuture<std::shared_ptr<CesiumAsync::IAssetRequest>>(
	        [&url](const auto& promise) {

	        std::string filename = convertFileUriToFilename(url);
            std::string path = url;

	        PackedByteArray data = FileAccess::get_file_as_bytes(filename.c_str());

	        promise.resolve(std::make_shared<GodotFileAssetRequestResponse>(
                std::move( path ),
                200,
                std::move(data)
            ));
	    });  
	}

    CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>> GodotAssetAccessor::request(
        const CesiumAsync::AsyncSystem &asyncSystem,
        const std::string &verb,
        const std::string &url,
        const std::vector<CesiumAsync::IAssetAccessor::THeader> &headers,
        const gsl::span<const std::byte> &contentPayload
    )
	{
	    WARN_PRINT( "request" );
        return asyncSystem.createFuture<std::shared_ptr<CesiumAsync::IAssetRequest>>(
             [&verb,
              &url,
              &headers](const auto& promise) {
                 
                return std::shared_ptr<CesiumAsync::IAssetRequest>();
            }
        );
	}
 
    void GodotAssetAccessor::tick() noexcept {}

} // Cesium