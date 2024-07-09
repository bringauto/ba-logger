
#include <bringauto/logging/SpdlogHelper.hpp>

#include <iostream>



spdlog::level::level_enum
bringauto::logging::SpdlogHelper::mapToSpdlogVerbosity(bringauto::logging::LoggerVerbosity verbosity) {
	switch(verbosity) {
		case LoggerVerbosity::Debug:
			return spdlog::level::debug;
		case LoggerVerbosity::Info:
			return spdlog::level::info;
		case LoggerVerbosity::Warning:
			return spdlog::level::warn;
		case LoggerVerbosity::Error:
			return spdlog::level::err;
		case LoggerVerbosity::Critical:
			return spdlog::level::critical;
		default:
			throw std::runtime_error("Unknown verbosity value");
	}
}
