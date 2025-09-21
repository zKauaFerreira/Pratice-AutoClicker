#include "core/ConfigManager.h"
#include <windows.h>
#include <shlobj.h>
#include <fstream>
#include <filesystem>

// Forward declaration for a helper function to convert wstring to string
std::string WStringToString(const std::wstring& wstr);

void ConfigManager::InitializePath() {
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
    if (SUCCEEDED(hr)) {
        m_configPath = std::wstring(path) + L"\\AutoClicker";
        CoTaskMemFree(path);
    } else {
        // Fallback or error handling
        // For simplicity, we'll just use the current directory, but this should be logged.
        m_configPath = L"."; 
    }
}

void ConfigManager::Load() {
    if (m_configPath.empty()) {
        InitializePath();
    }

    std::filesystem::path fullPath = std::filesystem::path(m_configPath) / "config.json";

    if (!std::filesystem::exists(fullPath)) {
        ResetToDefaults();
        return;
    }

    std::ifstream file(fullPath);
    try {
        file >> m_config;
    } catch (json::parse_error&) {
        // Log the error and reset to defaults
        // Logger::Log(LogLevel::ERROR, "Failed to parse config.json: " + std::string(e.what()));
        ResetToDefaults();
    }
}

void ConfigManager::Save() {
    if (m_configPath.empty()) {
        InitializePath();
    }

    try {
        if (!std::filesystem::exists(m_configPath)) {
            std::filesystem::create_directories(m_configPath);
        }
        std::filesystem::path fullPath = std::filesystem::path(m_configPath) / "config.json";
        std::ofstream file(fullPath);
        file << m_config.dump(4); // Pretty print with 4 spaces
    } catch (const std::filesystem::filesystem_error&) {
        // Logger::Log(LogLevel::ERROR, "Filesystem error while saving config: " + std::string(e.what()));
    }
}

void ConfigManager::ResetToDefaults() {
    m_config = {
        {"enabled", false},
        {"cps", 20},
        {"toggleKey", VK_END}, 

        {"Notifications", {
            {"notificationsEnabled", true},
            {"notifyCorner", "top-left"},
            {"notifyDurationSeconds", 3},
            {"silenceNotificationsOnFullscreen", true},
            {"notifyTextOn", "AutoClicker ON"},
            {"notifyTextOff", "AutoClicker OFF"}
        }},

        {"Others", {
            {"holdBehaviorM1", true},
            {"holdBehaviorM2", true},
            {"m4LimitSeconds", 0},
            {"m5LimitSeconds", 0},
            {"startWithWindows", false},
            {"killSwitchKey", VK_END}
        }},

        {"ClickerOptions", {
            {"mouse1Enabled", true},
            {"mouse2Enabled", true},
            {"mouse4Enabled", true},
            {"mouse5Enabled", true}
        }}
    };
    Save();
}



// Helper implementation
std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}
