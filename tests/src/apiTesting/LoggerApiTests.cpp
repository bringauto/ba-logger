#include <LoggerApiTests.hpp>



using namespace bringauto::logging;

using Logger1 = Logger<testId, LoggerImpl>;

TEST_F(LoggerApiTests, settingsAssertions) {
	ASSERT_EQ(defaultLoggerSettings.loggerName, "test");
	ASSERT_EQ(defaultLoggerSettings.verbosity, bringauto::logging::LoggerVerbosity::Debug);
	ASSERT_EQ(secondLoggerSettings.loggerName, "test2");
	ASSERT_EQ(secondLoggerSettings.verbosity, bringauto::logging::LoggerVerbosity::Debug);
	ASSERT_EQ(remoteSinkParams.ipv4, "127.0.0.1");
	ASSERT_EQ(remoteSinkParams.port, 15379);
	ASSERT_EQ(fileSinkParams.fileDir, "./");
	ASSERT_EQ(fileSinkParams.fileName, "tests.txt");
	ASSERT_EQ(logMessage, "log this message");
}

TEST_F(LoggerApiTests, logWithoutInit) {
	EXPECT_ANY_THROW(Logger1::logWarning(logMessage));

	EXPECT_ANY_THROW(Logger1::log(LoggerVerbosity::Error, logMessage));
}

TEST_F(LoggerApiTests, loggerInit) {
	EXPECT_NO_THROW(Logger1::addSink<ConsoleSink>(consoleSinkParams));

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));
}

TEST_F(LoggerApiTests, loggerDoubleInit) {
	Logger1::addSink<ConsoleSink>();

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));

	EXPECT_ANY_THROW(Logger1::init(defaultLoggerSettings));
}

TEST_F(LoggerApiTests, loggerDoubleInit2) {
	Logger1::addSink<ConsoleSink>();

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));

	EXPECT_ANY_THROW(Logger1::init(secondLoggerSettings));
}

TEST_F(LoggerApiTests, initWithoutSink) {
	EXPECT_ANY_THROW(Logger1::init(defaultLoggerSettings));
}

TEST_F(LoggerApiTests, deinitLogger) {
	Logger1::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));

	EXPECT_NO_THROW(Logger1::destroy());

	Logger1::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));
}

TEST_F(LoggerApiTests, multipleSinksAdd) {
	EXPECT_NO_THROW(Logger1::addSink<ConsoleSink>(consoleSinkParams));

	EXPECT_NO_THROW(Logger1::addSink<RemoteSink>(remoteSinkParams));

	EXPECT_NO_THROW(Logger1::addSink<FileSink>(fileSinkParams));

	EXPECT_NO_THROW(Logger1::addSink<ConsoleSink>(consoleSinkParams));

	EXPECT_NO_THROW(Logger1::addSink<RemoteSink>(remoteSinkParams));

	EXPECT_NO_THROW(Logger1::addSink<FileSink>(fileSinkParams));

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));
}

TEST_F(LoggerApiTests, sinkAddAfterInit) {
	EXPECT_NO_THROW(Logger1::addSink<ConsoleSink>(consoleSinkParams));

	EXPECT_NO_THROW(Logger1::init(defaultLoggerSettings));

	EXPECT_ANY_THROW(Logger1::addSink<ConsoleSink>(consoleSinkParams));
}
