// Copied from godot-cpp/test/src and modified.

#include "gdextension_interface.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

#include "GD3DTileset.h"
//#include "Example.h"
//#include "GDExtensionTemplate.h"
// #include "TestRendering.h"
#include "CesiumGeoreference.h"
#include "TestTilesetJson.h"
#include "CesiumIonServer.h"
#include <Cesium3DTilesContent/registerAllTileContentTypes.h>
#include <CesiumUtility/Tracing.h>
#include "GodotLoggerSink.h"

/// @file
/// Register our classes with Godot.
using namespace CesiumForGodot;
namespace
{
    /// @brief Called by Godot to let us register our classes with Godot.
    ///
    /// @param p_level the level being initialized by Godot
    ///
    /// @see GDExtensionInit
    void initializeExtension( godot::ModuleInitializationLevel p_level )
    {
        if ( p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE )
        {
            return;
        }
        Cesium3DTilesContent::registerAllTileContentTypes();
        std::shared_ptr<spdlog::logger> pLogger = spdlog::default_logger();
        pLogger->sinks() = {std::make_shared<GodotLoggerSink>()};
        CESIUM_TRACE_INIT(
         "cesium-trace-" +
         std::to_string(std::chrono::time_point_cast<std::chrono::microseconds>(
                            std::chrono::steady_clock::now())
                            .time_since_epoch()
                            .count()) +
         ".json");
        
        ClassDB::register_class<CesiumGeoreference>();
        // godot::ClassDB::register_class<TestRendering>();
        ClassDB::register_class<CesiumForGodot::GD3DTileset>();
        ClassDB::register_class<TestTilesetJson>();
        ClassDB::register_class<CesiumIonServer>();
        //        godot::ClassDB::register_class<ExampleRef>();
//        godot::ClassDB::register_class<ExampleMin>();
//        godot::ClassDB::register_class<Example>();
//        godot::ClassDB::register_class<ExampleVirtual>( true );
//        godot::ClassDB::register_abstract_class<ExampleAbstract>();
//
//        godot::ClassDB::register_class<GDExtensionTemplate>();
    }

    /// @brief Called by Godot to let us do any cleanup.
    ///
    /// @see GDExtensionInit
    void uninitializeExtension( godot::ModuleInitializationLevel p_level )
    {
        if ( p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE )
        {
            return;
        }
    }
}

extern "C"
{
    /// @brief This is the entry point for the shared library.
    ///
    /// @note The name of this function must match the "entry_symbol" in
    /// templates/template.*.gdextension.in
    ///
    /// @param p_get_proc_address the interface (need more info)
    /// @param p_library the library (need more info)
    /// @param r_initialization the intialization (need more info)
    ///
    /// @returns GDExtensionBool
    GDExtensionBool GDE_EXPORT GDCesiumInit(
        GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization )
    {
        {
            godot::GDExtensionBinding::InitObject init_obj( p_get_proc_address, p_library,
                                                            r_initialization );

            init_obj.register_initializer( initializeExtension );
            init_obj.register_terminator( uninitializeExtension );
            init_obj.set_minimum_library_initialization_level(
                godot::MODULE_INITIALIZATION_LEVEL_SCENE );

            return init_obj.init();
        }
    }
}
