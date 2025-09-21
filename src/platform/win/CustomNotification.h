#pragma once
#include <string>
#include <windows.h>

class ConfigManager; // Forward declaration

class CustomNotification {
public:
    static void Show(HINSTANCE hInstance, ConfigManager* config, const std::wstring& text, bool enabled);

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static const wchar_t CLASS_NAME[];
};
