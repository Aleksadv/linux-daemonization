#include "Daemon.hpp"
#include "ConfigLoader.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

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

void Daemon::daemonize(){

    //Порождение процесса
    pid_t pid = fork();

    if(pid == -1){
        //fork error
        exit(EXIT_FAILURE);
    }

    if(pid != 0){
        //Старый процесс
        exit(EXIT_SUCCESS);
    }

    //Задать демону новую группу процессов и сеанс, в котором демон будет ведущим процессом
    if(setsid() < 0){
       exit(EXIT_FAILURE); 
    }
    //Изменить рабочую директорию на root
    chdir("/");
    
    //открываем файловый дескриптор для перенаправления
    int nullFd = open("/dev/null", O_RDWR);
    if(nullFd < 0){
        exit(EXIT_FAILURE);
    }

    // dup2 автоматически закрывает старый дескриптор
    if(dup2(nullFd, STDIN_FILENO)== -1){
        //error
    }
    if(dup2(nullFd, STDOUT_FILENO)== -1){
        //error
    }
    if(dup2(nullFd, STDERR_FILENO)== -1){
        //error
    }

    close(nullFd);
}

