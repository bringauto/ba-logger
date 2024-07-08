#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/SpdlogHelper.hpp>

#include <spdlog/spdlog.h>

#include <iostream>



template <>
void bringauto::logging::Logger<>::initLogger(const LoggerSettings &settings) {
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

template <>
void bringauto::logging::Logger<>::destroyLogger() {
	spdlog::drop_all();
}

template <>
void bringauto::logging::Logger<>::logImplementation(Verbosity verbosity, std::string message) {
	auto logger = spdlog::get(programName_);
	logger->log(bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(verbosity), message);
}

template <>
void bringauto::logging::Logger<>::logImplementation(Verbosity verbosity, char const *message) {
	auto logger = spdlog::get(programName_);
	logger->log(bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(verbosity), message);
}



