#pragma once

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/Sink.hpp>

#include <iostream>
#include <optional>
#include <regex>
#include <string>



namespace bringauto::logging {
class RemoteSink: public Sink {
	/**
	 * Specific sink for logging to remote server
	 */
public:
	/**
	 * Specific remote sink parameters
	 */
	struct Params {
		/**
		 * Constructor, remote sink does need ip of remote server and port in order to work correctly
		 */
		Params(const std::string &ipv4String, unsigned int serverPort): ipv4(ipv4String), port(serverPort) {
			throwIfParamsNotValid(ipv4String, serverPort);
		};
		const unsigned int port {};              ///remote app port
		const std::string ipv4 {};               ///remote server address
		std::optional<LoggerVerbosity> verbosity;///verbosity specific for sink, overrides default logger verbosity

	private:
		/**
		 * Checking validity of port and ip value, throws std::invalid_argument if arguments are invalid
		 * @param ipv4String IPv4 string to check
		 * @param serverPort port value to check
		 */
		static void throwIfParamsNotValid(const std::string &ipv4String, unsigned int serverPort) {
			const unsigned int minimalPort = 0;
			const unsigned int maximalPort = 65535;

			std::regex regex(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$)");
			if(!std::regex_match(ipv4String, regex)) {
				throw std::invalid_argument("Wrong ip address format, file sink cannot be created.");
			}

			if(serverPort < minimalPort || serverPort > maximalPort) {
				throw std::invalid_argument(
						"Wrong server port, have to be in range " + std::to_string(minimalPort) + "-" +
						std::to_string(maximalPort) + ".");
			}
		}
	};

	/**
	 * Constructor
	 * @param params remote sink specific parameters
	 */
	RemoteSink(const Params &params): params_(params) {};

	/**
	 * Specific initialization, file sink will be added to logger
	 * @param loggerName name of logger
	 */
	void init(const std::string &loggerName) override;

private:
	Params params_;
};
}