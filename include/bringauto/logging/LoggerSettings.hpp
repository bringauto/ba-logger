#pragma once

#include <bringauto/logging/LoggerVerbosity.hpp>

#include <optional>
#include <string>



namespace bringauto::logging {
/**
 * Structure containing global logger parameters
 */
struct LoggerSettings {
	/**
	 * Constructor, logger does need name and default verbosity in order to work correctly
	 */
	LoggerSettings(const std::string &nameOfLogger, LoggerVerbosity defaultVerbosity): loggerName(
																								nameOfLogger),
																						verbosity(defaultVerbosity) {};
	const std::string loggerName {};  ///name of logger that will be used in output
	const LoggerVerbosity verbosity {};///default verbosity for all sinks (lower bound), if set to Warning, only Warning, Error and Critical will be logged

	/**
	 * Structure defining log filters
	 */
	struct Filter {
		bool filterRepeatingMessages {
				false};                                 /// if true repeating messages will be filtered based on repeatingMessageIntervalInMS
		std::optional<int> repeatingMessageIntervalInMS;///if repeating messages filter is set, repeating message will be able to log every repeatingMessageIntervalInMS milliseconds, rest of them will be discarded
	};
	std::optional<Filter> filter;        ///optional filters
	std::optional<std::string> logFormat;///log format see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
};
}