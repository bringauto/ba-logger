#include <MultipleLoggersApiTests.hpp>



using namespace bringauto::logging;

using Logger1 = Logger<testId, LoggerImpl>;
using Logger2 = Logger<testId2, LoggerImpl>;


TEST_F(LoggerApiTests, multipleLoggerInit
) {
	Logger1::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW (Logger1::init(defaultLoggerSettings));

	Logger2::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW (Logger2::init(secondLoggerSettings));

}


TEST_F(LoggerApiTests, badMultipleLoggerInit
) {
	Logger1::addSink<ConsoleSink>(consoleSinkParams);

	EXPECT_NO_THROW (Logger1::init(defaultLoggerSettings));

	//Logger2::addSink<ConsoleSink>(consoleSinkParams);

	//EXPECT_ANY_THROW (Logger2::init(defaultLoggerSettings));

}