#pragma once

#include <bringauto/logging/Concepts.hpp>
#include <bringauto/logging/LoggerId.hpp>
#include <bringauto/logging/LoggerVerbosity.hpp>

#include <string>



namespace bringauto::logging {
/**
 * Base class that all specific sink types will derive from
 */
class Sink {
private:
	template <LoggerId ID, Logable K>
	friend class Logger;

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param loggerName name of logger
	 */
	virtual void init(const std::string &loggerName) {};
};
}