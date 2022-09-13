#pragma once

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/RemoteSink.hpp>
#include <bringauto/logging/FileSink.hpp>

#include <gtest/gtest.h>

#include <string>



class LoggerApiTests: public ::testing::Test {
protected:

	void SetUp() override {
	}

	void TearDown() override {
		bringauto::logging::Logger::destroy();
	}

	bringauto::logging::Logger::LoggerSettings defaultLoggerSettings { "test",
																	   bringauto::logging::Logger::Verbosity::Debug };
	bringauto::logging::ConsoleSink::Params consoleSinkParams {};
	bringauto::logging::RemoteSink::Params remoteSinkParams { "127.0.0.1", 15379 };
	bringauto::logging::FileSink::Params fileSinkParams { "./", "tests.txt" };
	const std::string logMessage { "log this message" };
};