cmake_minimum_required(VERSION 3.20)

project(AutoClicker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Windows-specific settings
if(WIN32)
    set(CMAKE_WINDOWS_RTC_DEFAULT "JUMPOUT")
    add_definitions(-D_UNICODE -DUNICODE -D_WIN32_WINNT=0x0600)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
    # Enable console for debugging, will be changed to WIN32 for release
    set(EXECUTABLE_TYPE WIN32)
endif()

# Add executable
add_executable(AutoClicker ${EXECUTABLE_TYPE}
    src/main.cpp
    src/core/AutoClicker.cpp
    src/core/ClickHandler.cpp
    src/core/ConfigManager.cpp
    src/platform/win/InputHook.cpp
    src/platform/win/TrayIcon.cpp
    src/platform/win/Notification.cpp
    src/platform/win/FullscreenDetector.cpp
    src/platform/win/CustomNotification.cpp
    src/utils/Logger.cpp
    src/platform/win/Registry.cpp
    src/platform/win/ToggleKeyModal.cpp
    src/platform/win/CustomCPSModal.cpp
    res/resource.rc
)

# Include directories
target_include_directories(AutoClicker PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
    "${CMAKE_CURRENT_SOURCE_DIR}/res"
)

# Link libraries for Windows
if(WIN32)
    target_link_libraries(AutoClicker PRIVATE user32.lib shell32.lib gdi32.lib comctl32.lib)
endif()

# --- Installation ---
# This part will be configured later with NSIS script details
# install(TARGETS AutoClicker DESTINATION bin)
