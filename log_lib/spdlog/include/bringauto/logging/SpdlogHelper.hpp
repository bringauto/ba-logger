#pragma once

#include <bringauto/logging/Logger.hpp>

#include <spdlog/spdlog.h>

namespace bringauto::logging{
    class SpdlogHelper {
    public:
        static spdlog::level::level_enum mapToSpdlogVerbosity(Logger::Verbosity verbosity);
    };

}