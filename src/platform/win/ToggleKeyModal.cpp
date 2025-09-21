#include "platform/win/ToggleKeyModal.h"
#include "core/AutoClicker.h"
#include <windows.h>
#include <string>
#include <CommCtrl.h> // For SetWindowSubclass

#pragma comment(lib, "comctl32.lib")

// Control and message IDs
#define IDC_KEY_INPUT 101
#define IDC_OK_BUTTON 102
#define IDC_CANCEL_BUTTON 103
#define WM_APP_KEY_CAPTURED (WM_APP + 1)

// Static variables for the modal
static const wchar_t MODAL_CLASS_NAME[] = L"ToggleKeyModalClass";
static DWORD g_capturedVkCode = 0;
static LPARAM g_capturedLParam = 0;
static AutoClicker* g_app_ptr = nullptr;

// Forward declarations
LRESULT CALLBACK ToggleKeyModalProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
std::wstring GetKeyNameFromEvent(DWORD vkCode, LPARAM lParam);

// Subclass procedure for the EDIT control to capture key events
LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    switch (uMsg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            // Forward the message to the parent dialog
            PostMessage(GetParent(hWnd), WM_APP_KEY_CAPTURED, wParam, lParam);
            return 0; // We handled it
        }
        case WM_GETDLGCODE: {
            return DLGC_WANTALLKEYS;
        }
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Gets the name of a key from its virtual key code and lParam
std::wstring GetKeyNameFromEvent(DWORD vkCode, LPARAM lParam) {
    switch (vkCode) {
        case VK_SHIFT: return L"Shift";
        case VK_CONTROL: return L"Control";
        case VK_MENU: return L"Alt";
        case VK_PAUSE: return L"Pause";
        case VK_CAPITAL: return L"Caps Lock";
        case VK_ESCAPE: return L"Escape";
        case VK_SPACE: return L"Space";
        case VK_PRIOR: return L"Page Up";
        case VK_NEXT: return L"Page Down";
        case VK_END: return L"End";
        case VK_HOME: return L"Home";
        case VK_LEFT: return L"Left Arrow";
        case VK_UP: return L"Up Arrow";
        case VK_RIGHT: return L"Right Arrow";
        case VK_DOWN: return L"Down Arrow";
        case VK_INSERT: return L"Insert";
        case VK_DELETE: return L"Delete";
    }

    wchar_t keyName[256] = {0};
    if (GetKeyNameTextW((LONG)lParam, keyName, sizeof(keyName) / sizeof(keyName[0])) > 0) {
        return std::wstring(keyName);
    }

    // Fallback for keys GetKeyNameText might miss (like numpad keys without numlock)
    if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
        return L"NumPad " + std::to_wstring(vkCode - VK_NUMPAD0);
    }

    return L"Unknown Key";
}

// Main function to create and show the modal dialog
void ShowToggleKeyModal(HWND parent, HINSTANCE hInstance, AutoClicker* app) {
    g_app_ptr = app;
    g_capturedVkCode = g_app_ptr->GetConfigManager()->Get<int>("toggleKey", VK_END);
    g_capturedLParam = (MapVirtualKey(g_capturedVkCode, MAPVK_VK_TO_VSC) << 16);

    WNDCLASSW wc = {};
    if (!GetClassInfoW(hInstance, MODAL_CLASS_NAME, &wc)) {
        wc.lpfnWndProc = ToggleKeyModalProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = MODAL_CLASS_NAME;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);
    }

    int width = 300, height = 150;
    RECT parentRect;
    GetWindowRect(parent, &parentRect);
    int x = parentRect.left + (parentRect.right - parentRect.left - width) / 2;
    int y = parentRect.top + (parentRect.bottom - parentRect.top - height) / 2;

    HWND hWnd = CreateWindowExW(
        WS_EX_DLGMODALFRAME,
        MODAL_CLASS_NAME, L"Change Toggle Key",
        WS_POPUPWINDOW | WS_CAPTION | DS_MODALFRAME,
        x, y, width, height,
        parent, NULL, hInstance, NULL
    );

    if (!hWnd) return;

    EnableWindow(parent, FALSE);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (IsWindow(hWnd) && GetMessage(&msg, NULL, 0, 0) > 0) {
        if (!IsDialogMessage(hWnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    EnableWindow(parent, TRUE);
    SetForegroundWindow(parent);
}

// Main window procedure for the modal dialog
LRESULT CALLBACK ToggleKeyModalProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CreateWindowW(L"STATIC", L"Press any key:", WS_CHILD | WS_VISIBLE, 10, 10, 280, 20, hWnd, NULL, NULL, NULL);
            
            HWND hEdit = CreateWindowW(L"EDIT", GetKeyNameFromEvent(g_capturedVkCode, g_capturedLParam).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 80, 40, 140, 25, hWnd, (HMENU)IDC_KEY_INPUT, NULL, NULL);
            SetWindowSubclass(hEdit, EditSubclassProc, 0, 0);

            CreateWindowW(L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 60, 80, 80, 30, hWnd, (HMENU)IDC_OK_BUTTON, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE, 160, 80, 80, 30, hWnd, (HMENU)IDC_CANCEL_BUTTON, NULL, NULL);
            
            SetFocus(hEdit); // Set focus to the edit box to ensure it gets key events
            break;
        }
        case WM_APP_KEY_CAPTURED: {
            g_capturedVkCode = (DWORD)wParam;
            g_capturedLParam = (LPARAM)lParam;
            SetDlgItemTextW(hWnd, IDC_KEY_INPUT, GetKeyNameFromEvent(g_capturedVkCode, g_capturedLParam).c_str());
            return 0;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_OK_BUTTON: {
                    if (g_app_ptr && g_capturedVkCode != 0) {
                        g_app_ptr->GetConfigManager()->Set("toggleKey", g_capturedVkCode);
                        g_app_ptr->GetConfigManager()->Save();
                    }
                    DestroyWindow(hWnd);
                    break;
                }
                case IDC_CANCEL_BUTTON: {
                    DestroyWindow(hWnd);
                    break;
                }
            }
            break;
        }
        case WM_CLOSE: {
            DestroyWindow(hWnd);
            break;
        }
        case WM_DESTROY: {
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}