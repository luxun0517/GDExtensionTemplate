//
// Created by Harris.Lu on 2024/1/28.
//

#ifndef GDCESIUM_GD3DTILESET_H
#define GDCESIUM_GD3DTILESET_H

#include <godot_cpp/classes/node3d.hpp>
#include "Cesium3DTilesSelection/Tileset.h"

using namespace godot;
namespace Cesium {

    class GD3DTileset : public Node3D {
        GDCLASS(GD3DTileset, Node3D);

    private:
        String _url;
        std::unique_ptr<Cesium3DTilesSelection::Tileset> _pTileset;

        void Start();

        void Update( double delta );

        void LoadTileset();

    public:
        GD3DTileset();
        ~GD3DTileset();

        void set_url(String url);
        String get_url(){ return _url; };

    protected:
        void _notification( int p_what );
		static void _bind_methods();

    };

} // Cesium

#endif //GDCESIUM_GD3DTILESET_H
