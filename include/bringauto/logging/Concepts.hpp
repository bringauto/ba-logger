#pragma once


#include <bringauto/logging/LoggerVerbosity.hpp>
#include <bringauto/logging/LoggerSettings.hpp>

#include <string>
#include <format>

namespace bringauto::logging {

template<typename T>
concept Formattable = requires(T t) {
	{ std::formatter<T, char>{} } -> std::same_as<std::formatter<T, char>>;
};

template<typename T>
concept Logable = requires(T t, 
							const LoggerSettings &settings,
							LoggerVerbosity verbosity,
							std::string message,
							char const * message1,
							const std::string programName) {
	t.initLogger(settings);
    t.logImplementation(verbosity, message, programName);
    t.logImplementation(verbosity, message1, programName);
    t.destroyLogger();
};
}