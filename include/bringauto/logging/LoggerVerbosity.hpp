
#pragma once

namespace bringauto::logging {
    /***
	 * Log message verbosity enum
	 *
	 * Debug        - debugging messages (i.e. memory dump)
	 * Info         - actual state of program (i.e. module initialized)
	 * Warning      - condition that might cause a problem in the future but program can ignore it (i.e. missing parameter and using default one)
	 * Error        - error occurs but program is able to continue (i.e. failed thread)
	 * Critical     - program is unable to continue (i.e. segmentation fault)
	 * LowestLevel  - contains the lowest verbosity level
	 * HighestLevel - contains the highest verbosity level
	 */
	enum class LoggerVerbosity: int {
		Debug = 0,
		Info,
		Warning,
		Error,
		Critical,
		LowestLevel = Debug,
		HighestLevel = Critical,
	};
    }