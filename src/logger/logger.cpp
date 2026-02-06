#include "logger.hpp"

#include <iostream>

namespace ShiraNet {

    Logger::LogCallback Logger::logCallback = [](LogLevel level, const std::string& message) {
        const char* levelStr = "";
        switch (level) {
            case LogLevel::Debug:   levelStr = "[DEBUG]"; break;
            case LogLevel::Info:    levelStr = "[INFO]"; break;
            case LogLevel::Warning: levelStr = "[WARNING]"; break;
            case LogLevel::Error:   levelStr = "[ERROR]"; break;
        }
        std::cerr << levelStr << " " << message << std::endl;
    };

    void Logger::setCallback(LogCallback callback) {
        logCallback = callback;
    }

    void Logger::log(LogLevel level, const std::string& message) {
        if (logCallback) {
            logCallback(level, message);
        }
    }

    void Logger::debug(const std::string& message) {
        log(LogLevel::Debug, message);
    }

    void Logger::info(const std::string& message) {
        log(LogLevel::Info, message);
    }

    void Logger::warning(const std::string& message) {
        log(LogLevel::Warning, message);
    }

    void Logger::error(const std::string& message) {
        log(LogLevel::Error, message);
    }

}
