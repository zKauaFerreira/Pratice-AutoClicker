#pragma once
#include <string>

class AutoClicker; // Forward declaration

class Notification {
public:
    Notification(AutoClicker* app);
    void Show(const std::wstring& text, bool enabled);

private:
    AutoClicker* m_app;
};
