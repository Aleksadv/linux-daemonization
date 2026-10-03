#pragma once

#include <filesystem>
#include <chrono>

struct Config{

    std::filesystem::path sourceDirectory;
    std::filesystem::path destinationDirectory;

    std::chrono::seconds interval{30};
};