#include <LoggerApiTests.hpp>



using namespace bringauto::logging;

using Logger1 = Logger<testId, LoggerImpl>;

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
