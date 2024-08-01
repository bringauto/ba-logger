
#include <bringauto/logging/RemoteSink.hpp>

#include <iostream>



void bringauto::logging::RemoteSink::init(const std::string &loggerName) {
    std::cout<<"["<<loggerName<<"] RemoteSink was initialised." <<std::endl;
}
