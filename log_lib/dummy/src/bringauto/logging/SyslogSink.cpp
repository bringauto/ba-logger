
#include <bringauto/logging/SyslogSink.hpp>

#include <iostream>



void bringauto::logging::SyslogSink::init(const std::string &loggerName) {
    std::cout<<"["<<loggerName<<"] SyslogSink was initialised." <<std::endl;
}
