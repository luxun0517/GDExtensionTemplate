//
// Created by Harris.Lu on 2024/1/28.
//

#include "GD3DTileset.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "Cesium3DTilesetLoadFailureDetails.h"
#include "constants.h"
#include <godot_cpp/classes/rendering_server.hpp>
// #include "GodotTilesetExternals.h"
#include "CameraManager.h"
#include <spdlog/spdlog.h>
#include "GodotPrepareRendererResources.h"
#include "GodotTilesetExternals.h"
#include "godot_cpp/classes/http_client.hpp"
#include "godot_cpp/classes/http_request.hpp"
#include "godot_cpp/classes/os.hpp"
#include <godot_cpp/variant/string_name.hpp>
#include "GodotAssetAccessor.h"
#include "GodotExternals.h"

namespace CesiumForGodot {

	GD3DTileset::GD3DTileset() {
	    pRequest = memnew(HTTPRequest);
	}

	GD3DTileset::~GD3DTileset() {

	}

    void GD3DTileset::request( const String& url )
    {
	    pRequest->request( url );
    }

    void GD3DTileset::downloadTilesetJson( int p_status, int p_code, const PackedStringArray &p_headers, const PackedByteArray &p_data )
    {
        for (int i = 0; i < callbacks.size(); ++i)
        {
            callbacks[i](p_data.get_string_from_utf8());
        }
	    // emit_signal("tileset_request_completed" , p_status, p_code, p_headers, p_data);
	    // WARN_PRINT( vformat( "status: %d code: %d data: %s", p_status, p_code, p_data.get_string_from_utf8() ) );
    }

    void GD3DTileset::loadCompletedCallback( const std::function<void(String &response)> &callback )
    {
	    callbacks.clear();
	    callbacks.push_back( callback );
    }

    void GD3DTileset::RecreateTileset()
    {
        this->DestroyTileset();
	}

    void GD3DTileset::DestroyTileset()
    {
        //// Remove any existing raster overlays
        //System::Array1<CesiumForUnity::CesiumRasterOverlay> overlays =
        //    tileset.gameObject().GetComponents<CesiumForUnity::CesiumRasterOverlay>();
        //for ( int32_t i = 0, len = overlays.Length(); i < len; ++i )
        //{
        //    CesiumForUnity::CesiumRasterOverlay overlay = overlays[i];
        //    overlay.RemoveFromTileset();
        //}
        this->_pTileset.reset();
	}
		
	/// <summary>
    /// 设置3dtileset url
    /// </summary>
    /// <param name="url"></param>
    void GD3DTileset::set_url( String url )
    {
        _url = url;
        this->RecreateTileset();
    }

    void GD3DTileset::set_maximumScreenSpaceError( float maximumScreenSpaceError )
    {
        _maximumScreenSpaceError = maximumScreenSpaceError;
        this->RecreateTileset();
    }

    void GD3DTileset::set_preloadAncestors( bool preloadAncestors )
	{
		_preloadAncestors = preloadAncestors;
		this->RecreateTileset();
	}

    void GD3DTileset::set_forbidHoles( bool forbidHoles )
    {
        _forbidHoles = forbidHoles;
		this->RecreateTileset();
    }

    void GD3DTileset::set_maximumSimultaneousTileLoads( uint32_t maximumSimultaneousTileLoads )
    {
        _maximumSimultaneousTileLoads = maximumSimultaneousTileLoads;
        this->RecreateTileset();
    }

    void GD3DTileset::set_maximumCachedBytes( long maximumCachedBytes )
	{
		_maximumCachedBytes = maximumCachedBytes;
		this->RecreateTileset();
	}

    void GD3DTileset::set_loadingDescendantLimit( uint32_t loadingDescendantLimit )
	{
		_loadingDescendantLimit = loadingDescendantLimit;
		this->RecreateTileset();
	}

    void GD3DTileset::set_enableFrustumCulling( bool enableFrustumCulling )
    {
        _enableFrustumCulling = enableFrustumCulling;
		this->RecreateTileset();
    }

    void GD3DTileset::set_enableFogCulling( bool enableFogCulling )
    {
        _enableFogCulling = enableFogCulling;
        this->RecreateTileset();
    }

    void GD3DTileset::set_enforceCulledScreenSpaceError(bool enforceCulledScreenSpaceError)
	{
		_enforceCulledScreenSpaceError = enforceCulledScreenSpaceError;
		this->RecreateTileset();
	}

    void GD3DTileset::set_culledScreenSpaceError( float culledScreenSpaceError )
	{
		_culledScreenSpaceError = culledScreenSpaceError;
		this->RecreateTileset();
	}

    void GD3DTileset::set_showCreditsOnScreen( bool showCreditsOnScreen )
	{
		_showCreditsOnScreen = showCreditsOnScreen;
        WARN_PRINT( "SetShowCreditsOnScreen" );
       /* this.SetShowCreditsOnScreen( this._showCreditsOnScreen );
        if ( Cesium3DTileset.OnSetShowCreditsOnScreen != null )
        {
            Cesium3DTileset.OnSetShowCreditsOnScreen();
        }*/
	}

    void GD3DTileset::set_generateSmoothNormals( bool generateSmoothNormals )
    {
        _generateSmoothNormals = generateSmoothNormals;
		this->RecreateTileset();
    }

    void GD3DTileset::setCreditSystem( CesiumCreditSystem *NewCreditSystem )
    {
        _creditSystem = NewCreditSystem;
    }

    void GD3DTileset::set_suspendUpdate( bool suspendUpdate )
	{
        _suspendUpdate = suspendUpdate;
	}

    void GD3DTileset::set_logSelectionStats( bool logSelectionStats )
	{
        _logSelectionStats = logSelectionStats;
	}

    void GD3DTileset::set_ionAssetID( int ionAssetID )
    {
        _ionAssetID = ionAssetID;
    }

    void GD3DTileset::set_ionAccessToken( String ionAccessToken )
    {
        _ionAccessToken = ionAccessToken;
    }

    void GD3DTileset::set_IonServer( const Ref<CesiumIonServer> &ion_server )
    {
	    if(_ion_server != ion_server)
	    {
	        _ion_server = ion_server;
	        this->RecreateTileset();
	    }
    }

    void GD3DTileset::updateLastViewUpdateResultState(
        const ViewUpdateResult &currentResult
    )
    {
        if ( !this->get_logSelectionStats() )
        {
            return;
        }

        const ViewUpdateResult &previousResult = this->_lastUpdateResult;
        if ( currentResult.tilesToRenderThisFrame.size() !=
                 previousResult.tilesToRenderThisFrame.size() ||
             currentResult.workerThreadTileLoadQueueLength !=
                 previousResult.workerThreadTileLoadQueueLength ||
             currentResult.mainThreadTileLoadQueueLength !=
                 previousResult.mainThreadTileLoadQueueLength ||
             currentResult.tilesVisited != previousResult.tilesVisited ||
             currentResult.culledTilesVisited != previousResult.culledTilesVisited ||
             currentResult.tilesCulled != previousResult.tilesCulled ||
             currentResult.maxDepthVisited != previousResult.maxDepthVisited )
        {
           /* WARN_PRINT( vformat( "%s: Visited %d, Culled Visited %d, Rendered %d, Culled %d, Max Depth Visited %d, Loading-Worker %d, Loading-Main %d Total Tiles Resident %d, Frame %d" ),
                this->get_name(), 
                currentResult.tilesVisited,
                currentResult.culledTilesVisited,
                currentResult.tilesToRenderThisFrame.size(), 
                currentResult.tilesCulled,
                currentResult.maxDepthVisited, 
                currentResult.workerThreadTileLoadQueueLength， 
                currentResult.mainThreadTileLoadQueueLength,
                this->_pTileset->getNumberOfTilesLoaded()，
                currentResult.frameNumber
            );*/
            WARN_PRINT( "updateLastViewUpdateResultState" );
            // SPDLOG_LOGGER_INFO(
            //     this->_pTileset->getExternals().pLogger,
            //     "{0}: Visited {1}, Culled Visited {2}, Rendered {3}, Culled {4}, Max "
            //     "Depth Visited {5}, Loading-Worker {6}, Loading-Main {7} "
            //     "Total Tiles Resident {8}, Frame {9}",
            //     this->get_name(), currentResult.tilesVisited,
            //     currentResult.culledTilesVisited, currentResult.tilesToRenderThisFrame.size(),
            //     currentResult.tilesCulled, currentResult.maxDepthVisited,
            //     currentResult.workerThreadTileLoadQueueLength,
            //     currentResult.mainThreadTileLoadQueueLength,
            //     this->_pTileset->getNumberOfTilesLoaded(), currentResult.frameNumber );
        }

        this->_lastUpdateResult = currentResult;
    }

    void GD3DTileset::Start()
	{
	    //执行Update方法
	    set_process(true);
	}

    void GD3DTileset::Update( double delta )
	{
       if ( this->get_suspendUpdate() )
       {
           return;
       }
       
        if ( this->_destroyTilesetOnNextUpdate )
        {
			this->_destroyTilesetOnNextUpdate = false;
            this->DestroyTileset();
        }

        if ( !this->_pTileset )
        {
            this->LoadTileset();
            if ( !this->_pTileset )
            {
                return;
            }
        }
    //
    //      if ( Engine::get_singleton()->is_editor_hint() )
    //      {
    //          WARN_PRINT( "Editor" );
    //      }

         std::vector<ViewState> viewStates =
             CameraManager::getAllCameras( *this );
         
         const ViewUpdateResult &updateResult =
             this->_pTileset->updateView( viewStates, static_cast<float>(delta) );
         this->updateLastViewUpdateResultState( updateResult );

         //
         // for ( Tile *pTile : updateResult.tilesFadingOut )
         // {
         //     if ( pTile->getState() != TileLoadState::Done )
         //     {
         //         continue;
         //     }
         //
         //     const Cesium3DTilesSelection::TileContent &content = pTile->getContent();
         //     const Cesium3DTilesSelection::TileRenderContent *pRenderContent =
         //         content.getRenderContent();
         //     if ( pRenderContent )
         //     {
         //         //CesiumGltfGameObject *pCesiumGameObject =
         //         //    static_cast<CesiumGltfGameObject *>( pRenderContent->getRenderResources() );
         //         //if ( pCesiumGameObject && pCesiumGameObject->pGameObject )
         //         //{
         //         //    RS->instance_set_visible( *pCesiumGameObject->pGameObject, false );
         //         //    //RS->free_rid( *pCesiumGameObject->pGameObject );
         //         //    //pCesiumGameObject->pGameObject->SetActive( false );
         //         //}
         //     }
         //
         // }
         //
	   
         for ( auto pTile : updateResult.tilesToRenderThisFrame )
         {
             if ( pTile->getState() != TileLoadState::Done )
             {
                 continue;
             }
             const Cesium3DTilesSelection::TileContent &content = pTile->getContent();
             const Cesium3DTilesSelection::TileRenderContent *pRenderContent =
                 content.getRenderContent();
             if ( pRenderContent )
             {
                 WARN_PRINT( "tilesToRenderThisFrame" );
                 //CesiumGltfGameObject *pCesiumGameObject =
                 //    static_cast<CesiumGltfGameObject *>( pRenderContent->getRenderResources() );
                 //if ( pCesiumGameObject && pCesiumGameObject->pGameObject )
                 //{
                 //    RS->instance_set_visible( *pCesiumGameObject->pGameObject, true );
                 //    //pCesiumGameObject->pGameObject->SetActive( true );
                 //}
             }
         }
	}

	void GD3DTileset::LoadTileset()
    {
        TilesetOptions options;
        options.maximumScreenSpaceError = this->_maximumScreenSpaceError;
        options.preloadAncestors = this->_preloadAncestors;
        options.preloadSiblings = this->_preloadSiblings;
        options.forbidHoles = this->_forbidHoles;
        options.maximumSimultaneousTileLoads = this->_maximumSimultaneousTileLoads;
        options.maximumCachedBytes = this->_maximumCachedBytes;
        options.loadingDescendantLimit = this->_loadingDescendantLimit;
        options.enableFrustumCulling = this->_enableFrustumCulling;
        options.enableFogCulling = this->_enableFogCulling;
        options.enforceCulledScreenSpaceError = this->_enforceCulledScreenSpaceError;
        options.culledScreenSpaceError = this->_culledScreenSpaceError;
        options.showCreditsOnScreen = this->_showCreditsOnScreen;
        options.loadErrorCallback =
            [this]( const TilesetLoadFailureDetails &details ) {
                // uint8_t typeValue = (uint8_t)details.type;
                // Cesium3DTilesetLoadFailureDetails godotDetails;
                // //godotDetails.tileset = *this;
                // //godotDetails.tileset = this;
                // godotDetails.type = Cesium3DTilesetLoadType( typeValue );
                // godotDetails.httpStatusCode = details.statusCode;
                // godotDetails.message = details.message.c_str();

			    WARN_PRINT( "LoadErrorCallback" );
		    };

        options.mainThreadLoadingTimeLimit = 5.0f;
        options.tileCacheUnloadTimeLimit = 5.0f;

        TilesetContentOptions contentOptions{};
        contentOptions.generateMissingNormalsSmooth = this->_generateSmoothNormals;

        CesiumGltf::SupportedGpuCompressedPixelFormats supportedFormats;
        supportedFormats.ETC2_RGBA = true;
        supportedFormats.ETC1_RGB = true;
        supportedFormats.BC1_RGB = true;
        supportedFormats.BC3_RGBA = true;
        supportedFormats.BC4_R = true;
        supportedFormats.BC5_RG = true;
        supportedFormats.BC7_RGBA = true;
        supportedFormats.ASTC_4x4_RGBA = true;
        supportedFormats.PVRTC1_4_RGB = true;
        supportedFormats.ETC2_EAC_R11 = true;
        supportedFormats.ETC2_EAC_RG11 = true;

        contentOptions.ktx2TranscodeTargets =
            CesiumGltf::Ktx2TranscodeTargets( supportedFormats, false );

        options.contentOptions = contentOptions;

        this->_lastUpdateResult = ViewUpdateResult();

        if ( TilesetSource == TilesetSource::FromCesiumIon )
        {
            String ionAccessToken = this->_ion_server->getDefaultIonAccessToken();
            std::string ionAssetEndpointUrl = this->_ion_server->getApiUrl().utf8().get_data();
            if (*ionAssetEndpointUrl.rbegin() != '/')
            {
                ionAssetEndpointUrl += '/';
            }

            this->_pTileset = std::make_unique<Tileset>(
                 createTilesetExternals(this),
                 this->get_ionAssetID(),
                 ionAccessToken.utf8().get_data(),
                 options,
                 ionAssetEndpointUrl);
        }
        else
        {
            this->_pTileset = std::make_unique<Tileset>( 
                createTilesetExternals(this), 
                this->_url.utf8().get_data(),
                options
            );
        }

        // 添加 overlay
        // WARN_PRINT( "TODO: Add any overlay components" );
      /*  System::Array1<CesiumForUnity::CesiumRasterOverlay> overlays =
            tileset.gameObject().GetComponents<CesiumForUnity::CesiumRasterOverlay>();
        for ( int32_t i = 0, len = overlays.Length(); i < len; ++i )
        {
            CesiumForUnity::CesiumRasterOverlay overlay = overlays[i];
            overlay.AddToTileset();
        }*/
        /*TypedArray<Node> descendants =
            this->find_children( "collider_view", "MeshInstance3D" );*/
        //this->find_children()
        //  Add any tile excluder components
        // WARN_PRINT( "TODO: Add any tile excluder components" );
     /*   System::Array1<CesiumForUnity::CesiumTileExcluder> excluders =
            tileset.gameObject().GetComponentsInParent<CesiumForUnity::CesiumTileExcluder>();
        for ( int32_t i = 0, len = excluders.Length(); i < len; ++i )
        {
            CesiumForUnity::CesiumTileExcluder excluder = excluders[i];
            if ( !excluder.enabled() )
            {
                continue;
            }

            excluder.AddToTileset( tileset );
        }*/
      
    }

	void GD3DTileset::_bind_methods() {
        ClassDB::bind_method( D_METHOD( "set_url", "url"), &GD3DTileset::set_url);
        ClassDB::bind_method( D_METHOD( "get_url" ), &GD3DTileset::get_url);

	    ClassDB::bind_method( D_METHOD( "set_IonServer", "ion_server" ), &GD3DTileset::set_IonServer );
	    ClassDB::bind_method( D_METHOD( "get_IonServer" ), &GD3DTileset::get_IonServer );

	    ClassDB::bind_method( D_METHOD( "set_ionAssetID", "ionAssetID" ), &GD3DTileset::set_ionAssetID );
	    ClassDB::bind_method( D_METHOD( "get_ionAssetID" ), &GD3DTileset::get_ionAssetID );

		// ADD_GROUP( "Renderer", "render_" );
        ADD_PROPERTY( PropertyInfo( Variant::STRING, "URL", PROPERTY_HINT_NONE ),
                "set_url", "get_url" );
	    ADD_PROPERTY( PropertyInfo(Variant::INT, "Ion Asset ID", PROPERTY_HINT_NONE ), "set_ionAssetID", "get_ionAssetID" );
	    ADD_PROPERTY( PropertyInfo(Variant::OBJECT, "Ion Server", PROPERTY_HINT_RESOURCE_TYPE, "CesiumIonServer"), "set_IonServer", "get_IonServer" );
  //       ADD_GROUP( "Level of Detail", "level of detail" );
	}

	void GD3DTileset::_notification(int p_what) {
        switch ( p_what )
        {
            case NOTIFICATION_READY:
            {
                Start();
                break;
            }
            case NOTIFICATION_PROCESS:
			{
                Update(get_process_delta_time());
				break;
			}
            case NOTIFICATION_ENTER_TREE:
			{
                add_child( pRequest );
                pRequest->connect( "request_completed", callable_mp(this, &GD3DTileset::downloadTilesetJson) );
				break;
			}
            default:
                break;
        }
	}

} // Cesium
