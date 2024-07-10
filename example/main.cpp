
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/RemoteSink.hpp>
#include <bringauto/logging/SyslogSink.hpp>
#include <bringauto/logging/SizeLiterals.hpp>



using namespace bringauto::logging;
constexpr LoggerId myContext = {.id = 1};
using Logger1 = Logger<myContext, LoggerImpl>;
void createConsoleSink() {
	Logger1::addSink<ConsoleSink>();

	ConsoleSink::Params paramConsoleSink;
	paramConsoleSink.verbosity = LoggerVerbosity::Critical;
	Logger1::addSink<ConsoleSink>(paramConsoleSink);
}

void createFileSink() {
	Logger1::addSink<FileSink>({ "./", "log.txt" });

	FileSink::Params paramFileSink { "./", "log2.txt" };
	paramFileSink.maxFileSize = 5_MB;
	paramFileSink.numberOfRotatedFiles = 2;
	paramFileSink.verbosity = LoggerVerbosity::Info;
	Logger1::addSink<FileSink>(paramFileSink);
}

void createRemoteSink() {
	Logger1::addSink<RemoteSink>({ "192.168.1.1", 4507 });

	RemoteSink::Params paramRemoteSink { "192.168.1.2", 3333 };
	paramRemoteSink.verbosity = LoggerVerbosity::Critical;
	Logger1::addSink<RemoteSink>(paramRemoteSink);
}

void createSyslogSink() {
	Logger1::addSink<SyslogSink>({ "syslog", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true });

	SyslogSink::Params paramRemoteSink { "syslog", bringauto::logging::Option::E_LOG_PID, bringauto::logging::Facility::E_LOG_SYSLOG, false };
	paramRemoteSink.verbosity = LoggerVerbosity::Critical;
	Logger1::addSink<SyslogSink>(paramRemoteSink);
}

void initLogger() {
	LoggerSettings loggerSettings { "Demo app", LoggerVerbosity::Debug };
	loggerSettings.filter = { true, 200 };
	loggerSettings.logFormat = "*** [%H:%M:%S %z] ***";
	Logger1::init(loggerSettings);
}

int main(int arg, char **argv) {
	createConsoleSink();
	createFileSink();
	createRemoteSink();
	createSyslogSink();
	initLogger();

	Logger1::logInfo(std::string { "Demo app" });
	Logger1::logInfo("Info about app");
	Logger1::logInfo("Formated {}: {:08b}, {}", "message", 54, 34.8);
	Logger1::logDebug("Debug");
	Logger1::logWarning("Warning");
	Logger1::logError("Error");
	Logger1::logCritical("Critical");
	Logger1::log(LoggerVerbosity::Error, "Error2");
}
