#pragma once


#include <bringauto/logging/LoggerVerbosity.hpp>
#include <bringauto/logging/LoggerSettings.hpp>

#include <functional>

namespace bringauto::logging {

struct LoggerId {
    int id   {0}; //TODO make char const array later
};
}