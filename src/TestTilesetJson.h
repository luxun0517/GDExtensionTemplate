//
// Created by Harris.Lu on 2024/2/9.
//

#ifndef GDCESIUM_TESTTILESETJSON_H
#define GDCESIUM_TESTTILESETJSON_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;
namespace CesiumForGodot {

    class TestTilesetJson : public Node3D {
        GDCLASS(TestTilesetJson, Node3D);

    private:
        void __ready();
        
        void __update();
        
    public:
        TestTilesetJson();

    protected:
        void _notification( int p_what );
        static void _bind_methods();
    };

} // CesiumForGodot

#endif //GDCESIUM_TESTTILESETJSON_H
