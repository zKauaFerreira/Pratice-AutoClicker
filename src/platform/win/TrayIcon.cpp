#include "platform/win/TrayIcon.h"
#include "core/AutoClicker.h"
#include "resource.h" // For icon ID

TrayIcon::TrayIcon(AutoClicker* app) : m_hWnd(NULL), m_hInstance(NULL), m_app(app) {
    ZeroMemory(&m_nid, sizeof(m_nid));
}

TrayIcon::~TrayIcon() {
    Destroy();
}

bool TrayIcon::Create(HWND hWnd, HINSTANCE hInstance) {
    m_hWnd = hWnd;
    m_hInstance = hInstance;

    m_nid.cbSize = sizeof(NOTIFYICONDATAW);
    m_nid.hWnd = hWnd;
    m_nid.uID = 1;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_TRAY_ICON;
    m_nid.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 
                                GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    wcscpy_s(m_nid.szTip, L"AutoClicker");

    return Shell_NotifyIconW(NIM_ADD, &m_nid);
}

void TrayIcon::Destroy() {
    Shell_NotifyIconW(NIM_DELETE, &m_nid);
}

void TrayIcon::ShowNotification(const std::wstring& title, const std::wstring& text) {
    m_nid.uFlags = NIF_INFO;
    m_nid.dwInfoFlags = NIIF_INFO | NIIF_NOSOUND;
    wcscpy_s(m_nid.szInfoTitle, title.c_str());
    wcscpy_s(m_nid.szInfo, text.c_str());

    Shell_NotifyIconW(NIM_MODIFY, &m_nid);

    // Reset flags
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
}

LRESULT TrayIcon::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wParam);
    if (lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP) {
        ShowContextMenu();
    }
    return 0;
}

void TrayIcon::ShowContextMenu() {
    POINT pt;
    GetCursorPos(&pt);

    HMENU hMenu = CreatePopupMenu();
    HMENU hSubMenuCPS = CreatePopupMenu();
    HMENU hSubMenuOther = CreatePopupMenu(); // Renamed from hSubMenuSettings
    HMENU hSubMenuNotifications = CreatePopupMenu(); // New submenu

    HMENU hSubMenuNotifyCorner = CreatePopupMenu();
    AppendMenuW(hSubMenuNotifyCorner, MF_STRING, ID_MENU_NOTIFY_TL, L"Top-Left");
    AppendMenuW(hSubMenuNotifyCorner, MF_STRING, ID_MENU_NOTIFY_TR, L"Top-Right");
    AppendMenuW(hSubMenuNotifyCorner, MF_STRING, ID_MENU_NOTIFY_BL, L"Bottom-Left");
    AppendMenuW(hSubMenuNotifyCorner, MF_STRING, ID_MENU_NOTIFY_BR, L"Bottom-Right");

    HMENU hSubMenuNotifyDuration = CreatePopupMenu();
    AppendMenuW(hSubMenuNotifyDuration, MF_STRING, ID_MENU_NOTIFY_DUR_1S, L"1 Second");
    AppendMenuW(hSubMenuNotifyDuration, MF_STRING, ID_MENU_NOTIFY_DUR_3S, L"3 Seconds");
    AppendMenuW(hSubMenuNotifyDuration, MF_STRING, ID_MENU_NOTIFY_DUR_5S, L"5 Seconds");
    AppendMenuW(hSubMenuNotifyDuration, MF_STRING, ID_MENU_NOTIFY_DUR_10S, L"10 Seconds");

    // Build Notifications SubMenu
    AppendMenuW(hSubMenuNotifications, MF_STRING, ID_MENU_TOGGLE_NOTIFICATIONS, L"Enable Notifications");
    AppendMenuW(hSubMenuNotifications, MF_STRING, ID_MENU_TOGGLE_FULLSCREEN_SILENCE, L"Silence on Fullscreen");
    AppendMenuW(hSubMenuNotifications, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hSubMenuNotifications, MF_POPUP, (UINT_PTR)hSubMenuNotifyCorner, L"Notification Corner");
    AppendMenuW(hSubMenuNotifications, MF_POPUP, (UINT_PTR)hSubMenuNotifyDuration, L"Notification Duration");

    // Build Other SubMenu (previously Settings)
    AppendMenuW(hSubMenuOther, MF_STRING, ID_MENU_CHANGE_TOGGLE_KEY, L"Change Toggle Key");
    AppendMenuW(hSubMenuOther, MF_STRING, ID_MENU_TOGGLE_START_WITH_WINDOWS, L"Start with Windows");

    // Build CPS SubMenu
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_1, L"1 CPS");
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_5, L"5 CPS");
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_10, L"10 CPS");
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_20, L"20 CPS");
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_50, L"50 CPS");
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_100, L"100 CPS");
    AppendMenuW(hSubMenuCPS, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hSubMenuCPS, MF_STRING, ID_MENU_CPS_CUSTOM, L"Custom...");

    // Build Main Menu
    AppendMenuW(hMenu, MF_STRING, ID_MENU_TOGGLE_ENABLED, L"Enabled");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenuCPS, L"Set CPS");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenuNotifications, L"Notifications"); // Added
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenuOther, L"Other"); // Renamed
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_MENU_RESET_CONFIG, L"Reset Settings");
    AppendMenuW(hMenu, MF_STRING, ID_MENU_EXIT, L"Exit");

    // Set initial menu state (checkmarks, etc.)
    UpdateMenuState(hMenu);

    // Show menu
    SetForegroundWindow(m_hWnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
    PostMessage(m_hWnd, WM_NULL, 0, 0);

    DestroyMenu(hMenu); // Submenus are destroyed automatically
}

void TrayIcon::UpdateMenuState(HMENU menu) {
    if (m_app) {
        // Update Enabled/Disabled checkmark
        CheckMenuItem(menu, ID_MENU_TOGGLE_ENABLED, m_app->IsEnabled() ? MF_CHECKED : MF_UNCHECKED);

        // Update CPS checkmark
        HMENU hSubMenuCPS = GetSubMenu(menu, 2);
        int currentCPS = m_app->GetCPS();
        int menuIdToCheck = -1;
        bool isCustom = true;

        if (currentCPS == 1) { menuIdToCheck = ID_MENU_CPS_1; isCustom = false; }
        else if (currentCPS == 5) { menuIdToCheck = ID_MENU_CPS_5; isCustom = false; }
        else if (currentCPS == 10) { menuIdToCheck = ID_MENU_CPS_10; isCustom = false; }
        else if (currentCPS == 20) { menuIdToCheck = ID_MENU_CPS_20; isCustom = false; }
        else if (currentCPS == 50) { menuIdToCheck = ID_MENU_CPS_50; isCustom = false; }
        else if (currentCPS == 100) { menuIdToCheck = ID_MENU_CPS_100; isCustom = false; }

        if (isCustom) {
            std::wstring customText = L"Custom: " + std::to_wstring(currentCPS) + L" CPS";
            ModifyMenuW(hSubMenuCPS, ID_MENU_CPS_CUSTOM, MF_BYCOMMAND | MF_STRING, ID_MENU_CPS_CUSTOM, customText.c_str());
            menuIdToCheck = ID_MENU_CPS_CUSTOM;
        }
        
        CheckMenuRadioItem(hSubMenuCPS, ID_MENU_CPS_1, ID_MENU_CPS_CUSTOM, menuIdToCheck, MF_BYCOMMAND);

        // Update settings checkmarks
        ConfigManager* cfg = m_app->GetConfigManager();

        HMENU hSubMenuNotifications = GetSubMenu(menu, 3);
        HMENU hSubMenuOther = GetSubMenu(menu, 4);

        CheckMenuItem(hSubMenuNotifications, ID_MENU_TOGGLE_NOTIFICATIONS, cfg->GetKey("Notifications").value("notificationsEnabled", true) ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hSubMenuNotifications, ID_MENU_TOGGLE_FULLSCREEN_SILENCE, cfg->GetKey("Notifications").value("silenceNotificationsOnFullscreen", true) ? MF_CHECKED : MF_UNCHECKED);
        
        std::string corner = cfg->GetKey("Notifications").value("notifyCorner", "top-left");
        int cornerId = ID_MENU_NOTIFY_TL;
        if (corner == "top-right") cornerId = ID_MENU_NOTIFY_TR;
        else if (corner == "bottom-left") cornerId = ID_MENU_NOTIFY_BL;
        else if (corner == "bottom-right") cornerId = ID_MENU_NOTIFY_BR;
        CheckMenuRadioItem(GetSubMenu(hSubMenuNotifications, 3), ID_MENU_NOTIFY_TL, ID_MENU_NOTIFY_BR, cornerId, MF_BYCOMMAND);

        int duration = cfg->GetKey("Notifications").value("notifyDurationSeconds", 3);
        int durationId = ID_MENU_NOTIFY_DUR_3S;
        if (duration == 1) durationId = ID_MENU_NOTIFY_DUR_1S;
        else if (duration == 5) durationId = ID_MENU_NOTIFY_DUR_5S;
        else if (duration == 10) durationId = ID_MENU_NOTIFY_DUR_10S;
        CheckMenuRadioItem(GetSubMenu(hSubMenuNotifications, 4), ID_MENU_NOTIFY_DUR_1S, ID_MENU_NOTIFY_DUR_10S, durationId, MF_BYCOMMAND);

        bool startWithWindows = Registry::IsStartWithWindowsEnabled(L"AutoClicker");
        CheckMenuItem(hSubMenuOther, ID_MENU_TOGGLE_START_WITH_WINDOWS, startWithWindows ? MF_CHECKED : MF_UNCHECKED);
    }
}
