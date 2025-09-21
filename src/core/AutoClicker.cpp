#include "core/AutoClicker.h"
#include "platform/win/ToggleKeyModal.h"

// Helper to convert UTF-8 std::string to std::wstring
std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// Define a unique name for the window class
const wchar_t CLASS_NAME[] = L"AutoClickerHiddenWindowClass";

AutoClicker::AutoClicker()
    : m_enabled(true), m_cps(20), m_hInstance(NULL), m_hWnd(NULL), 
      m_trayIcon(this), m_clickHandler(this), m_notification(this) {}

AutoClicker::~AutoClicker() {
    Shutdown();
}

bool AutoClicker::Initialize(HINSTANCE hInstance) {
    m_hInstance = hInstance;

    // Load configuration first
    m_configManager.Load();
    m_enabled = m_configManager.Get<bool>("enabled", true);
    m_cps = m_configManager.Get<int>("cps", 20);

    if (!CreateMainWindow(hInstance)) {
        return false;
    }

    if (!m_trayIcon.Create(m_hWnd, hInstance)) {
        return false;
    }

    m_inputHook.Install(this);
    m_clickHandler.StartClickerThread();

    return true;
}

void AutoClicker::Shutdown() {
    m_configManager.Save(); // Save config on exit
    m_clickHandler.StopClickerThread();
    m_inputHook.Uninstall();
    m_trayIcon.Destroy();
    if (m_hWnd) {
        DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
    UnregisterClass(CLASS_NAME, m_hInstance);
}

void AutoClicker::ToggleEnabled() {
    m_enabled = !m_enabled;
    m_configManager.Set("enabled", m_enabled);
    // m_configManager.Save(); // Removed to prevent lag on toggle

    std::string text = m_enabled ? "AutoClicker ON" : "AutoClicker OFF";

    m_notification.Show(StringToWString(text), m_enabled);
}

void AutoClicker::SetCPS(int cps) {
    m_cps = cps;
    m_configManager.Set("cps", m_cps);
    m_configManager.Save();
}

bool AutoClicker::IsEnabled() const {
    return m_enabled;
}

int AutoClicker::GetCPS() const {
    return m_cps;
}

LRESULT CALLBACK AutoClicker::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    AutoClicker* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (AutoClicker*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    } else {
        pThis = (AutoClicker*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis) {
        // Let the tray icon handle its message
        if (msg == WM_TRAY_ICON) {
            return pThis->m_trayIcon.HandleMessage(msg, wParam, lParam);
        }

        switch (msg) {
            case WM_COMMAND: {
                int wmId = LOWORD(wParam);
                ConfigManager* cfg = pThis->GetConfigManager();
                // Handle menu item clicks
                switch (wmId) {
                    case ID_MENU_TOGGLE_ENABLED:
                        pThis->ToggleEnabled();
                        break;
                    case ID_MENU_CPS_1:
                    case ID_MENU_CPS_5:
                    case ID_MENU_CPS_10:
                    case ID_MENU_CPS_20:
                    case ID_MENU_CPS_50:
                    case ID_MENU_CPS_100: {
                        int cps[] = {1, 5, 10, 20, 50, 100};
                        pThis->SetCPS(cps[wmId - ID_MENU_CPS_1]);
                        break;
                    }
                    case ID_MENU_CPS_CUSTOM:
                        pThis->ShowCustomCPSModal();
                        break;
                case ID_MENU_TOGGLE_NOTIFICATIONS: {
                    bool enabled = cfg->GetKey("Notifications").value("notificationsEnabled", true);
                    cfg->GetKey("Notifications")["notificationsEnabled"] = !enabled;
                    cfg->Save();
                    break;
                }
                case ID_MENU_TOGGLE_FULLSCREEN_SILENCE: {
                    bool enabled = cfg->GetKey("Notifications").value("silenceNotificationsOnFullscreen", true);
                    cfg->GetKey("Notifications")["silenceNotificationsOnFullscreen"] = !enabled;
                    cfg->Save();
                    break;
                }
                case ID_MENU_NOTIFY_TL:
                    cfg->GetKey("Notifications")["notifyCorner"] = "top-left";
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_TR:
                    cfg->GetKey("Notifications")["notifyCorner"] = "top-right";
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_BL:
                    cfg->GetKey("Notifications")["notifyCorner"] = "bottom-left";
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_BR:
                    cfg->GetKey("Notifications")["notifyCorner"] = "bottom-right";
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_DUR_1S:
                    cfg->GetKey("Notifications")["notifyDurationSeconds"] = 1;
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_DUR_3S:
                    cfg->GetKey("Notifications")["notifyDurationSeconds"] = 3;
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_DUR_5S:
                    cfg->GetKey("Notifications")["notifyDurationSeconds"] = 5;
                    cfg->Save();
                    break;
                case ID_MENU_NOTIFY_DUR_10S:
                    cfg->GetKey("Notifications")["notifyDurationSeconds"] = 10;
                    cfg->Save();
                    break;
                case ID_MENU_TOGGLE_START_WITH_WINDOWS: {
                    bool isEnabled = Registry::IsStartWithWindowsEnabled(L"AutoClicker");
                    Registry::SetStartWithWindows(L"AutoClicker", !isEnabled);
                    cfg->GetKey("Others")["startWithWindows"] = !isEnabled;
                    cfg->Save();
                    break;
                }                    case ID_MENU_RESET_CONFIG:
                        pThis->m_configManager.ResetToDefaults();
                        // You might want to reload settings in the app after reset
                        break;
                    case ID_MENU_CHANGE_TOGGLE_KEY:
                        pThis->ShowChangeToggleKeyModal();
                        break;
                    case ID_MENU_EXIT:
                        PostQuitMessage(0);
                        break;
                }
                return 0;
            }
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#include "platform/win/ToggleKeyModal.h"

void AutoClicker::ShowChangeToggleKeyModal() {
    m_inputHook.Uninstall();
    ::ShowToggleKeyModal(m_hWnd, m_hInstance, this);
    m_inputHook.Install(this);
}


#include "platform/win/CustomCPSModal.h"

void AutoClicker::ShowCustomCPSModal() {
    ::ShowCustomCPSModal(m_hWnd, m_hInstance, this);
}

bool AutoClicker::CreateMainWindow(HINSTANCE hInstance) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = AutoClicker::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassW(&wc)) {
        return false;
    }

    m_hWnd = CreateWindowExW(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"AutoClicker Hidden Window",   // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        this        // Additional application data
    );

    return (m_hWnd != NULL);
}
