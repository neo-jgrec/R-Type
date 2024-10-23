#include "ConfigManager.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

ConfigManager& ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

void ConfigManager::parse(const std::string& configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open config file: " + configFilePath);
    }
    configFile >> config;
    std::cout << "[ConfigManager] Config parsed successfully" << std::endl;
}

template<typename T>
T ConfigManager::getValue(const std::string& key) const
{
    nlohmann::json::json_pointer ptr(key);
    return config.at(ptr).get<T>();
}

template int ConfigManager::getValue<int>(const std::string&) const;
template double ConfigManager::getValue<double>(const std::string&) const;
template std::string ConfigManager::getValue<std::string>(const std::string&) const;
template bool ConfigManager::getValue<bool>(const std::string&) const;
template float ConfigManager::getValue<float>(const std::string&) const;
