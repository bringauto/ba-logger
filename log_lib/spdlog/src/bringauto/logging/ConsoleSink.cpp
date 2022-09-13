#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/SpdlogHelper.hpp>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>



void bringauto::logging::ConsoleSink::init(const std::string &loggerName) {
	auto logger = spdlog::get(loggerName);
	if(!logger) {
		throw std::runtime_error("Logger was not initialized (" + loggerName + ")");
	}
	auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
	if(params_.verbosity.has_value()) {
		sink->set_level(logging::SpdlogHelper::mapToSpdlogVerbosity(params_.verbosity.value()));
	}

	logger->sinks().push_back(sink);
}
