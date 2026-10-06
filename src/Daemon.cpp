#include "Daemon.hpp"
#include "ConfigLoader.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <syslog.h>

Daemon& Daemon::getInstance(){
    static Daemon instance;
    return instance;
}

int Daemon::run(const std::filesystem::path& configArgument){

    initializeConfiguration(configArgument);

    pidFile_.acquireLock();

    daemonize();

    openLog(); // открываем системный журнал

    try {

        redirectStandardStreams();
        
        pidFile_.writeCurrentPid();
        //handler
        //mainLoop();
    }
    catch (std::exception& e){

        syslog(LOG_ERR, "Fatal daemon error: %s", e.what());

        pidFile_.release();
        closelog();

        return EXIT_FAILURE;

        }
       
    //shutdown()
    return EXIT_SUCCESS;
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
        throw std::runtime_error(
            std::string("fork failed:")
            + std::strerror(errno));
        
    }

    if(pid > 0){
        //Старый процесс
        _exit(EXIT_SUCCESS);
    }

    //Задать демону новую группу процессов и сеанс, в котором демон будет ведущим процессом
    if(setsid() < 0){

      throw std::runtime_error(
            std::string("setsid failed:")
            + std::strerror(errno));
         
    }
    //Изменить рабочую директорию на root
    if(chdir("/") == -1){

        throw std::runtime_error(
            std::string("chdir failed:")
            + std::strerror(errno));
        
    }

}

void Daemon::openLog(){
    // открываем системный журнал
    openlog("my_daemon",LOG_PID,LOG_DAEMON);
    syslog(LOG_INFO, "Daemon successfully started. PID: %d", static_cast<int>(getpid()));
}

void Daemon::redirectStandardStreams(){

    //открываем файловый дескриптор для перенаправления
    int nullFd = open("/dev/null", O_RDWR);

    if(nullFd == -1){
       throw std::runtime_error(
            std::string("open /dev/null failed: ")
            + std::strerror(errno));
         
    }

    // dup2 автоматически закрывает старый дескриптор
    if(dup2(nullFd, STDIN_FILENO)== -1){
        const int err = errno;
        if(nullFd > STDERR_FILENO) close(nullFd);
        
        throw std::runtime_error(
            std::string("dup2 stdin failed: ")
            +std::strerror(err)
        );

    }
    if(dup2(nullFd, STDOUT_FILENO)== -1){
        const int err = errno;
        if(nullFd > STDERR_FILENO) close(nullFd);
        
        throw std::runtime_error(
            std::string("dup2 stdout failed: ")
            +std::strerror(err)
        );
    }
    if(dup2(nullFd, STDERR_FILENO)== -1){
        const int err = errno;
        if(nullFd > STDERR_FILENO) close(nullFd);
        
        throw std::runtime_error(
            std::string("dup2 stderr failed: ")
            +std::strerror(err)
        );
    }

    if(nullFd > STDERR_FILENO) close(nullFd);

}
