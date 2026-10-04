#include "Daemon.hpp"
#include "ConfigLoader.hpp"
#include <iostream>

Daemon& Daemon::getInstance(){
    static Daemon instance;
    return instance;
}

int Daemon::run(const std::filesystem::path& configArgument){

    configPath_ = std::filesystem::canonical(configArgument);
    config_ = ConfigLoader::load(configPath_);
    
    return 0;
}