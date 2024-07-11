#pragma once

#include <bringauto/logging/LoggerSettings.hpp>
#include <bringauto/logging/LoggerVerbosity.hpp>

#include <format>
#include <string>



namespace bringauto::logging {
/**
 * Concept ensures T meets the BasicFormatter requirements
 * @tparam T argument pack used for std::vformat
 */
template <typename T>
concept Formattable = requires(T t) {
	{ std::formatter<T, char> {} } -> std::same_as<std::formatter<T, char>>;
};

/**
 * Concept ensures T contains required functions
 * @tparam T loggerImpl
 */
template <typename T>
concept Logable = requires(
		T t,
		const LoggerSettings &settings,
		LoggerVerbosity verbosity,
		std::string message,
		char const *message1,
		const std::string programName) {
	t.initLogger(settings);
	t.logImplementation(verbosity, message, programName);
	t.logImplementation(verbosity, message1, programName);
	t.destroyLogger();
};
}