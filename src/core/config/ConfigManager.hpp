#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>

class ConfigManager {
public:
    void parse(const std::string& configFilePath) {
        std::ifstream configFile(configFilePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("Unable to open config file: " + configFilePath);
        }
        configFile >> config;
        std::cout << "[ConfigManager] Config parsed successfully" << std::endl;
    }

    template<typename T>
    T getValue(const std::string& key, const T& defaultValue) const {
        try {
            nlohmann::json::json_pointer ptr(key);
            return config.at(ptr).get<T>();
        } catch (const std::exception&) {
            return defaultValue;
        }
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
