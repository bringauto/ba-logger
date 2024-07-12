#pragma once

#include <bringauto/logging/LoggerSettings.hpp>
#include <bringauto/logging/LoggerVerbosity.hpp>



namespace bringauto::logging {
/**
 * Class contains functions that differ for various logger implementations
 */
class LoggerImpl {
public:
	/**
	 * Create logger prints warning if set setting is not suppor
//todoted
	 * @param settings global logger settings, if some settings that are set are not supported prints a warning message
	 */
	void initLogger(const LoggerSettings &settings);

	/**
	 * Message logging, method will log message to logger and/or all sinks
	 * @param verbosity verbosity lvl
	 * @param message message to log
	 * @param loggerName logger identifier
	 */
	void logImplementation(LoggerVerbosity verbosity, std::string message, const std::string &loggerName);

	/**
	 * Message logging, method will log message to logger and/or all sinks
	 * @param verbosity verbosity lvl
	 * @param message message to log
	 * @param loggerName logger identifier
	 */

	void logImplementation(LoggerVerbosity verbosity, char const *message, const std::string &loggerName);

	/**
	 * Destroys logger, addSink() init() can be run again
	 */
	void destroyLogger();
};
}