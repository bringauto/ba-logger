#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/SpdlogHelper.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>



void bringauto::logging::FileSink::init(const std::string &logerName) {
	auto logger = spdlog::get(logerName);
	if(!logger) {
		throw std::runtime_error("Logger was not initialized (" + logerName + ")");
	}

	auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(params_.fileDir.string() + params_.fileName,
																	   params_.maxFileSize,
																	   params_.numberOfRotatedFiles, false);

	if(params_.verbosity.has_value()) {
		sink->set_level(logging::SpdlogHelper::mapToSpdlogVerbosity(params_.verbosity.value()));
	}
	logger->sinks().push_back(sink);
}
