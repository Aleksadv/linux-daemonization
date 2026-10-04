#include "Config.hpp"
#include "ConfigLoader.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

Config ConfigLoader::load(const std::filesystem::path& path){

    std::ifstream file(path);
    if(!file.is_open()){
        throw std::runtime_error("Cannot open config file");
    }
    
    Config config;
    bool sourceFound = false;
    bool destinationFound = false;
    bool intervalFound = false;

    const std::filesystem::path configPath = path.parent_path();

    std::string line;
    while (std::getline(file, line))
    {
        if(line.empty()) {
            continue;
        }

        const auto separator = line.find('=');

        if(separator == std::string::npos) {
            throw std::runtime_error("Invalid config line");
        }

        const std::string key = line.substr(0,separator);
        const std::string value = line.substr(separator+1);

        if(key == "source") {

            config.sourceDirectory = resolvePath(value, configPath);
            sourceFound = true;

            }else if(key == "destination") {

                config.destinationDirectory = resolvePath(value, configPath);
                destinationFound = true;

            }else if(key == "interval") {

                int seconds = std::stoi(value);
                if(seconds <= 0){
                    throw std::runtime_error("Interval must be positive");
                }
                config.interval = std::chrono::seconds(seconds);
                intervalFound = true;

            }else {
                throw std::runtime_error("Unknown config option: " + key);
            }
        
    }
    if(!sourceFound || !destinationFound || !intervalFound){
            throw std::runtime_error("Config does not contain all required options");
        }
    return config;
}

std::filesystem::path ConfigLoader::resolvePath(
        const std::filesystem::path& value,
        const std::filesystem::path& configDirectory){
    
    if(value.is_absolute()){
        return value.lexically_normal();
    }

    return (configDirectory / value).lexically_normal();


}