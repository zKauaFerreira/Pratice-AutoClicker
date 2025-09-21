#include "platform/win/Notification.h"
#include "core/AutoClicker.h"
#include "platform/win/CustomNotification.h"
#include "platform/win/FullscreenDetector.h"

Notification::Notification(AutoClicker* app) : m_app(app) {}

void Notification::Show(const std::wstring& text, bool enabled) {
    if (!m_app) return;

    ConfigManager* cfg = m_app->GetConfigManager();

    if (!cfg->GetKey("Notifications").value("notificationsEnabled", true)) {
        return; // Notifications are globally disabled
    }

    // The FullscreenDetector might not be available in all contexts, check for it.
    FullscreenDetector* fullscreenDetector = m_app->GetFullscreenDetector();
    if (fullscreenDetector && cfg->GetKey("Notifications").value("silenceNotificationsOnFullscreen", true)) {
        if (fullscreenDetector->IsForegroundFullscreen()) {
            return; // Silence notifications on fullscreen
        }
    }

    // If we passed all checks, show the custom notification
    CustomNotification::Show(m_app->GetInstance(), cfg, text, enabled);
}
