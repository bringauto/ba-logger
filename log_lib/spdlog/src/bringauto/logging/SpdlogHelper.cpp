
#include <bringauto/logging/SpdlogHelper.hpp>

#include <iostream>



spdlog::level::level_enum
bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(bringauto::logging::Logger::Verbosity verbosity) {
	switch(verbosity) {
		case Logger::Verbosity::Debug:
			return spdlog::level::debug;
		case Logger::Verbosity::Info:
			return spdlog::level::info;
		case Logger::Verbosity::Warning:
			return spdlog::level::warn;
		case Logger::Verbosity::Error:
			return spdlog::level::err;
		case Logger::Verbosity::Critical:
			return spdlog::level::critical;
		default:
			throw std::runtime_error("Unknown verbosity value");
	}
}
