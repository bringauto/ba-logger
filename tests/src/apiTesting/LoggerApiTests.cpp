#include <LoggerApiTests.hpp>



using namespace bringauto::logging;

TEST_F(LoggerApiTests, logWithoutInit
) {
	EXPECT_ANY_THROW (Logger::logWarning(logMessage));

	EXPECT_ANY_THROW (Logger::log(Logger::Verbosity::Error, logMessage));

}

TEST_F(LoggerApiTests, loggerInit
) {
	EXPECT_NO_THROW (Logger::addSink<ConsoleSink>(consoleSinkParams));

	EXPECT_NO_THROW (Logger::init(defaultLoggerSettings));

}

TEST_F(LoggerApiTests, loggerDoubleInit
) {
	Logger::addSink<ConsoleSink>();

	EXPECT_NO_THROW (Logger::init(defaultLoggerSettings));

	EXPECT_ANY_THROW (Logger::init(defaultLoggerSettings));

}



TEST_F(LoggerApiTests, initWithoutSink
) {
	EXPECT_ANY_THROW (Logger::init(defaultLoggerSettings));

}

TEST_F(LoggerApiTests, deinitLogger
) {
	Logger::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW (Logger::init(defaultLoggerSettings));

	EXPECT_NO_THROW (Logger::destroy());

	Logger::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW (Logger::init(defaultLoggerSettings));

}

TEST_F(LoggerApiTests, multipleSinksAdd
) {
	EXPECT_NO_THROW(Logger::addSink<ConsoleSink>(consoleSinkParams)
	);

	EXPECT_NO_THROW (Logger::addSink<RemoteSink>(remoteSinkParams));

	EXPECT_NO_THROW (Logger::addSink<FileSink>(fileSinkParams));

	EXPECT_NO_THROW(Logger::addSink<ConsoleSink>(consoleSinkParams)
	);

	EXPECT_NO_THROW (Logger::addSink<RemoteSink>(remoteSinkParams));

	EXPECT_NO_THROW (Logger::addSink<FileSink>(fileSinkParams));

	EXPECT_NO_THROW (Logger::init(defaultLoggerSettings));

}

TEST_F(LoggerApiTests, sinkAddAfterInit
) {
	EXPECT_NO_THROW(Logger::addSink<ConsoleSink>(consoleSinkParams)
	);

	EXPECT_NO_THROW (Logger::init(defaultLoggerSettings));

	EXPECT_ANY_THROW(Logger::addSink<ConsoleSink>(consoleSinkParams)
	);
}





