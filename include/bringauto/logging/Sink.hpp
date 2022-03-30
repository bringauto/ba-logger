#pragma once

#include <string>

namespace bringauto::logging {
    /**
     * Base class that all specific sink types will derive from
     */
    class Sink {
    public:
        /**
         * Specific initialization, file sink will be added to logger
         * @param programName name of program, logger uses same name
         */
        virtual void init(const std::string &programName) {};
    };
}