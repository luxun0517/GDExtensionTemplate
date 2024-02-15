//
// Created by Harris.Lu on 2024/2/9.
//

#include "TestTilesetJson.h"

#include "Cesium3DTiles/Tileset.h"
#include "Cesium3DTilesContent/GltfConverters.h"
#include "Cesium3DTilesSelection/TilesetExternals.h"
#include "CesiumAsync/IAssetResponse.h"
#include "Cesium3DTilesSelection/IPrepareRendererResources.h"
#include "Cesium3DTilesSelection/Tile.h"
#include "Cesium3DTilesSelection/Tileset.h"
#include "CesiumRasterOverlays/RasterOverlayTile.h"
#include "godot_cpp/classes/file_access.hpp"
#include <gsl/span>
#include <filesystem>
#include <fstream>

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

    class SimpleAssetAccessor : public CesiumAsync::IAssetAccessor {
    public:
        SimpleAssetAccessor(
            std::map<std::string, std::shared_ptr<SimpleAssetRequest>>&&
                mockCompletedRequests)
            : mockCompletedRequests{std::move(mockCompletedRequests)} {}

        virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
        get(const CesiumAsync::AsyncSystem& asyncSystem,
            const std::string& url,
            const std::vector<THeader>&) override {
            auto mockRequestIt = mockCompletedRequests.find(url);
            if (mockRequestIt != mockCompletedRequests.end()) {
                return asyncSystem.createResolvedFuture(
                    std::shared_ptr<CesiumAsync::IAssetRequest>(mockRequestIt->second));
            }

            return asyncSystem.createResolvedFuture(
                std::shared_ptr<CesiumAsync::IAssetRequest>(nullptr));
        }

        virtual CesiumAsync::Future<std::shared_ptr<CesiumAsync::IAssetRequest>>
        request(
            const CesiumAsync::AsyncSystem& asyncSystem,
            const std::string& /* verb */,
            const std::string& url,
            const std::vector<THeader>& headers,
            const gsl::span<const std::byte>&) override {
            return this->get(asyncSystem, url, headers);
        }

        virtual void tick() noexcept override {}

        std::map<std::string, std::shared_ptr<SimpleAssetRequest>>
            mockCompletedRequests;
    };

    class SimpleTaskProcessor : public CesiumAsync::ITaskProcessor {
    public:
        virtual void startTask(std::function<void()> f) override { f(); }
    };

    class SimplePrepareRendererResource
    : public Cesium3DTilesSelection::IPrepareRendererResources {
    public:
      std::atomic<size_t> totalAllocation{};

      struct AllocationResult {
        AllocationResult(std::atomic<size_t>& allocCount_)
            : allocCount{allocCount_} {
          ++allocCount;
        }

        ~AllocationResult() noexcept { --allocCount; }

        std::atomic<size_t>& allocCount;
      };

      ~SimplePrepareRendererResource() noexcept { }

      virtual CesiumAsync::Future<Cesium3DTilesSelection::TileLoadResultAndRenderResources>
      prepareInLoadThread(
          const CesiumAsync::AsyncSystem& asyncSystem,
          Cesium3DTilesSelection::TileLoadResult&& tileLoadResult,
          const glm::dmat4& /*transform*/,
          const std::any& /*rendererOptions*/) override {
          WARN_PRINT( "prepareInLoadThread" );
        return asyncSystem.createResolvedFuture( Cesium3DTilesSelection::TileLoadResultAndRenderResources{
            std::move(tileLoadResult),
            new AllocationResult{totalAllocation}});
      }

      virtual void* prepareInMainThread(
          Cesium3DTilesSelection::Tile& /*tile*/,
          void* pLoadThreadResult) override {
        if (pLoadThreadResult) {
          AllocationResult* loadThreadResult =
              reinterpret_cast<AllocationResult*>(pLoadThreadResult);
          delete loadThreadResult;
        }
          WARN_PRINT( "prepareInMainThread" );
        return new AllocationResult{totalAllocation};
      }

      virtual void free(
          Cesium3DTilesSelection::Tile& /*tile*/,
          void* pLoadThreadResult,
          void* pMainThreadResult) noexcept override {
        if (pMainThreadResult) {
          AllocationResult* mainThreadResult =
              reinterpret_cast<AllocationResult*>(pMainThreadResult);
          delete mainThreadResult;
        }

        if (pLoadThreadResult) {
          AllocationResult* loadThreadResult =
              reinterpret_cast<AllocationResult*>(pLoadThreadResult);
          delete loadThreadResult;
        }
      }

      virtual void* prepareRasterInLoadThread(
          CesiumGltf::ImageCesium& /*image*/,
          const std::any& /*rendererOptions*/) override {
        return new AllocationResult{totalAllocation};
      }

      virtual void* prepareRasterInMainThread(
          CesiumRasterOverlays::RasterOverlayTile& /*rasterTile*/,
          void* pLoadThreadResult) override {
        if (pLoadThreadResult) {
          AllocationResult* loadThreadResult =
              reinterpret_cast<AllocationResult*>(pLoadThreadResult);
          delete loadThreadResult;
        }

        return new AllocationResult{totalAllocation};
      }

      virtual void freeRaster(
          const CesiumRasterOverlays::RasterOverlayTile& /*rasterTile*/,
          void* pLoadThreadResult,
          void* pMainThreadResult) noexcept override {
        if (pMainThreadResult) {
          AllocationResult* mainThreadResult =
              reinterpret_cast<AllocationResult*>(pMainThreadResult);
          delete mainThreadResult;
        }

        if (pLoadThreadResult) {
          AllocationResult* loadThreadResult =
              reinterpret_cast<AllocationResult*>(pLoadThreadResult);
          delete loadThreadResult;
        }
      }

      virtual void attachRasterInMainThread(
          const Cesium3DTilesSelection::Tile& /*tile*/,
          int32_t /*overlayTextureCoordinateID*/,
          const CesiumRasterOverlays::RasterOverlayTile& /*rasterTile*/,
          void* /*pMainThreadRendererResources*/,
          const glm::dvec2& /*translation*/,
          const glm::dvec2& /*scale*/) override {}

      virtual void detachRasterInMainThread(
          const Cesium3DTilesSelection::Tile& /*tile*/,
          int32_t /*overlayTextureCoordinateID*/,
          const CesiumRasterOverlays::RasterOverlayTile& /*rasterTile*/,
          void* /*pMainThreadRendererResources*/) noexcept override {}
    };
}

namespace CesiumForGodot {

    TestTilesetJson::TestTilesetJson() {
    }

    std::vector<std::byte> readFile(const std::filesystem::path& fileName) {
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<std::byte> buffer(static_cast<size_t>(size));
        file.read(reinterpret_cast<char*>(buffer.data()), size);

        return buffer;
    }

    void initializeTileset( Cesium3DTilesSelection::Tileset& tileset) {
        // create a random view state so that we can able to load the tileset first
        const CesiumGeospatial::Ellipsoid& ellipsoid = CesiumGeospatial::Ellipsoid::WGS84;
        CesiumGeospatial::Cartographic viewPositionCartographic{
            CesiumUtility::Math::degreesToRadians(118.0),
            CesiumUtility::Math::degreesToRadians(32.0),
            200.0};
        CesiumGeospatial::Cartographic viewFocusCartographic{
            viewPositionCartographic.longitude + CesiumUtility::Math::degreesToRadians(0.5),
            viewPositionCartographic.latitude + CesiumUtility::Math::degreesToRadians(0.5),
            0.0};
        glm::dvec3 viewPosition =
            ellipsoid.cartographicToCartesian(viewPositionCartographic);
        glm::dvec3 viewFocus =
            ellipsoid.cartographicToCartesian(viewFocusCartographic);
        glm::dvec3 viewUp{0.0, 0.0, 1.0};
        glm::dvec2 viewPortSize{500.0, 500.0};
        double aspectRatio = viewPortSize.x / viewPortSize.y;
        double horizontalFieldOfView = CesiumUtility::Math::degreesToRadians(60.0);
        double verticalFieldOfView =
            std::atan(std::tan(horizontalFieldOfView * 0.5) / aspectRatio) * 2.0;
        Cesium3DTilesSelection::ViewState viewState = Cesium3DTilesSelection::ViewState::create(
            viewPosition,
            glm::normalize(viewFocus - viewPosition),
            viewUp,
            viewPortSize,
            horizontalFieldOfView,
            verticalFieldOfView);

        tileset.updateView({viewState});
    }

    void TestTilesetJson::__ready() {
        set_process( true );

        std::filesystem::path testDataPath = "C:/Users/10968/Desktop/WorkSpace/engine/cesium-godot/engine/GDExtensionTemplate/extern/cesium-native/Cesium3DTilesSelection/test/data";
        testDataPath = testDataPath / "ReplaceTileset";
        std::vector<std::string> files{
            "tileset.json",
            "parent.b3dm",
            "ll.b3dm",
            "lr.b3dm",
            "ul.b3dm",
            "ur.b3dm",
            "ll_ll.b3dm",
        };

        std::map<std::string, std::shared_ptr<SimpleAssetRequest>>
            mockCompletedRequests;
        for (const auto& file : files) {
            std::unique_ptr<SimpleAssetResponse> mockCompletedResponse =
                std::make_unique<SimpleAssetResponse>(
                    static_cast<uint16_t>(200),
                    "doesn't matter",
                    CesiumAsync::HttpHeaders{},
                    readFile(testDataPath / file));
            mockCompletedRequests.insert(
                {file,
                 std::make_shared<SimpleAssetRequest>(
                     "GET",
                     file,
                     CesiumAsync::HttpHeaders{},
                     std::move(mockCompletedResponse))});
        }
        
        std::shared_ptr<SimpleAssetAccessor> mockAssetAccessor =
            std::make_shared<SimpleAssetAccessor>(std::move(mockCompletedRequests));
        Cesium3DTilesSelection::TilesetExternals tilesetExternals{
            mockAssetAccessor,
            std::make_shared<SimplePrepareRendererResource>(),
            CesiumAsync::AsyncSystem(std::make_shared<SimpleTaskProcessor>()),
            nullptr};
        
        // create tileset and call updateView() to give it a chance to load
        Cesium3DTilesSelection::Tileset tileset(tilesetExternals, "tileset.json");
        initializeTileset(tileset);

        const Cesium3DTilesSelection::Tile* pTilesetJson = tileset.getRootTile();

        WARN_PRINT( vformat( "size %d", pTilesetJson->getChildren().size() ) );
        

        // String b3dmFile = "C:/Users/10968/Desktop/WorkSpace/engine/cesium-godot/project/resource/Tileset/parent.b3dm";
        //
        // // PackedByteArray data = FileAccess::get_file_as_bytes( b3dmFile );
        // std::vector<std::byte> data2 = readFile( b3dmFile.utf8().get_data() );
        //
        // gsl::span<const std::byte> responseData = gsl::span<const std::byte>(
        //      reinterpret_cast<const std::byte*>(data2.data()),
        //      data2.size());
        //
        // WARN_PRINT( vformat( "size %d", data2.size() ) );
        //
        // auto converter = Cesium3DTilesContent::GltfConverters::getConverterByMagic(responseData);
        //
        // if(converter)
        // {
        //     WARN_PRINT( "converter" );
        // } else
        // {
        //     WARN_PRINT( "not converter" );
        // }
        //
    }

    void TestTilesetJson::__update()
    {
        
    }

    void TestTilesetJson::_notification(int p_what) {
        switch (p_what) {
            case NOTIFICATION_READY: {
                __ready();
                break;
            } 
            case NOTIFICATION_PROCESS: {
                __update();
                break;
            } 
        }
    }

    void TestTilesetJson::_bind_methods() {

    }
    
} // CesiumForGodot