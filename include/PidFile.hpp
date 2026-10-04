#pragma once

class PidFile
{
private:

   static constexpr const char* PATH = "/var/run/mydaemon.pid";
   int fd_ = -1;
   pid_t readPid() const;
   bool processExists(pid_t pid) const;

public:

    void terminatePreviousInstance();
    void acquireLock();
    void writeCurrentPid();
    void remove();
};

