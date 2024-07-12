#pragma once

#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/RemoteSink.hpp>

#include <gtest/gtest.h>

#include <string>



constexpr bringauto::logging::LoggerId testId = {.id = "id1"};
class LoggerApiTests: public ::testing::Test {
protected:	

	void SetUp() override {
	}

	void TearDown() override {
		bringauto::logging::Logger<testId, bringauto::logging::LoggerImpl>::destroy();
	}

	bringauto::logging::LoggerSettings defaultLoggerSettings {"test",
															  bringauto::logging::LoggerVerbosity::Debug};
	bringauto::logging::LoggerSettings secondLoggerSettings {"test2",
															 bringauto::logging::LoggerVerbosity::Debug};
	bringauto::logging::ConsoleSink::Params consoleSinkParams {};
	bringauto::logging::RemoteSink::Params remoteSinkParams {"127.0.0.1", 15379};
	bringauto::logging::FileSink::Params fileSinkParams {"./", "tests.txt"};
	const std::string logMessage {"log this message"};
};