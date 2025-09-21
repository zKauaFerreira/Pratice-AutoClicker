#pragma once
#include <windows.h>

#include "platform/win/TrayIcon.h"
#include "core/ConfigManager.h"
#include "core/ClickHandler.h"
#include "platform/win/InputHook.h"
#include "platform/win/Notification.h"
#include "platform/win/FullscreenDetector.h"
#include "platform/win/Registry.h"

class AutoClicker {
public:
    AutoClicker();
    ~AutoClicker();

    bool Initialize(HINSTANCE hInstance);
    void Shutdown();

    // State management methods to be called from UI/hooks
    void ToggleEnabled();
    void SetCPS(int cps);
    bool IsEnabled() const;
    int GetCPS() const;

    ConfigManager* GetConfigManager() { return &m_configManager; }
    ClickHandler* GetClickHandler() { return &m_clickHandler; }
    FullscreenDetector* GetFullscreenDetector() { return &m_fullscreenDetector; }
    TrayIcon* GetTrayIcon() { return &m_trayIcon; }
    HINSTANCE GetInstance() { return m_hInstance; }
    HWND GetWindowHandle() { return m_hWnd; }

private:
    bool m_enabled;
    int m_cps;

    HINSTANCE m_hInstance;
    HWND m_hWnd;
    TrayIcon m_trayIcon;
    ConfigManager m_configManager;
    InputHook m_inputHook;
    ClickHandler m_clickHandler;
    Notification m_notification;
    FullscreenDetector m_fullscreenDetector;

    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    bool CreateMainWindow(HINSTANCE hInstance);
    void ShowChangeToggleKeyModal();
    void ShowCustomCPSModal();
};
