#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>

class ConfigManager {
public:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    void parse(const std::string& configFilePath) {
        std::ifstream configFile(configFilePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("Unable to open config file: " + configFilePath);
        }
        configFile >> config;
        std::cout << "[ConfigManager] Config parsed successfully" << std::endl;
    }

    template<typename T>
    T getValue(const std::string& key) const {
        nlohmann::json::json_pointer ptr(key);
        return config.at(ptr).get<T>();
    }

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager() = default;
    ~ConfigManager() = default;

private:
    nlohmann::json config;
};

template int ConfigManager::getValue<int>(const std::string&) const;
template double ConfigManager::getValue<double>(const std::string&) const;
template std::string ConfigManager::getValue<std::string>(const std::string&) const;
template bool ConfigManager::getValue<bool>(const std::string&) const;
template float ConfigManager::getValue<float>(const std::string&) const;
