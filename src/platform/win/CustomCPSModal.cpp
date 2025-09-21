#include "platform/win/CustomCPSModal.h"
#include "core/AutoClicker.h"
#include <windows.h>
#include <string>

// Control IDs
#define IDC_CPS_INPUT 201
#define IDC_OK_BUTTON 202
#define IDC_CANCEL_BUTTON 203

static const wchar_t MODAL_CLASS_NAME[] = L"CustomCPSModalClass";
static AutoClicker* g_app_ptr = nullptr;

// Forward declaration of the window procedure
LRESULT CALLBACK CustomCPSModalProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ShowCustomCPSModal(HWND parent, HINSTANCE hInstance, AutoClicker* app) {
    g_app_ptr = app;

    WNDCLASSW wc = {};
    if (!GetClassInfoW(hInstance, MODAL_CLASS_NAME, &wc)) {
        wc.lpfnWndProc = CustomCPSModalProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = MODAL_CLASS_NAME;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);
    }

    int width = 300, height = 170;
    RECT parentRect;
    GetWindowRect(parent, &parentRect);
    int x = parentRect.left + (parentRect.right - parentRect.left - width) / 2;
    int y = parentRect.top + (parentRect.bottom - parentRect.top - height) / 2;

    HWND hWnd = CreateWindowExW(
        WS_EX_DLGMODALFRAME,
        MODAL_CLASS_NAME, L"Set Custom CPS",
        WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        x, y, width, height,
        parent, NULL, hInstance, NULL
    );

    if (!hWnd) return;

    EnableWindow(parent, FALSE);

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

LRESULT CALLBACK CustomCPSModalProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CreateWindowW(L"STATIC", L"Enter CPS value (1-1000):", WS_CHILD | WS_VISIBLE, 10, 20, 280, 20, hWnd, NULL, NULL, NULL);
            
            std::wstring currentCPS = std::to_wstring(g_app_ptr->GetCPS());
            CreateWindowW(L"EDIT", currentCPS.c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 80, 50, 140, 25, hWnd, (HMENU)IDC_CPS_INPUT, NULL, NULL);
            
            CreateWindowW(L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 60, 90, 80, 30, hWnd, (HMENU)IDC_OK_BUTTON, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE, 160, 90, 80, 30, hWnd, (HMENU)IDC_CANCEL_BUTTON, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_OK_BUTTON: {
                    wchar_t buffer[10];
                    GetDlgItemTextW(hWnd, IDC_CPS_INPUT, buffer, 10);
                    int cps = _wtoi(buffer);

                    if (cps >= 1 && cps <= 1000) {
                        if (g_app_ptr) {
                            g_app_ptr->SetCPS(cps);
                        }
                        DestroyWindow(hWnd);
                    } else {
                        MessageBoxW(hWnd, L"Invalid CPS value. Please enter a number between 1 and 1000.", L"Error", MB_OK | MB_ICONERROR);
                    }
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
            // PostQuitMessage(0); // This was causing the entire app to close
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
