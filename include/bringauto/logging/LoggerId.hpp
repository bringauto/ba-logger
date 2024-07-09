#pragma once


#include <bringauto/logging/LoggerVerbosity.hpp>
#include <bringauto/logging/LoggerSettings.hpp>

#include <functional>

namespace bringauto::logging {

class LoggerId {
public:
    int id   {0}; //TODO make char const array later

    void initLogger(const LoggerSettings &settings);

    void logImplementation(LoggerVerbosity verbosity, std::string message);

    void logImplementation(LoggerVerbosity verbosity, char const * message);

    void destroyLogger();

    
    /*LoggerID(int id_, 
    std::function<void(const LoggerSettings &settings)>initLogger_,
    std::function<void(const LoggerSettings &settings)>initLogger_,
    )*/
    //constexpr LoggerId(char id[8], const int constant) : id(id), constant(constant) {}
};
}