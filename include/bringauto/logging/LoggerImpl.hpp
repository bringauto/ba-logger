#pragma once


#include <bringauto/logging/LoggerVerbosity.hpp>
#include <bringauto/logging/LoggerSettings.hpp>


namespace bringauto::logging {

class LoggerImpl {
    public:

    void initLogger(const LoggerSettings &settings);

    void logImplementation(LoggerVerbosity verbosity, std::string message, const std::string id);

    void logImplementation(LoggerVerbosity verbosity, char const * message, const std::string id);

    void destroyLogger();

};
}