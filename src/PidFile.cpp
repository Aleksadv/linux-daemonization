#include "PidFile.hpp"

#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <string>
#include <cstring>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <sys/file.h>

void PidFile::acquireLock(){
    if(fd_ != -1){
        throw std::runtime_error("PID file is already opened");
    }

    //Открытие файла (или создание)
    int fd_ = open(PidFile::PATH, O_RDWR | O_CREAT | O_CLOEXEC, 0644);

    if(fd_ == -1) {
        throw std::runtime_error(
            std::string("PID file could not be opened or created: ")
            + std::strerror(errno)
            );
    }

    //Попытка установить эксклюзивную блокировку
    if (flock(fd_, LOCK_EX | LOCK_NB) == 0){
        //Успешно получен lock
        return; 
    }

    //flock() с ошибкой
    if (errno != EWOULDBLOCK){
        int error = errno;
        close(fd_);
        fd_ = -1;
        throw std::runtime_error(
            std::string("Cannot lock PID file: ")
            + std::strerror(error)
            );
    }

    //Файл заблокирован другим процессом errno == EWOULDBLOCK
    
    //Завершаем предыдущий процесс
    terminatePreviousInstance();

    if (flock(fd_, LOCK_EX) == -1){
        int error = errno;
        close(fd_);
        fd_ = -1;
        throw std::runtime_error(
            std::string("Cannot acquire PID lock: ")
            + std::strerror(error)
            );
    }

    //Успешно получен lock
}
void PidFile::terminatePreviousInstance(){
    //Получение pid
    const pid_t pid = readPid();

    if (pid <= 0){
        throw std::runtime_error(
            "PID file is locked, but does not contain a valid PID"
        );
    }
    //Проверка существования процесса
    if (!processExists(pid)){
        throw std::runtime_error(
            "PID file is locked, but process does not exist"
        );
    }
    //Попытка завершить процесс
    if (kill(pid, SIGTERM) == -1){
        if(errno == ESRCH){
            return; // Процесс завершился между проверкой /proc и kill()
        }

        throw std::runtime_error(
            std::string("Cannot send SIGTERM")
            + std::strerror(errno)
        );
        
    }

}

void PidFile::writeCurrentPid(){

}

void PidFile::remove(){

}

pid_t PidFile::readPid() const{

    if(fd_ == -1){
        throw std::runtime_error("PID file is not opened");
    }
    //Чтение файла
    lseek(fd_, 0, SEEK_SET);

    char buffer[128];
    ssize_t bytes_read = read(fd_, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        //Прочитанный PID из файла:  buffer 
        long v = std::strtol(buffer, nullptr, 10);
        if(v > 0){
            return static_cast<pid_t>(v);
        }

    } else if (bytes_read == 0) {
        // Файл пуст (новый процесс)
        return -1;
    } else {
        // Ошибка чтения файла
        throw std::runtime_error(
            std::string("Cannot read PID file:")
            + std::strerror(errno)
        );
    }

}
