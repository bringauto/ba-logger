#pragma once

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/Sink.hpp>

#include <iostream>
#include <optional>
#include <regex>
#include <string>



namespace bringauto::logging {
/**
 * @brief Syslog argument for openlog(), see https://www.man7.org/linux/man-pages/man3/syslog.3.html
 */
enum class Option {
	E_LOG_PID,
	E_LOG_CONS,
	E_LOG_ODELAY,
	E_LOG_NDELAY,
	E_LOG_NOWAIT,
	E_LOG_PERROR

};
/**
 * @brief Facility specifies which type of program is logging the message, see https://www.man7.org/linux/man-pages/man3/syslog.3.html
 */
enum class Facility {
	E_LOG_AUTH,
	E_LOG_AUTHPRIV,
	E_LOG_CRON,
	E_LOG_DAEMON,
	E_LOG_FTP,
	E_LOG_KERN,
	E_LOG_LOCAL0,
	E_LOG_LOCAL1,
	E_LOG_LOCAL2,
	E_LOG_LOCAL3,
	E_LOG_LOCAL4,
	E_LOG_LOCAL5,
	E_LOG_LOCAL6,
	E_LOG_LOCAL7,
	E_LOG_LPR,
	E_LOG_MAIL,
	E_LOG_NEWS,
	E_LOG_SYSLOG,
	E_LOG_USER,
	E_LOG_UUCP
};

class SyslogSink: public Sink {
	/**
	 * Specific sink for logging to syslog
	 */
public:
	/**
	 * Specific syslog sink parameters
	 */
	struct Params {
		/**
		 * Constructor, syslog sink need identity program name, option control operations of openlog() and syslog(),
		 * facility specifies which type of program is logging the message
		 */
		Params(const std::string &identity, Option options, Facility facility, bool enableFormatting): identity(identity),
																									   options(options), facility(facility), enableFormatting(enableFormatting) {};
		std::string identity;
		Option options;
		Facility facility;
		std::optional<LoggerVerbosity> verbosity;
		bool enableFormatting;
	};

	/**
	 * Constructor
	 * @param params syslog sink specific parameters
	 */
	SyslogSink(const Params &params): params_(params) {};

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param programName name of program, logger uses same name
	 */
	void init(const std::string &programName) override;

private:
	Params params_;
};
}