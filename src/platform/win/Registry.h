#pragma once
#include <windows.h>
#include <string>

namespace Registry {
    bool SetStartWithWindows(const std::wstring& appName, bool enabled);
    bool IsStartWithWindowsEnabled(const std::wstring& appName);
}
