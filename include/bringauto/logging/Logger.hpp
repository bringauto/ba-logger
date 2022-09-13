#pragma once
#define FMT_HEADER_ONLY ///allows use of headers only fmt formatting library included in this library, will be switched for std::format in the future

#include <bringauto/logging/Sink.hpp>

#include <fmt/fmt.h>

#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <optional>
#include <iostream>



namespace bringauto::logging {
/**
 * This class handles logger creation and addition of sinks
 * Supported message types are: std::string, const char*, fmt
 * Call addSink() to add sinks and init() to initialize the logger. log with log() and log<Verbosity>() functions
 */
class Logger {
public:
	/***
	 * Log message verbosity enum
	 *
	 * Debug        - debugging messages (i.e. memory dump)
	 * Info         - actual state of program (i.e. module initialized)
	 * Warning      - condition that might cause a problem in the future but program can ignore it (i.e. missing parameter and using default one)
	 * Error        - error occurs but program is able to continue (i.e. failed thread)
	 * Critical     - program is unable to continue (i.e. segmentation fault)
	 * LowestLevel  - contains the lowest verbosity level
	 * HighestLevel - contains the highest verbosity level
	 */
	enum class Verbosity: int {
		Debug = 0,
		Info,
		Warning,
		Error,
		Critical,
		LowestLevel = Debug,
		HighestLevel = Critical,
	};

	/**
	 * Structure containing global logger parameters
	 */
	struct LoggerSettings {
		/**
		 * Constructor, logger does need name of program and default verbosity in order to work correctly
		 */
		LoggerSettings(const std::string &nameOfProgram, Verbosity defaultVerbosity): programName(
				nameOfProgram), verbosity(defaultVerbosity) {};
		const std::string programName {};                      ///name of program that will be used in log
		const Verbosity verbosity {};                          ///default verbosity for all sinks (lower bound), if set to Warning, only Warning, Error and Critical will be logged

		/**
		 * Structure defining log filters
		 */
		struct Filter {
			bool filterRepeatingMessages {
					false };                                   /// if true repeating messages will be filtered based on repeatingMessageIntervalInMS
			std::optional<int> repeatingMessageIntervalInMS;  ///if repeating messages filter is set, repeating message will be able to log every repeatingMessageIntervalInMS milliseconds, rest of them will be discarded
		};
		std::optional<Filter> filter;                         ///optional filters
		std::optional<std::string> logFormat;                 ///log format see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
	};

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
		initLogger(settings);
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
	 * @tparam Args fmt/std::format arguments message pack, fmt is supported format
	 * @param verbosity see Verbosity enum for options
	 * @param message message to log of supported type - see isSupportedType()
	 * @param args additional arguments for supported types
	 *
	 */
	template <typename T, typename... Args>
	static constexpr void log(Verbosity verbosity, T message, Args... args) {
		if(!initialized_) {
			throw std::runtime_error("Logger was not initialize! Please call Logger::init() before log functions");
		}
		if(isSupportedType(message)) {
			logImplementation<decltype(getFormattedString(message, args...))>(verbosity,
																			  getFormattedString(message, args...));
			return;
		}
		logImplementation(Verbosity::Warning, "Unsupported message type");
	};

	/**
	 * Log debugging message, method will call log() with Debug verbosity
	 * @tparam T message with supported type - see isSupportedType() to see which types are supported
	 * @tparam Args fmt/std::format arguments message pack, fmt is supported format
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
	 * @tparam Args fmt/std::format arguments message pack, fmt is supported format
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
	 * @tparam Args fmt/std::format arguments message pack, fmt is supported format
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
	 * @tparam Args fmt/std::format arguments message pack, fmt is supported format
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
	 * @tparam Args fmt/std::format arguments message pack, fmt is supported format
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
		destroyLogger();
		sinks_.clear();
		initialized_ = false;
	};

private:
	inline static std::vector<std::shared_ptr<Sink>> sinks_; ///static list of sinks that will be added to logger
	inline static std::string programName_ {};                ///name of program that will be used in logging
	inline static bool initialized_ { false };                  ///true if logger is initialized (able to log)

	/**
	 * Message logging, method will log message to logger and/or all sinks
	 * @tparam T supported message type
	 * @param verbosity verbosity lvl
	 * @param message message to log
	 */
	template <typename T>
	static void logImplementation(Verbosity verbosity, T message);

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
	 * Logger supports fmt formatting (in future will be replaced with std::format) this method takes input string (or other supported string type like const char*)
	 * and argument pack that contain all arguments needed to create final string and uses fmt to transform it into final string that will be logged
	 * to see fmt formatting example see https://fmt.dev/latest/index.html
	 * @tparam T supported message type
	 * @tparam Args argument pack
	 * @param message message in fmt format or simple string
	 * @param args arguments needed in message to form final string
	 * @return final string
	 */
	template <typename T, typename ...Args>
	static std::string getFormattedString(T message, Args ...args) {
		std::string formattedString;
		try {
			formattedString = fmt::format(message, args...);
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
