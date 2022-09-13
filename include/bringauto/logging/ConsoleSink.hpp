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
		std::optional<Logger::Verbosity> verbosity; ///verbosity specific for sink, overrides default logger verbosity
	};

	/**
	 * Constructor
	 * @param params console sink parameters
	 */
	ConsoleSink(Params params): params_(params) {};

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param programName name of program, logger uses same name
	 */
	void init(const std::string &programName) override;

private:
	Params params_;
};
}