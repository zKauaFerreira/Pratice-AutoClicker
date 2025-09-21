#include "platform/win/CustomNotification.h"
#include "core/ConfigManager.h"
#include <windowsx.h>
#include <thread>

const wchar_t CustomNotification::CLASS_NAME[] = L"CustomNotificationClass";
static HWND g_notificationWnd = NULL;

struct WindowData {
    std::wstring text;
    ConfigManager* config;
    COLORREF backgroundColor;
};

// Helper function for fade-out
void FadeOutAndDestroy(HWND hWnd) {
    for (int alpha = 230; alpha >= 0; alpha -= 10) {
        SetLayeredWindowAttributes(hWnd, 0, (BYTE)alpha, LWA_ALPHA);
        Sleep(15);
    }
    DestroyWindow(hWnd);
}

void CustomNotification::Show(HINSTANCE hInstance, ConfigManager* config, const std::wstring& text, bool enabled) {
    std::thread([=]() {
        if (g_notificationWnd) {
            PostMessage(g_notificationWnd, WM_CLOSE, 0, 0);
            Sleep(50); // Give time for the window to process the message
        }

        WNDCLASSW wc = {};
        if (!GetClassInfoW(hInstance, CLASS_NAME, &wc)) {
            wc.lpfnWndProc   = CustomNotification::WndProc;
            wc.hInstance     = hInstance;
            wc.lpszClassName = CLASS_NAME;
            wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
            if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
                return;
            }
        }

        HDC hdc = GetDC(NULL);
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SelectObject(hdc, hFont);
        RECT textRect = {0, 0, 0, 0};
        DrawTextW(hdc, text.c_str(), -1, &textRect, DT_CALCRECT | DT_SINGLELINE);
        ReleaseDC(NULL, hdc);

        int padding = 15;
        int windowWidth = textRect.right - textRect.left + 2 * padding;
        int windowHeight = textRect.bottom - textRect.top + 2 * padding;

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int screenPadding = 20;

        std::string corner = config->GetKey("Notifications").value("notifyCorner", "bottom-right");
        int x, y;

        if (corner == "top-left") { x = screenPadding; y = screenPadding; }
        else if (corner == "top-right") { x = screenWidth - windowWidth - screenPadding; y = screenPadding; }
        else if (corner == "bottom-left") { x = screenPadding; y = screenHeight - windowHeight - screenPadding; }
        else { x = screenWidth - windowWidth - screenPadding; y = screenHeight - windowHeight - screenPadding; }

        COLORREF bgColor = enabled ? RGB(0, 100, 0) : RGB(139, 0, 0); // Dark Green / Dark Red
        WindowData* pData = new WindowData{text, config, bgColor};

        HWND hWnd = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
            CLASS_NAME, L"Notification", WS_POPUP,
            x, y, windowWidth, windowHeight,
            NULL, NULL, hInstance, pData
        );

        if (hWnd) {
            g_notificationWnd = hWnd;
            SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);
            ShowWindow(hWnd, SW_SHOWNOACTIVATE);
            UpdateWindow(hWnd);

            for (int alpha = 0; alpha <= 230; alpha += 10) {
                SetLayeredWindowAttributes(hWnd, 0, (BYTE)alpha, LWA_ALPHA);
                Sleep(15);
            }

            int duration = config->GetKey("Notifications").value("notifyDurationSeconds", 3);
            Sleep(duration * 1000);
            FadeOutAndDestroy(hWnd);
        }
    }).detach();
}

LRESULT CALLBACK CustomNotification::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowData* pData = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (msg) {
        case WM_NCCREATE: {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pData = (WindowData*)pCreate->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pData);
            return TRUE;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            HBRUSH hBrush = CreateSolidBrush(pData->backgroundColor);
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkMode(hdc, TRANSPARENT);
            HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SelectObject(hdc, hFont);
            DrawTextW(hdc, pData->text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_CLOSE: {
            DestroyWindow(hWnd);
            break;
        }
        case WM_DESTROY: {
            if (g_notificationWnd == hWnd) {
                g_notificationWnd = NULL;
            }
            delete pData;
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}