#pragma once

#include <bringauto/logging/LoggerId.hpp>
#include <bringauto/logging/LoggerVerbosity.hpp>

#include <string>



namespace bringauto::logging {
/**
 * Base class that all specific sink types will derive from
 */
class Sink {
private:
	template <LoggerId ID>
	friend class Logger;

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param programName name of program, logger uses same name
	 */
	virtual void init(const std::string &programName) {};
};
}