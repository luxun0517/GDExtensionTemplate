//
// Created by Harris.Lu on 2024/2/9.
//

#include "CesiumIonServer.h"

namespace CesiumForGodot {

    CesiumIonServer::CesiumIonServer() {
    }

    CesiumIonServer::~CesiumIonServer() {
    }

    void CesiumIonServer::setServerUrl(const String& url) {
        _serverUrl = url;
    }

    void CesiumIonServer::setApiUrl(const String& url) {
        _apiUrl = url;
    }

    void CesiumIonServer::setOauth2ApplicationID(float id) {
        _oauth2ApplicationID = id;
    }

    void CesiumIonServer::setDefaultIonAccessTokenId(const String& id) {
        _defaultIonAccessTokenId = id;
    }

    void CesiumIonServer::setDefaultIonAccessToken(const String& token) {
        _defaultIonAccessToken = token;
    }

    void CesiumIonServer::_bind_methods()
    {
        ClassDB::bind_method( D_METHOD( "setServerUrl", "url"), &CesiumIonServer::setServerUrl);
        ClassDB::bind_method( D_METHOD( "getServerUrl" ), &CesiumIonServer::getServerUrl);

        ClassDB::bind_method( D_METHOD( "setApiUrl", "url" ), &CesiumIonServer::setApiUrl );
        ClassDB::bind_method( D_METHOD( "getApiUrl" ), &CesiumIonServer::getApiUrl );

        ClassDB::bind_method( D_METHOD( "setOauth2ApplicationID", "id" ), &CesiumIonServer::setOauth2ApplicationID );
        ClassDB::bind_method( D_METHOD( "getOauth2ApplicationID" ), &CesiumIonServer::getOauth2ApplicationID );

        ClassDB::bind_method( D_METHOD( "setDefaultIonAccessTokenId", "token_id" ), &CesiumIonServer::setDefaultIonAccessTokenId );
        ClassDB::bind_method( D_METHOD( "getDefaultIonAccessTokenId" ), &CesiumIonServer::getDefaultIonAccessTokenId );

        ClassDB::bind_method( D_METHOD( "setDefaultIonAccessToken", "token"), &CesiumIonServer::setDefaultIonAccessToken);
        ClassDB::bind_method( D_METHOD( "getDefaultIonAccessToken" ), &CesiumIonServer::getDefaultIonAccessToken );

        ADD_PROPERTY( PropertyInfo( Variant::STRING, "Server Url", PROPERTY_HINT_NONE ), "setServerUrl", "getServerUrl" );
        ADD_PROPERTY( PropertyInfo( Variant::STRING, "Api Url", PROPERTY_HINT_NONE ), "setApiUrl", "getApiUrl" );
        ADD_PROPERTY( PropertyInfo( Variant::FLOAT, "Oauth2 Application ID", PROPERTY_HINT_NONE ), "setOauth2ApplicationID", "getOauth2ApplicationID" );
        ADD_PROPERTY( PropertyInfo( Variant::STRING, "Default Ion Access Token ID", PROPERTY_HINT_NONE ), "setDefaultIonAccessTokenId", "getDefaultIonAccessTokenId" );
        ADD_PROPERTY( PropertyInfo( Variant::STRING, "Default Ion Access Token", PROPERTY_HINT_NONE ), "setDefaultIonAccessToken", "getDefaultIonAccessToken" );
    }

    
} // CesiumForGodot