#pragma once
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class AutoClicker; // Forward declaration

// Enum for mouse buttons
enum class MouseButton {
    M1, // Left
    M2, // Right
    M4, // XButton1
    M5  // XButton2
};

class ClickHandler {
public:
    ClickHandler(AutoClicker* app);
    ~ClickHandler();

    void StartClickerThread();
    void StopClickerThread();

    // Called by the InputHook
    void SetButtonState(MouseButton button, bool pressed);
    void ToggleSpamMode(MouseButton button);
    void NotifyStateChange();

private:
    void ClickerThread();
    void PerformClick(MouseButton button);

    AutoClicker* m_app;
    std::thread m_clickerThread;
    std::atomic<bool> m_running;
    
    // Thread synchronization
    std::mutex m_mutex;
    std::condition_variable m_cv;

    // State variables protected by the mutex
    bool m_m1_pressed;
    bool m_m2_pressed;
    bool m_m4_spam_active;
    bool m_m5_spam_active;

    // For timed spam
    std::chrono::steady_clock::time_point m_m4_spam_start_time;
    std::chrono::steady_clock::time_point m_m5_spam_start_time;
};