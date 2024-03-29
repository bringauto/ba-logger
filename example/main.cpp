
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/RemoteSink.hpp>
#include <bringauto/logging/SyslogSink.hpp>
#include <bringauto/logging/SizeLiterals.hpp>



using namespace bringauto::logging;

void createConsoleSink() {
	Logger::addSink<ConsoleSink>();

	ConsoleSink::Params paramConsoleSink;
	paramConsoleSink.verbosity = Logger::Verbosity::Critical;
	Logger::addSink<ConsoleSink>(paramConsoleSink);
}

void createFileSink() {
	Logger::addSink<FileSink>({ "./", "log.txt" });

	FileSink::Params paramFileSink { "./", "log2.txt" };
	paramFileSink.maxFileSize = 5_MB;
	paramFileSink.numberOfRotatedFiles = 2;
	paramFileSink.verbosity = Logger::Verbosity::Info;
	Logger::addSink<FileSink>(paramFileSink);
}

void createRemoteSink() {
	Logger::addSink<RemoteSink>({ "192.168.1.1", 4507 });

	RemoteSink::Params paramRemoteSink { "192.168.1.2", 3333 };
	paramRemoteSink.verbosity = Logger::Verbosity::Critical;
	Logger::addSink<RemoteSink>(paramRemoteSink);
}

void createSyslogSink() {
	Logger::addSink<SyslogSink>({ "syslog", bringauto::logging::Option::E_LOG_PERROR, bringauto::logging::Facility::E_LOG_USER, true });

	SyslogSink::Params paramRemoteSink { "syslog", bringauto::logging::Option::E_LOG_PID, bringauto::logging::Facility::E_LOG_SYSLOG, false };
	paramRemoteSink.verbosity = Logger::Verbosity::Critical;
	Logger::addSink<SyslogSink>(paramRemoteSink);
}

void initLogger() {
	Logger::LoggerSettings loggerSettings { "Demo app", Logger::Verbosity::Debug };
	loggerSettings.filter = { true, 200 };
	loggerSettings.logFormat = "*** [%H:%M:%S %z] ***";
	Logger::init(loggerSettings);
}

int main(int arg, char **argv) {
	createConsoleSink();
	createFileSink();
	createRemoteSink();
	createSyslogSink();
	initLogger();

	Logger::logInfo(std::string { "Demo app" });
	Logger::logInfo("Info about app");
	Logger::logInfo("Formated {}: {:08b}, {}", "message", 54, 34.8);
	Logger::logDebug("Debug");
	Logger::logWarning("Warning");
	Logger::logError("Error");
	Logger::logCritical("Critical");
	Logger::log(Logger::Verbosity::Error, "Error2");
}
