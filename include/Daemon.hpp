#include <filesystem>

class Daemon
{
private:
    Daemon() = default;
    
    void mainLoop();
    void daemonize();
    static void signalHandler(int signal);
    void shutdown();

    std::filesystem::path configPath_;
    std::filesystem::path pidPath_;


public:
    Daemon(const Daemon&) = delete;
    Daemon(Daemon&&) = delete;
    Daemon& operator=(const Daemon&) = delete;
    Daemon& operator=(Daemon&&) = delete;

    static Daemon& getInstance();
    int run(const std::filesystem::path& configArgument);
};


