//
// Created by Harris.Lu on 2024/2/9.
//

#ifndef GDCESIUM_CESIUMIONSERVER_H
#define GDCESIUM_CESIUMIONSERVER_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;
namespace CesiumForGodot {

    class CesiumIonServer : public Resource {
        GDCLASS(CesiumIonServer, Resource);

    private:
        String _serverUrl = "https://ion.cesium.com";
        String _apiUrl = "https://api.cesium.com";
        float _oauth2ApplicationID = 381;
        String _defaultIonAccessTokenId = "9825fd6f-1403-406d-a8c5-e81a53eb546b";
        String _defaultIonAccessToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiI2OWYyOWUwMS03NzRkLTRkOWQtYWM4OC1kZTgyMzJkMmViOTAiLCJpZCI6NzI1LCJpYXQiOjE1MjU2NjMzMzN9.lAqYBgDLUTFYmpmc7x_5_Lpc5_Pf9DiJXhpkWBpy8h0";

    public:
        CesiumIonServer();
        ~CesiumIonServer();

        void setServerUrl(const String& url);
        String getServerUrl() const
        {
            return _serverUrl;
        }

        void setApiUrl(const String& url);
        String getApiUrl() const
        {
            return _apiUrl;
        }

        void setOauth2ApplicationID(float id);
        float getOauth2ApplicationID() const
        {
            return _oauth2ApplicationID;
        }

        void setDefaultIonAccessTokenId(const String& id);
        String getDefaultIonAccessTokenId() const
        {
            return _defaultIonAccessTokenId;
        }

        void setDefaultIonAccessToken(const String& token);
        String getDefaultIonAccessToken() const
        {
            return _defaultIonAccessToken;
        }
        
    protected:
        static void _bind_methods();
    };

} // CesiumForGodot

#endif //GDCESIUM_CESIUMIONSERVER_H
