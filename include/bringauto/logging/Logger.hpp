#pragma once

#include <bringauto/logging/Sink.hpp>
#include <bringauto/logging/LoggerId.hpp>
#include <bringauto/logging/LoggerImpl.hpp>
#include <bringauto/logging/LoggerVerbosity.hpp>
#include <bringauto/logging/LoggerSettings.hpp>
#include <bringauto/logging/Concepts.hpp>

#include <format>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <optional>
#include <iostream>



namespace bringauto::logging {

using Verbosity = LoggerVerbosity;


/**
 * This class handles logger creation and addition of sinks
 * Supported message types are: std::string, const char*
 * Call addSink() to add sinks and init() to initialize the logger. log with log() and log<Verbosity>() functions
 */
//template <LoggerId ID = {.id = "Default", .void}>
template <LoggerId ID, Logable K>
class Logger {
public:

	/**
	 * Add sink defined by T with given arguments, only specialized sinks can be created, base class Sink cannot be created. This method does not create logger, only prepares sink to be included with logger
	 * to create logger with added sinks call init() functions with global params. Sinks added after init() call will throw a runtime_error exception
	 * @tparam T Sink type template , type of sink to be created
	 * @tparam Args Argument pack template
	 * @param args templates that sink requires, to see what each sink need see their constructors
	 */
	template <class T, typename ...Args>
	static typename std::enable_if_t<std::is_base_of_v<Sink, T> && !std::is_same_v<Sink, T>, void>
	addSink(const typename T::Params &args = {}) {
		if(initialized_) {
			throw std::runtime_error("Cannot add sink to already initialized logger!");
		}
		sinks_.push_back(std::make_shared<T>(args));
	}

	/**
	 * Create logger with given params, if implementation does not support any of given params exception will be thrown.
	 * Sinks created using addSink() function BEFORE init call will be added to logger (init() function will be called on them)
	 * Init will throw runtime_error if init() is used more then once without calling first or if no sink was added
	 * @param params global logger settings
	 */
	static void init(const LoggerSettings &settings) {
		if(initialized_) {
			throw std::runtime_error("Logger already initialized, ignoring init() call.");
		}
		if(sinks_.empty()) {
			throw std::runtime_error("Trying to init logger without any sinks, please add sinks first.");
		}
		loggingImpl_.initLogger(settings);
		programName_ = settings.programName;
		for(const auto &sink: sinks_) {
			sink->init(programName_);
		}
		sinks_.clear();
		initialized_ = true;
	}

	/**
	 * Log message with set verbosity, if logger was not created using init() function exception is thrown
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args std::format arguments message pack
	 * @param verbosity see Verbosity enum for options
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 *
	 */
	template <typename T, typename... Args> requires (Formattable<Args> && ...)
	static constexpr void log(Verbosity verbosity, T message, Args... args) {
		if(!initialized_) {
			throw std::runtime_error("Logger was not initialize! Please call Logger::init() before log functions");
		}
		if(isSupportedType(message)) {
			loggingImpl_.logImplementation(verbosity, getFormattedString(message, args...), programName_);
			return;
		}
		loggingImpl_.logImplementation(Verbosity::Warning, "Unsupported message type", programName_);
	};

	/**
	 * Log debugging message, method will call log() with Debug verbosity
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args std::format arguments message pack
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 */
	template <typename T, typename... Args>
	static void logDebug(T message, Args... args) {
		log(Verbosity::Debug, message, args...);
	}

	/**
	 * Log info message, method will call log() with Info verbosity
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args std::format arguments message pack
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 */
	template <typename T, typename... Args>
	static void logInfo(T message, Args... args) {
		log(Verbosity::Info, message, args...);
	}

	/**
	 * Log warning message, method will call log() with Warning verbosity
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args std::format arguments message pack
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 */
	template <typename T, typename... Args>
	static void logWarning(T message, Args... args) {
		log(Verbosity::Warning, message, args...);
	}

	/**
	 * Log error message, method will call log() with Error verbosity
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args std::format arguments message pack
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 */
	template <typename T, typename... Args>
	static void logError(T message, Args... args) {
		log(Verbosity::Error, message, args...);
	}

	/**
	 * Log critical message, method will call log() with Critical verbosity
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args std::format arguments message pack
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 */
	template <typename T, typename... Args>
	static void logCritical(T message, Args... args) {
		log(Verbosity::Critical, message, args...);
	}

	/**
	 * Method will reset logger and sinks (even not created ones) if it was created by init() function. If logger was not initialized method will do nothing.
	 * After calling delete() new logger can be created (also new sinks have to be created)
	 */
	static void destroy() {
		loggingImpl_.destroyLogger();
		sinks_.clear();
		initialized_ = false;
	};

private:
	inline static std::vector<std::shared_ptr<Sink>> sinks_; ///static list of sinks that will be added to logger
	inline static std::string programName_ {};                ///name of program that will be used in logging
	inline static bool initialized_ { false };                  ///true if logger is initialized (able to log)
	inline static K loggingImpl_ {};


	/**
	 * Create logger prints warning if set setting is not suppor
//todoted
	 * @param settings global logger settings, if some settings that are set are not supported prints a warning message
	 */
	static void initLogger(const LoggerSettings &settings);

	/**
	 * Destroys logger, addSink() init() can be run again
	 */
	static void destroyLogger();


	/**
	 * This method takes input string (or other supported string type like const char*)
	 * and argument pack that contain all arguments needed to create final string that will be logged
	 * @tparam T supported message type
	 * @tparam Args argument pack according to the format string fmt
	 * @param message message in fmt format or simple string
	 * @param args arguments needed in message to form final string
	 * @return final string
	 */
	template <typename T, typename ...Args>
	static std::string getFormattedString(T message, Args ...args) {
		std::string formattedString;
		try {
			formattedString = std::vformat(message, std::make_format_args(args...)); 
		} catch(std::exception &e) {
			formattedString = "[Logger error] Wrong log format (" + std::string { e.what() } +
							  "), please use fmt formatting for logger. Message: \"" + message
							  + "\".";
		}

		return formattedString;
	};

	/**
	 * Method checks if type T is supported "string" type in this library, std::string and const char* are supported
	 * @tparam T type
	 * @param message
	 * @return true if type is supported
	 */
	template <typename T>
	static constexpr bool isSupportedType(T message) {
		return std::is_same_v<decltype(message), std::string> ||
			   std::is_same_v<decltype(message), const char *>;
	}

};

}
