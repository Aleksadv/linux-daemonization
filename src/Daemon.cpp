#include "Daemon.hpp"

Daemon& Daemon::getInstance(){
    static Daemon instance;
    return instance;
}

int Daemon::run(const std::filesystem::path& configArgument){
    
}