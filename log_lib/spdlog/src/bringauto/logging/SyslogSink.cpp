#include <bringauto/logging/SyslogSink.hpp>
#include <bringauto/logging/SpdlogHelper.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/syslog_sink.h>



using namespace bringauto::logging;

const std::map<Option, int> optionMap = {
		{ Option::E_LOG_PID,    LOG_PID },
		{ Option::E_LOG_CONS,   LOG_CONS },
		{ Option::E_LOG_ODELAY, LOG_ODELAY },
		{ Option::E_LOG_NDELAY, LOG_NDELAY },
		{ Option::E_LOG_NOWAIT, LOG_NOWAIT },
		{ Option::E_LOG_PERROR, LOG_PERROR }
};

const std::map<Facility, int> facilityMap = {
		{ Facility::E_LOG_AUTH,     LOG_AUTH },
		{ Facility::E_LOG_AUTHPRIV, LOG_AUTHPRIV },
		{ Facility::E_LOG_CRON,     LOG_CRON },
		{ Facility::E_LOG_DAEMON,   LOG_DAEMON },
		{ Facility::E_LOG_FTP,      LOG_FTP },
		{ Facility::E_LOG_KERN,     LOG_KERN },
		{ Facility::E_LOG_LOCAL0,   LOG_LOCAL0 },
		{ Facility::E_LOG_LOCAL1,   LOG_LOCAL1 },
		{ Facility::E_LOG_LOCAL2,   LOG_LOCAL2 },
		{ Facility::E_LOG_LOCAL3,   LOG_LOCAL3 },
		{ Facility::E_LOG_LOCAL4,   LOG_LOCAL4 },
		{ Facility::E_LOG_LOCAL5,   LOG_LOCAL5 },
		{ Facility::E_LOG_LOCAL6,   LOG_LOCAL6 },
		{ Facility::E_LOG_LOCAL7,   LOG_LOCAL7 },
		{ Facility::E_LOG_LPR,      LOG_LPR },
		{ Facility::E_LOG_MAIL,     LOG_MAIL },
		{ Facility::E_LOG_NEWS,     LOG_NEWS },
		{ Facility::E_LOG_SYSLOG,   LOG_SYSLOG },
		{ Facility::E_LOG_USER,     LOG_USER },
		{ Facility::E_LOG_UUCP,     LOG_UUCP }
};

void SyslogSink::init(const std::string &logerName) {
	auto logger = spdlog::get(logerName);
	if(!logger) {
		throw std::runtime_error("Logger was not initialized (" + logerName + ")");
	}

	auto sink = std::make_shared<spdlog::sinks::syslog_sink_mt>(params_.identity, optionMap.at(params_.options),
																facilityMap.at(params_.facility),
																params_.enableFormatting);

	if(params_.verbosity.has_value()) {
		sink->set_level(logging::SpdlogHelper::mapToSpdlogVerbosity(params_.verbosity.value()));
	}
	logger->sinks().push_back(sink);
}
