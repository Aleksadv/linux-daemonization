#pragma once
#include "Config.hpp"
#include <filesystem>

class ConfigLoader
{
public:
    static Config load(const std::filesystem::path& path);

private:
    static std::filesystem::path resolvePath(
        const std::filesystem::path& value,
        const std::filesystem::path& configDirectory
        );
};
