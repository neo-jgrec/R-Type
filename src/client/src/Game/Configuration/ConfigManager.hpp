#pragma once

#include <nlohmann/json.hpp>
#include <string>

class ConfigManager {
public:
    static ConfigManager& getInstance();

    void parse(const std::string& configFilePath);

    template<typename T>
    T getValue(const std::string& key) const;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager() = default;
    ~ConfigManager() = default;

private:

    nlohmann::json config;
};
