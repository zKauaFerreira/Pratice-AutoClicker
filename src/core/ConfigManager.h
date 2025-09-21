#pragma once
#include "nlohmann/json.hpp"
#include <string>
#include <optional>

using json = nlohmann::json;

class ConfigManager {
public:
    void Load();
    void Save();
    void ResetToDefaults();

    // Gets a value from the config, returns default if not found
    template<typename T>
    T Get(const std::string& key, T defaultValue) {
        if (m_config.contains(key)) {
            return m_config.at(key).get<T>();
        }
        return defaultValue;
    }

    // Sets a value in the config
    template<typename T>
    void Set(const std::string& key, T value) {
        m_config[key] = value;
    }

    // Returns a mutable reference to a top-level key
    json& GetKey(const std::string& key) {
        return m_config[key];
    }

private:
    json m_config;
    std::wstring m_configPath;

    void InitializePath();
};
