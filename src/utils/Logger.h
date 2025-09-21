#pragma once
#include <string>

enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static void Init();
    static void Log([[maybe_unused]] LogLevel level, [[maybe_unused]] const std::string& message);
};
