#include <bringauto/logging/Logger.hpp>
#include <iostream>

void bringauto::logging::Logger::initLogger(const LoggerSettings &settings) {
}


template<typename T>
void bringauto::logging::Logger::logImplementation(Verbosity verbosity, T message) {

}

void bringauto::logging::Logger::destroyLogger() {

}

template<>
void bringauto::logging::Logger::logImplementation<std::string>(Verbosity verbosity, std::string message) {

}

template<>
void bringauto::logging::Logger::logImplementation<char const *>(Verbosity verbosity, char const *message) {

}



