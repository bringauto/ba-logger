#pragma once

#include <bringauto/logging/Sink.hpp>
#include <bringauto/logging/Logger.hpp>

#include <string>
#include <regex>
#include <iostream>
#include <optional>



namespace bringauto::logging {
enum class Option {
	E_LOG_PID,
	E_LOG_CONS,
	E_LOG_ODELAY,
	E_LOG_NDELAY,
	E_LOG_NOWAIT,
	E_LOG_PERROR

};

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
		 * Constructor, syslog sink need ident program name, option control operations of openlog() and syslog(),
		 * facility is default to be used if not specified otherwise
		 */
		Params(std::string identity, Option options, Facility facility, bool enableFormatting ): identiy_(identity),
		options_(options), facility_(facility), enableFormatting_(enableFormatting) {};
		std::string identiy_;
		Option options_;
		Facility facility_;
		std::optional<Logger::Verbosity> verbosity;
		bool enableFormatting_;
	};

	/**
	 * Constructor
	 * @param params syslog sink specific parameters
	 */
	SyslogSink(const Params &params) : params_(params) {};

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param programName name of program, logger uses same name
	 */
	void init(const std::string &programName) override;

private:
	Params params_;
	static const std::map<Option, int> optionMap;
	static const std::map<Facility, int> facilityMap;
};
}