#pragma once
#include <windows.h>
#include <string>

#define WM_TRAY_ICON (WM_USER + 1)

// Menu Item IDs
#define ID_MENU_EXIT 1001
#define ID_MENU_TOGGLE_ENABLED 1002
#define ID_MENU_RESET_CONFIG 1003

#define ID_MENU_CPS_SUBMENU 1100
#define ID_MENU_CPS_1 1101
#define ID_MENU_CPS_5 1102
#define ID_MENU_CPS_10 1103
#define ID_MENU_CPS_20 1104
#define ID_MENU_CPS_50 1105
#define ID_MENU_CPS_100 1106
#define ID_MENU_CPS_CUSTOM 1107

#define ID_MENU_SETTINGS_SUBMENU 1200
#define ID_MENU_TOGGLE_NOTIFICATIONS 1201
#define ID_MENU_TOGGLE_FULLSCREEN_SILENCE 1202
#define ID_MENU_TOGGLE_START_WITH_WINDOWS 1203
#define ID_MENU_CHANGE_TOGGLE_KEY 1204

#define ID_MENU_NOTIFY_CORNER_SUBMENU 1300
#define ID_MENU_NOTIFY_TL 1301
#define ID_MENU_NOTIFY_TR 1302
#define ID_MENU_NOTIFY_BL 1303
#define ID_MENU_NOTIFY_BR 1304

#define ID_MENU_NOTIFY_DURATION_SUBMENU 1400
#define ID_MENU_NOTIFY_DUR_1S 1401
#define ID_MENU_NOTIFY_DUR_3S 1402
#define ID_MENU_NOTIFY_DUR_5S 1403
#define ID_MENU_NOTIFY_DUR_10S 1404

class AutoClicker; // Forward declaration

class TrayIcon {
public:
    TrayIcon(AutoClicker* app);
    ~TrayIcon();

    bool Create(HWND hWnd, HINSTANCE hInstance);
    void Destroy();
    void ShowNotification(const std::wstring& title, const std::wstring& text);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    void ShowContextMenu();
    void UpdateMenuState(HMENU menu);

    HWND m_hWnd;
    HINSTANCE m_hInstance;
    NOTIFYICONDATAW m_nid;
    AutoClicker* m_app; // Pointer to the main app logic
};
