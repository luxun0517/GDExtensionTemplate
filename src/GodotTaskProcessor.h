//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_GODOTTASKPROCESSOR_H
#define CESIUM_GODOT_GODOTTASKPROCESSOR_H

#include <CesiumAsync/ITaskProcessor.h>

namespace CesiumForGodot {

    class GodotTaskProcessor : public CesiumAsync::ITaskProcessor {
    public:
        virtual void startTask( std::function<void()> f ) override;
    };

} // CesiumForGodot

#endif //CESIUM_GODOT_GODOTTASKPROCESSOR_H
