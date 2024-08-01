#pragma once

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/Sink.hpp>

#include <optional>



namespace bringauto::logging {
/**
 * Specific sink for logging into console
 */
class ConsoleSink: public Sink {
public:
	struct Params {
		std::optional<LoggerVerbosity> verbosity; ///verbosity specific for sink, overrides default logger verbosity
	};

	/**
	 * Constructor
	 * @param params console sink parameters
	 */
	ConsoleSink(Params params): params_(params) {};

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param loggerName name of logger
	 */
	void init(const std::string &loggerName) override;

private:
	Params params_;
};
}