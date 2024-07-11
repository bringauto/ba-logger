#pragma once

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/LoggerId.hpp>

#include <gtest/gtest.h>



constexpr bringauto::logging::LoggerId testId = {.id = "id1"};
class SinksApiTests: public ::testing::Test {
protected:

	void SetUp() override {
	}

	void TearDown() override {
		bringauto::logging::Logger<testId, bringauto::logging::LoggerImpl>::destroy();
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
};