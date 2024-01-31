//
// Created by Harris.Lu on 2024/1/31.
//

#ifndef GODOT_TESTRENDERING_H
#define GODOT_TESTRENDERING_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;
namespace CesiumForGodot {

    class TestRendering : public Node3D  {
        GDCLASS(TestRendering, Node3D);

    private:
        void __ready();

    public:
        TestRendering();

    protected:
        void _notification( int p_what );
        static void _bind_methods();
    };

} // CesiumForGodot

#endif //GODOT_TESTRENDERING_H
