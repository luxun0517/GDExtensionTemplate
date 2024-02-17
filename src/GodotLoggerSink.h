//
// Created by Harris.Lu on 2024/2/16.
//

#ifndef GDCESIUM_GODOTLOGGERSINK_H
#define GDCESIUM_GODOTLOGGERSINK_H

#include <spdlog/sinks/base_sink.h>
#include <mutex>
namespace CesiumForGodot {

    class GodotLoggerSink : public spdlog::sinks::base_sink<std::mutex> {
    protected:
        virtual void sink_it_(const spdlog::details::log_msg& msg) override;
        virtual void flush_() override;
    };

} // CesiumForGodot

#endif //GDCESIUM_GODOTLOGGERSINK_H
