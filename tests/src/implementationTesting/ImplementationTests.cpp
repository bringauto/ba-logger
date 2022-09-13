#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>

#include <ImplementationTests.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <fstream>



using namespace bringauto::logging;

TEST_F(ImplementationTests, destroyLogger) {
	Logger::addSink<ConsoleSink>();
	EXPECT_NO_THROW(Logger::init(Logger::LoggerSettings { loggerName, Logger::Verbosity::Debug }));
	Logger::destroy();
	Logger::addSink<ConsoleSink>();
	EXPECT_NO_THROW(Logger::init(Logger::LoggerSettings { loggerName, Logger::Verbosity::Debug }));
}

TEST_F(ImplementationTests, logWrongVerbosity) {
	Logger::addSink<ConsoleSink>();
	Logger::init(Logger::LoggerSettings { loggerName, Logger::Verbosity::Debug });
	EXPECT_ANY_THROW(Logger::log((Logger::Verbosity)INT_MAX, logMessage));


}

TEST_F(ImplementationTests, loggerCreateWrongVerbosity) {
	Logger::addSink<ConsoleSink>();
	EXPECT_ANY_THROW(Logger::init(Logger::LoggerSettings { loggerName, (Logger::Verbosity)INT_MAX }));
}


TEST_F(ImplementationTests, sinkCreateWrongVerbosity) {
	ConsoleSink::Params params {};
	params.verbosity = (Logger::Verbosity)INT_MAX;

	Logger::addSink<ConsoleSink>(params);
	EXPECT_ANY_THROW(Logger::init(Logger::LoggerSettings(loggerName, Logger::Verbosity::Debug)));
}

TEST_F(ImplementationTests, consoleLog) {
	Logger::addSink<ConsoleSink>();
	Logger::init({ loggerName, Logger::Verbosity::Debug });

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logDebug(logMessage););
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("debug"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logInfo(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("info"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logWarning(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("warning"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logError(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("error"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logCritical(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("critical"), std::string::npos);
}

TEST_F(ImplementationTests, fileLog) {
	if(!hasWriteAccess(logDirPath)) {
		std::cerr << "Unable to test file log, no write rights in " << logDirPath << " folder" << std::endl;
		return;
	}
	FileSink::Params params { logDirPath, filename + extension };
	params.numberOfRotatedFiles = 1;
	params.maxFileSize = 16*1024;
	Logger::addSink<FileSink>(params);
	Logger::init({ loggerName, Logger::Verbosity::Debug });

	EXPECT_NO_THROW(Logger::logDebug(logMessage););
	EXPECT_NO_THROW(Logger::logInfo(logMessage););
	EXPECT_NO_THROW(Logger::logWarning(logMessage););
	EXPECT_NO_THROW(Logger::logError(logMessage););
	EXPECT_NO_THROW(Logger::logCritical(logMessage););
	Logger::destroy(); //need for flush

	std::ifstream logFile(logFilePath);
	std::string logFileContent((std::istreambuf_iterator<char>(logFile)),
							   std::istreambuf_iterator<char>());
	logFile.close();
	EXPECT_EQ(std::count(logFileContent.begin(), logFileContent.end(), '\n'), 5);
	EXPECT_NE(logFileContent.find("debug"), std::string::npos);
	EXPECT_NE(logFileContent.find("info"), std::string::npos);
	EXPECT_NE(logFileContent.find("warning"), std::string::npos);
	EXPECT_NE(logFileContent.find("error"), std::string::npos);
	EXPECT_NE(logFileContent.find("critical"), std::string::npos);

	EXPECT_NE(logFileContent.find(logMessage), std::string::npos);
}

TEST_F(ImplementationTests, fileRotation) {
	if(!hasWriteAccess(logDirPath)) {
		std::cerr << "Unable to test file log, no write rights in " << logDirPath << " folder" << std::endl;
		return;
	}

	FileSink::Params params { logDirPath, filename + extension };
	params.maxFileSize = 10;
	params.numberOfRotatedFiles = numberOfFiles;

	EXPECT_NO_THROW(Logger::addSink<FileSink>(params));
	EXPECT_NO_THROW(Logger::init({ loggerName, Logger::Verbosity::Debug }));

	for(int i = 0; i < 10; i++) {
		EXPECT_NO_THROW(Logger::logCritical(logMessage));
	}

	EXPECT_TRUE(std::filesystem::exists(logDirPath.string() + filename + extension));
	EXPECT_TRUE(std::filesystem::exists(logDirPath.string() + filename + ".1" + extension));
	EXPECT_TRUE(std::filesystem::exists(logDirPath.string() + filename + ".2" + extension));
	EXPECT_TRUE(std::filesystem::exists(logDirPath.string() + filename + ".3" + extension));
	EXPECT_TRUE(std::filesystem::exists(logDirPath.string() + filename + ".4" + extension));
	EXPECT_TRUE(std::filesystem::exists(logDirPath.string() + filename + ".5" + extension));
}

TEST_F(ImplementationTests, verbosityIgnoreLogConsole) {
	ConsoleSink::Params params {};
	params.verbosity = Logger::Verbosity::Warning;
	Logger::addSink<ConsoleSink>(params);


	Logger::init({ loggerName, Logger::Verbosity::Info });

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logDebug(logMessage););
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output.find(logMessage), std::string::npos);
	EXPECT_EQ(output.find("debug"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logInfo(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output.find(logMessage), std::string::npos);
	EXPECT_EQ(output.find("info"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logWarning(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("warning"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger::logCritical(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("critical"), std::string::npos);
}

TEST_F(ImplementationTests, verbosityIgnoreLogFile) {
	if(!hasWriteAccess(logDirPath)) {
		std::cerr << "Unable to test file log, no write rights in " << logDirPath << " folder" << std::endl;
		return;
	}
	FileSink::Params params { logDirPath, filename + extension };
	params.verbosity = Logger::Verbosity::Warning;
	params.numberOfRotatedFiles = 1;
	params.maxFileSize = 16*1024;
	Logger::addSink<FileSink>(params);


	Logger::init({ loggerName, Logger::Verbosity::Info });
	EXPECT_NO_THROW(Logger::logDebug(logMessage););
	EXPECT_NO_THROW(Logger::logInfo(logMessage););
	EXPECT_NO_THROW(Logger::logWarning(logMessage););
	EXPECT_NO_THROW(Logger::logError(logMessage););
	EXPECT_NO_THROW(Logger::logCritical(logMessage););
	Logger::destroy(); //need for flush

	std::ifstream logFile(logFilePath);
	std::string logFileContent((std::istreambuf_iterator<char>(logFile)),
							   std::istreambuf_iterator<char>());
	logFile.close();
	EXPECT_EQ(std::count(logFileContent.begin(), logFileContent.end(), '\n'), 3);
	EXPECT_EQ(logFileContent.find("debug"), std::string::npos);
	EXPECT_EQ(logFileContent.find("info"), std::string::npos);
	EXPECT_NE(logFileContent.find("warning"), std::string::npos);
	EXPECT_NE(logFileContent.find("error"), std::string::npos);
	EXPECT_NE(logFileContent.find("critical"), std::string::npos);

	EXPECT_NE(logFileContent.find(logMessage), std::string::npos);

}
