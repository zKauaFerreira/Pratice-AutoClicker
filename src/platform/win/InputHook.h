#pragma once
#include <windows.h>

class AutoClicker; // Forward declaration

class InputHook {
public:
    InputHook();
    void Install(AutoClicker* app);
    void Uninstall();

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    HHOOK m_keyboardHook;
    HHOOK m_mouseHook;
    static AutoClicker* s_app; // Static pointer to the main application instance
};
