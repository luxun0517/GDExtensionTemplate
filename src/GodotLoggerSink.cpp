//
// Created by Harris.Lu on 2024/2/16.
//

#include "GodotLoggerSink.h"

#include "godot_cpp/core/error_macros.hpp"

namespace CesiumForGodot {

    void GodotLoggerSink::sink_it_(const spdlog::details::log_msg& msg) {
        spdlog::memory_buf_t formatted;
        this->formatter_->format(msg, formatted);
        WARN_PRINT( fmt::to_string(formatted).c_str() );
//        UnityEngine::Debug::Log(System::String(fmt::to_string(formatted)));
    }

    void GodotLoggerSink::flush_() {}


} // CesiumForGodot