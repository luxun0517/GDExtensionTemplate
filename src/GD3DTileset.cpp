//
// Created by Harris.Lu on 2024/1/28.
//

#include "GD3DTileset.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "Cesium3DTilesetLoadFailureDetails.h"
#include "constants.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include "GodotTilesetExternals.h"
#include "CameraManager.h"
#include <spdlog/spdlog.h>
#include "GodotPrepareRendererResources.h"

namespace CesiumForGodot {

	GD3DTileset::GD3DTileset() {

	}

	GD3DTileset::~GD3DTileset() {

	}

    void GD3DTileset::RecreateTileset()
    {
        this->DestroyTileset();
	}

    void GD3DTileset::DestroyTileset()
    {
        WARN_PRINT( "DestroyTileset" );
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
        logSelectionStats = logSelectionStats;
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
            SPDLOG_LOGGER_INFO(
                this->_pTileset->getExternals().pLogger,
                "{0}: Visited {1}, Culled Visited {2}, Rendered {3}, Culled {4}, Max "
                "Depth Visited {5}, Loading-Worker {6}, Loading-Main {7} "
                "Total Tiles Resident {8}, Frame {9}",
                this->get_name(), currentResult.tilesVisited,
                currentResult.culledTilesVisited, currentResult.tilesToRenderThisFrame.size(),
                currentResult.tilesCulled, currentResult.maxDepthVisited,
                currentResult.workerThreadTileLoadQueueLength,
                currentResult.mainThreadTileLoadQueueLength,
                this->_pTileset->getNumberOfTilesLoaded(), currentResult.frameNumber );
        }

        this->_lastUpdateResult = currentResult;
    }

    void GD3DTileset::Start()
	{

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

         if ( Engine::get_singleton()->is_editor_hint() )
         {
             WARN_PRINT( "Editor" );
         }

         if ( !this->_pTileset )
         {
             this->LoadTileset();
             if (!this->_pTileset )
			 {
				 return;
			 }
         }

         std::vector<ViewState> viewStates =
             CameraManager::getAllCameras( *this );

         const ViewUpdateResult &updateResult =
             this->_pTileset->updateView( viewStates, static_cast<float>(delta) );
         this->updateLastViewUpdateResultState( updateResult );

         for ( Tile *pTile : updateResult.tilesFadingOut )
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
                 //CesiumGltfGameObject *pCesiumGameObject =
                 //    static_cast<CesiumGltfGameObject *>( pRenderContent->getRenderResources() );
                 //if ( pCesiumGameObject && pCesiumGameObject->pGameObject )
                 //{
                 //    RS->instance_set_visible( *pCesiumGameObject->pGameObject, false );
                 //    //RS->free_rid( *pCesiumGameObject->pGameObject );
                 //    //pCesiumGameObject->pGameObject->SetActive( false );
                 //}
             }

         }

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
                uint8_t typeValue = (uint8_t)details.type;
                Cesium3DTilesetLoadFailureDetails godotDetails;
                //godotDetails.tileset = *this;
                //godotDetails.tileset = this;
                godotDetails.type = Cesium3DTilesetLoadType( typeValue );
                godotDetails.httpStatusCode = details.statusCode;
                godotDetails.message = details.message.c_str();

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
            WARN_PRINT( "FromCesiumIon" );
        }
        else
        {
            this->_pTileset = std::make_unique<Tileset>( 
                createTilesetExternals(this), 
                this->_url.utf8(),
                options
            );
        }

        // 添加 overlay
        WARN_PRINT( "TODO: Add any overlay components" );
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
        WARN_PRINT( "TODO: Add any tile excluder components" );
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

		ADD_GROUP( "Renderer", "render_" );
        ADD_PROPERTY( PropertyInfo( Variant::STRING, "URL", PROPERTY_HINT_TYPE_STRING ),
                      "set_url", "get_url" );

        ADD_GROUP( "Level of Detail", "level of detail" );
	}

	void GD3DTileset::_notification(int p_what) {
        switch ( p_what )
        {
            case NOTIFICATION_READY:
            {
                Start();
                WARN_PRINT( "NOTIFICATION_READY" );
                break;
            }
            case NOTIFICATION_PROCESS:
			{
                Update(get_process_delta_time());
				break;
			}
            case NOTIFICATION_ENTER_TREE:
			{
				WARN_PRINT( "NOTIFICATION_ENTER_TREE" );
				break;
			}
            default:
                break;
        }
	}

} // Cesium

///**
// * If running in the editor, recurses into the editor scene tree to find the editor cameras and
// * grabs the first one. The edited_scene_root is excluded in case the user already has a Camera3D in
// * their scene.
// */
//void Terrain3D::_grab_camera()
//{
//    if ( Engine::get_singleton()->is_editor_hint() )
//    {
//        EditorScript temp_editor_script;
//        EditorInterface *editor_interface = temp_editor_script.get_editor_interface();
//        TypedArray<Camera3D> cam_array = TypedArray<Camera3D>();
//        _find_cameras( editor_interface->get_editor_main_screen()->get_children(),
//                       editor_interface->get_edited_scene_root(), cam_array );
//        if ( !cam_array.is_empty() )
//        {
//            LOG( DEBUG, "Connecting to the first editor camera" );
//            _camera = Object::cast_to<Camera3D>( cam_array[0] );
//        }
//    }
//    else
//    {
//        LOG( DEBUG, "Connecting to the in-game viewport camera" );
//        _camera = get_viewport()->get_camera_3d();
//    }
//    if ( !_camera )
//    {
//        set_process( false );
//        LOG( ERROR, "Cannot find active camera. Stopping _process()" );
//    }
//}