//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_CESIUMRUNTIMESETTINGS_H
#define CESIUM_GODOT_CESIUMRUNTIMESETTINGS_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

namespace CesiumForGodot {
    static uint32_t _requestsPerCachePrune = 10000;

    static uint64_t _maxItems = 4086;

    class CesiumRuntimeSettings : public Resource {
        GDCLASS(CesiumRuntimeSettings, Resource)

    public:
        static uint32_t requestsPerCachePrune()
        {
            return _requestsPerCachePrune;
        }

        static uint64_t maxItems()
		{
			return _maxItems;
		}

    private:

    protected:
        static void _bind_methods();
    };

} // CesiumForGodot

#endif //CESIUM_GODOT_CESIUMRUNTIMESETTINGS_H
