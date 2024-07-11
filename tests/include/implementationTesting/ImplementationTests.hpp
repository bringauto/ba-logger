#pragma once

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/LoggerId.hpp>

#include <gtest/gtest.h>

#include <mutex>
#include <iostream>
#include <fstream>



std::mutex g_singleThread;
constexpr bringauto::logging::LoggerId testId = {.id = "id1"};

class ImplementationTests: public ::testing::Test {
protected:

	void SetUp() override {
		g_singleThread.lock();
	}
	
	
	void TearDown() override {
		bringauto::logging::Logger<testId, bringauto::logging::LoggerImpl>::destroy();
		g_singleThread.unlock();
		removeLogFiles();
	}

	void removeLogFiles() {
		std::filesystem::remove(logDirPath.string() + filename + extension);
		std::filesystem::remove(logDirPath.string() + filename + ".1" + extension);
		std::filesystem::remove(logDirPath.string() + filename + ".2" + extension);
		std::filesystem::remove(logDirPath.string() + filename + ".3" + extension);
		std::filesystem::remove(logDirPath.string() + filename + ".4" + extension);
		std::filesystem::remove(logDirPath.string() + filename + ".5" + extension);
	}

	bool hasWriteAccess(const std::filesystem::path &dir) {
		std::filesystem::path tmpFilePath(dir.string() + "tmp.tmp");
		FILE *fp = fopen(tmpFilePath.c_str(), "w");
		bool isWritable = fp;
		if(isWritable) {
			fclose(fp);
			std::remove(tmpFilePath.c_str());
		}
		return isWritable;
	}

	const std::string logMessage { "Test logger message." };
	const std::string loggerName { "test" };
	const std::string filename { "test" };
	const std::string extension { ".txt" };
	const std::filesystem::path logDirPath { "/srv/yocto_cache/" };
	const std::filesystem::path logFilePath { logDirPath.string() + filename + extension };
	const unsigned int numberOfFiles = 5;
};