#include "platform/win/FullscreenDetector.h"
#include <windows.h>

bool FullscreenDetector::IsForegroundFullscreen() {
    HWND foregroundWindow = GetForegroundWindow();
    if (!foregroundWindow) {
        return false;
    }

    RECT windowRect;
    GetWindowRect(foregroundWindow, &windowRect);

    HMONITOR monitor = MonitorFromWindow(foregroundWindow, MONITOR_DEFAULTTONEAREST);
    if (!monitor) {
        return false;
    }

    MONITORINFO monitorInfo = { sizeof(monitorInfo) };
    if (!GetMonitorInfo(monitor, &monitorInfo)) {
        return false;
    }

    // Check if the window rect is equal to the monitor rect
    return windowRect.left == monitorInfo.rcMonitor.left &&
           windowRect.top == monitorInfo.rcMonitor.top &&
           windowRect.right == monitorInfo.rcMonitor.right &&
           windowRect.bottom == monitorInfo.rcMonitor.bottom;
}
