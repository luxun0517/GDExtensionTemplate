//
// Created by Harris.Lu on 2024/1/28.
//

#ifndef GDCESIUM_GD3DTILESET_H
#define GDCESIUM_GD3DTILESET_H

#include <godot_cpp/classes/node3d.hpp>
#include <Cesium3DTilesSelection/Tileset.h>
#include <Cesium3DTilesSelection/ViewUpdateResult.h>
#include "CesiumCreditSystem.h"
#include "Cesium3DTilesetLoadFailureDetails.h"

using namespace Cesium3DTilesSelection;
using namespace godot;
namespace CesiumForGodot {

    enum class TilesetSource : uint8_t
    {
        FromCesiumIon,
        FromUrl
    };

    class GD3DTileset : public Node3D {
        GDCLASS(GD3DTileset, Node3D);

    private:
        String _url;

        float _maximumScreenSpaceError = 16.0;
        //预加载父节点
        bool _preloadAncestors = true;

        bool _preloadSiblings = true;

        bool _forbidHoles = false;

        uint32_t _maximumSimultaneousTileLoads = 20;

        long _maximumCachedBytes = 512 * 1024 * 1024;

        uint32_t _loadingDescendantLimit = 20;

        bool _enableFrustumCulling = true;

        bool _enableFogCulling = true;

        bool _enforceCulledScreenSpaceError = true;

        float _culledScreenSpaceError = 64.0f;

        bool _showCreditsOnScreen = false;

        bool _generateSmoothNormals = false;
        // Pauses level-of-detail and culling updates of this tileset.
        bool _suspendUpdate = false;

        bool _destroyTilesetOnNextUpdate;

        bool _logSelectionStats = false;

        std::unique_ptr<Cesium3DTilesSelection::Tileset> _pTileset;
        Cesium3DTilesSelection::ViewUpdateResult _lastUpdateResult;

        CesiumCreditSystem* _creditSystem;

        TilesetSource TilesetSource = TilesetSource::FromUrl;

        void Start();

        void Update( double delta );

        void LoadTileset();

        void updateLastViewUpdateResultState( 
            const Cesium3DTilesSelection::ViewUpdateResult& currentResult
        );

    public:
        GD3DTileset();
        ~GD3DTileset();

        void RecreateTileset();

        void set_url(String url);
        String get_url(){ 
            return _url;
        }

        CesiumCreditSystem* getCreditSystem() const
        {
            return this->_creditSystem;
        };
        void setCreditSystem( CesiumCreditSystem * NewCreditSystem );

        void set_maximumScreenSpaceError( float maximumScreenSpaceError );
        float get_maximumScreenSpaceError()
        {
            return _maximumScreenSpaceError;
        }

        void set_preloadAncestors( bool preloadAncestors );
        bool get_preloadAncestors()
		{
			return _preloadAncestors;
		}

        void set_forbidHoles( bool forbidHoles );
        bool get_forbidHoles()
        {
            return _forbidHoles;
        }

        void set_maximumSimultaneousTileLoads( uint32_t maximumSimultaneousTileLoads );
        uint32_t get_maximumSimultaneousTileLoads()
		{
			return _maximumSimultaneousTileLoads;
		}

        void set_maximumCachedBytes( long maximumCachedBytes );
        long get_maximumCachedBytes()
        {
            return _maximumCachedBytes;
        }

        void set_loadingDescendantLimit( uint32_t loadingDescendantLimit );
        uint32_t get_loadingDescendantLimit()
		{
			return _loadingDescendantLimit;
		}

        void set_enableFrustumCulling( bool enableFrustumCulling );
        bool get_enableFrustumCulling()
		{
			return _enableFrustumCulling;
		}

        void set_enableFogCulling( bool enableFogCulling );
        bool get_enableFogCulling()
        {
        	return _enableFogCulling;
		}

        void set_enforceCulledScreenSpaceError( bool enforceCulledScreenSpaceError );
        bool get_enforceCulledScreenSpaceError()
		{
			return _enforceCulledScreenSpaceError;
		}

        void set_culledScreenSpaceError( float culledScreenSpaceError );
        float get_culledScreenSpaceError()
        {
            return _culledScreenSpaceError;
        }

        void set_showCreditsOnScreen( bool showCreditsOnScreen );
        bool get_showCreditsOnScreen()
		{
			return _showCreditsOnScreen;
		}

        void set_generateSmoothNormals( bool generateSmoothNormals );
		bool get_generateSmoothNormals()
		{
			return _generateSmoothNormals;
		}

		void set_suspendUpdate( bool suspendUpdate );
        bool get_suspendUpdate()
        {
            return _suspendUpdate;
        }

        void set_logSelectionStats( bool logSelectionStats );
        bool get_logSelectionStats()
		{
			return _logSelectionStats;
		}

    protected:
        void DestroyTileset();

        void _notification( int p_what );
		static void _bind_methods();

    };

} // Cesium

#endif //GDCESIUM_GD3DTILESET_H
