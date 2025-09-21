#include "Registry.h"

namespace {
    const wchar_t* RUN_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
}

bool Registry::SetStartWithWindows(const std::wstring& appName, bool enabled) {
    HKEY hKey = NULL;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, RUN_KEY, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    bool success = false;
    if (enabled) {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        if (RegSetValueExW(hKey, appName.c_str(), 0, REG_SZ, (const BYTE*)path, static_cast<DWORD>((wcslen(path) + 1) * sizeof(wchar_t))) == ERROR_SUCCESS) {
            success = true;
        }
    } else {
        if (RegDeleteValueW(hKey, appName.c_str()) == ERROR_SUCCESS) {
            success = true;
        }
    }

    RegCloseKey(hKey);
    return success;
}

bool Registry::IsStartWithWindowsEnabled(const std::wstring& appName) {
    HKEY hKey = NULL;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, RUN_KEY, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    bool enabled = (RegQueryValueExW(hKey, appName.c_str(), NULL, NULL, NULL, NULL) == ERROR_SUCCESS);

    RegCloseKey(hKey);
    return enabled;
}
