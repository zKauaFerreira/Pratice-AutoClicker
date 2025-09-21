#include "core/ClickHandler.h"
#include "core/AutoClicker.h"
#include <windows.h>

ClickHandler::ClickHandler(AutoClicker* app) 
    : m_app(app), m_running(false), m_m1_pressed(false), m_m2_pressed(false), 
      m_m4_spam_active(false), m_m5_spam_active(false) {}

ClickHandler::~ClickHandler() {
    StopClickerThread();
}

void ClickHandler::StartClickerThread() {
    if (!m_running) {
        m_running = true;
        m_clickerThread = std::thread(&ClickHandler::ClickerThread, this);
    }
}

void ClickHandler::StopClickerThread() {
    if (m_running) {
        m_running = false;
        m_cv.notify_one(); // Wake up the thread to exit
        if (m_clickerThread.joinable()) {
            m_clickerThread.join();
        }
    }
}

void ClickHandler::SetButtonState(MouseButton button, bool pressed) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        switch (button) {
            case MouseButton::M1:
                m_m1_pressed = pressed;
                break;
            case MouseButton::M2:
                m_m2_pressed = pressed;
                break;
        }
    }
    m_cv.notify_one();
}

void ClickHandler::ToggleSpamMode(MouseButton button) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (button == MouseButton::M4) {
            m_m4_spam_active = !m_m4_spam_active;
            if (m_m4_spam_active) { 
                m_m4_spam_start_time = std::chrono::steady_clock::now();
            }
        } else if (button == MouseButton::M5) {
            m_m5_spam_active = !m_m5_spam_active;
            if (m_m5_spam_active) {
                m_m5_spam_start_time = std::chrono::steady_clock::now();
            }
        }
    }
    m_cv.notify_one();
}

void ClickHandler::ClickerThread() {
    while (m_running) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] {
            return !m_running || (m_app && m_app->IsEnabled() && (m_m1_pressed || m_m2_pressed || m_m4_spam_active || m_m5_spam_active));
        });

        if (!m_running) {
            break;
        }

        ConfigManager* cfg = m_app->GetConfigManager();
        int cps = m_app->GetCPS();
        if (cps <= 0) cps = 1;
        auto delay = std::chrono::milliseconds(1000 / cps);

        json othersConfig = cfg->GetKey("Others");
        bool holdM1 = othersConfig.value("holdBehaviorM1", true);
        bool holdM2 = othersConfig.value("holdBehaviorM2", true);

        // The loop continues as long as a click condition is met
        while (m_running && m_app->IsEnabled()) {
            bool clicked = false;
            if (m_m4_spam_active) {
                int limit = othersConfig.value("m4LimitSeconds", 0);
                if (limit > 0 && (std::chrono::steady_clock::now() - m_m4_spam_start_time > std::chrono::seconds(limit))) {
                    m_m4_spam_active = false;
                } else {
                    PerformClick(MouseButton::M1);
                    clicked = true;
                }
            } else if (m_m5_spam_active) {
                int limit = othersConfig.value("m5LimitSeconds", 0);
                if (limit > 0 && (std::chrono::steady_clock::now() - m_m5_spam_start_time > std::chrono::seconds(limit))) {
                    m_m5_spam_active = false;
                } else {
                    PerformClick(MouseButton::M2);
                    clicked = true;
                }
            } else if (holdM1 && m_m1_pressed) {
                PerformClick(MouseButton::M1);
                clicked = true;
            } else if (holdM2 && m_m2_pressed) {
                PerformClick(MouseButton::M2);
                clicked = true;
            }

            if (clicked) {
                lock.unlock();
                std::this_thread::sleep_for(delay);
                lock.lock();
            } else {
                // No click condition is met, so break inner loop and wait on CV again
                break;
            }
        }
    }
}

#define APP_EXTRA_INFO 0xABCDE

void ClickHandler::PerformClick(MouseButton button) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    DWORD down_flag, up_flag;

    if (button == MouseButton::M1) {
        down_flag = MOUSEEVENTF_LEFTDOWN;
        up_flag = MOUSEEVENTF_LEFTUP;
    } else { // M2
        down_flag = MOUSEEVENTF_RIGHTDOWN;
        up_flag = MOUSEEVENTF_RIGHTUP;
    }

    // Send Mouse Down
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = down_flag;
    input.mi.dwExtraInfo = APP_EXTRA_INFO; // Mark as synthetic
    SendInput(1, &input, sizeof(INPUT));
    
    // Send Mouse Up
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = up_flag;
    input.mi.dwExtraInfo = APP_EXTRA_INFO; // Mark as synthetic
    SendInput(1, &input, sizeof(INPUT));
}
