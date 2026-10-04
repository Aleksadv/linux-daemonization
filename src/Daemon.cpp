#include "Daemon.hpp"
#include "ConfigLoader.hpp"
#include <iostream>

Daemon& Daemon::getInstance(){
    static Daemon instance;
    return instance;
}

int Daemon::run(const std::filesystem::path& configArgument){

    initializeConfiguration(configArgument);

    pidFile_.acquireLock();

    //daemonize();
    //openLog()
    pidFile_.writeCurrentPid();
    //handler
    //mainLoop();
    //shutdown()
    return 0;
}

void Daemon::initializeConfiguration(const std::filesystem::path& configArgument){
    configPath_ = std::filesystem::canonical(configArgument);
    config_ = ConfigLoader::load(configPath_);
}

