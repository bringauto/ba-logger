#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/SyslogSink.hpp>
#include <ImplementationTests.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <fstream>



using namespace bringauto::logging;

using Logger1 = Logger<testId, LoggerImpl>;

TEST_F(ImplementationTests, destroyLogger) {
	Logger1::addSink<ConsoleSink>();
	EXPECT_NO_THROW(Logger1::init(LoggerSettings { loggerName, LoggerVerbosity::Debug }));
	Logger1::destroy();
	Logger1::addSink<ConsoleSink>();
	EXPECT_NO_THROW(Logger1::init(LoggerSettings { loggerName, LoggerVerbosity::Debug }));
}

TEST_F(ImplementationTests, logWrongVerbosity) {
	Logger1::addSink<ConsoleSink>();
	Logger1::init(LoggerSettings { loggerName, LoggerVerbosity::Debug });
	EXPECT_ANY_THROW(Logger1::log((LoggerVerbosity)INT_MAX, logMessage));


}

TEST_F(ImplementationTests, loggerCreateWrongVerbosity) {
	Logger1::addSink<ConsoleSink>();
	EXPECT_ANY_THROW(Logger1::init(LoggerSettings { loggerName, (LoggerVerbosity)INT_MAX }));
}

TEST_F(ImplementationTests, sinkCreateWrongVerbosity) {
	ConsoleSink::Params params {};
	params.verbosity = (LoggerVerbosity)INT_MAX;

	Logger1::addSink<ConsoleSink>(params);
	EXPECT_ANY_THROW(Logger1::init(LoggerSettings(loggerName, LoggerVerbosity::Debug)));
}

TEST_F(ImplementationTests, consoleLog) {
	Logger1::addSink<ConsoleSink>();
	Logger1::init({ loggerName, LoggerVerbosity::Debug });

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logDebug(logMessage););
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("debug"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logInfo(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("info"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logWarning(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("warning"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logError(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("error"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logCritical(logMessage););
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
	Logger1::addSink<FileSink>(params);
	Logger1::init({ loggerName, LoggerVerbosity::Debug });

	EXPECT_NO_THROW(Logger1::logDebug(logMessage););
	EXPECT_NO_THROW(Logger1::logInfo(logMessage););
	EXPECT_NO_THROW(Logger1::logWarning(logMessage););
	EXPECT_NO_THROW(Logger1::logError(logMessage););
	EXPECT_NO_THROW(Logger1::logCritical(logMessage););
	Logger1::destroy(); //need for flush

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

	EXPECT_NO_THROW(Logger1::addSink<FileSink>(params));
	EXPECT_NO_THROW(Logger1::init({ loggerName, LoggerVerbosity::Debug }));

	for(int i = 0; i < 10; i++) {
		EXPECT_NO_THROW(Logger1::logCritical(logMessage));
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
	params.verbosity = LoggerVerbosity::Warning;
	Logger1::addSink<ConsoleSink>(params);


	Logger1::init({ loggerName, LoggerVerbosity::Info });

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logDebug(logMessage););
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output.find(logMessage), std::string::npos);
	EXPECT_EQ(output.find("debug"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logInfo(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output.find(logMessage), std::string::npos);
	EXPECT_EQ(output.find("info"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logWarning(logMessage););
	output = testing::internal::GetCapturedStdout();
	EXPECT_NE(output.find(logMessage), std::string::npos);
	EXPECT_NE(output.find("warning"), std::string::npos);

	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(Logger1::logCritical(logMessage););
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
	params.verbosity = LoggerVerbosity::Warning;
	params.numberOfRotatedFiles = 1;
	params.maxFileSize = 16*1024;
	Logger1::addSink<FileSink>(params);


	Logger1::init({ loggerName, LoggerVerbosity::Info });
	EXPECT_NO_THROW(Logger1::logDebug(logMessage););
	EXPECT_NO_THROW(Logger1::logInfo(logMessage););
	EXPECT_NO_THROW(Logger1::logWarning(logMessage););
	EXPECT_NO_THROW(Logger1::logError(logMessage););
	EXPECT_NO_THROW(Logger1::logCritical(logMessage););
	Logger1::destroy(); //need for flush

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

TEST_F(ImplementationTests,destroySyslogLogger) {
	Logger1::addSink<SyslogSink>({ "lol", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true });
	EXPECT_NO_THROW(Logger1::init(LoggerSettings { loggerName, LoggerVerbosity::Debug }));
	Logger1::destroy();
	Logger1::addSink<SyslogSink>({ "lol", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true });
	EXPECT_NO_THROW(Logger1::init(LoggerSettings { loggerName, LoggerVerbosity::Debug }));
}

TEST_F(ImplementationTests,SyslogWrongVerbosity) {
	Logger1::addSink<SyslogSink>({ "lol", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true });
	Logger1::init(LoggerSettings { loggerName, LoggerVerbosity::Debug });
	EXPECT_ANY_THROW(Logger1::log((LoggerVerbosity)INT_MAX, logMessage));
}

TEST_F(ImplementationTests, SyslogLoggerCreateWrongVerbosity) {
	Logger1::addSink<SyslogSink>({ "lol", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true });
	EXPECT_ANY_THROW(Logger1::init(LoggerSettings { loggerName, (LoggerVerbosity)INT_MAX }));
}

TEST_F(ImplementationTests, SyslogSinkCreateWrongVerbosity) {
	SyslogSink::Params params {"lol", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true};
	params.verbosity = (LoggerVerbosity)INT_MAX;

	Logger1::addSink<SyslogSink>(params);
	EXPECT_ANY_THROW(Logger1::init(LoggerSettings(loggerName, LoggerVerbosity::Debug)));
}
