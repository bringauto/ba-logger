
#include <bringauto/logging/ConsoleSink.hpp>

#include <iostream>



void bringauto::logging::ConsoleSink::init(const std::string &loggerName) {
    std::cout<<"["<<loggerName<<"] ConsoleSink was initialised." <<std::endl;
}
