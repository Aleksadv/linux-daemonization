#include "Config.hpp"
#include "PidFile.hpp"

#include <filesystem>

class Daemon
{
private:
    Daemon() = default;
    
    void initializeConfiguration(const std::filesystem::path& configArgument);
    void reloadConfig();
    void mainLoop();
    void daemonize();
    void openLog();
    void redirectStandardStreams();
    static void signalHandler(int signal);
    void shutdown();

    std::filesystem::path configPath_;
    std::filesystem::path pidPath_;
    Config config_;
    PidFile pidFile_;

public:
    Daemon(const Daemon&) = delete;
    Daemon(Daemon&&) = delete;
    Daemon& operator=(const Daemon&) = delete;
    Daemon& operator=(Daemon&&) = delete;

    static Daemon& getInstance();
    int run(const std::filesystem::path& configArgument);
};


