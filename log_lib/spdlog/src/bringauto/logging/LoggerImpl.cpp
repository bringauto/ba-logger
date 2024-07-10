#include <bringauto/logging/LoggerImpl.hpp>
#include <bringauto/logging/SpdlogHelper.hpp>

#include <spdlog/spdlog.h>

#include <iostream>




void bringauto::logging::LoggerImpl::initLogger(const LoggerSettings &settings) {
	auto logger = std::make_shared<spdlog::logger>(settings.programName);
	logger->set_level(bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(settings.verbosity));
	spdlog::register_logger(logger);

	if(settings.filter.has_value()) {
		std::cerr << "Filters are not implemented in spdlog" << std::endl;
	}

	if(settings.logFormat.has_value()) {
		std::cerr << "Log format is not implemented in spdlog" << std::endl;
	}
}


void bringauto::logging::LoggerImpl::destroyLogger() {
	spdlog::drop_all();
}




void bringauto::logging::LoggerImpl::logImplementation(Verbosity verbosity, std::string message, const std::string &programName) {
	auto logger = spdlog::get(programName);
	logger->log(bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(verbosity), message);
}


void bringauto::logging::LoggerImpl::logImplementation(Verbosity verbosity, char const *message, const std::string &programName) {
	auto logger = spdlog::get(programName);
	logger->log(bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(verbosity), message);
}



