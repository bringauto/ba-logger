#include <bringauto/logging/LoggerImpl.hpp>

#include <iostream>



void bringauto::logging::LoggerImpl::initLogger(const bringauto::logging::LoggerSettings &settings){
    std::cout<<"["<<settings.loggerName<<"] Logger was initialised." <<std::endl;
}

void bringauto::logging::LoggerImpl::logImplementation(bringauto::logging::LoggerVerbosity verbosity, std::string message, const std::string &loggerName){
    std::cout<<"["<<loggerName<<"] Logger logged a message." <<std::endl;

}

void bringauto::logging::LoggerImpl::logImplementation(bringauto::logging::LoggerVerbosity verbosity, char const * message, const std::string &loggerName){
    std::cout<<"["<<loggerName<<"] Logger logged a message." <<std::endl;

}

void bringauto::logging::LoggerImpl::destroyLogger(){
    std::cout<<"Logger was destroyed." <<std::endl;
}




