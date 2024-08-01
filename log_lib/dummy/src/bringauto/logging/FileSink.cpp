
#include <bringauto/logging/FileSink.hpp>

#include <iostream>



void bringauto::logging::FileSink::init(const std::string &loggerName) {
    std::cout<<"["<<loggerName<<"] FileSink was initialised." <<std::endl;
}
