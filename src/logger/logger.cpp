#include "logger.hpp"

#include <iostream>

namespace ShiraNet {

    Logger::LogCallback Logger::logCallback = [](LogLevel level, const std::string& message) {
        const char* levelStr = "";
        switch (level) {
            case LogLevel::Debug:   levelStr = "\033[32m" "[DEBUG]"   "\033[0m"; break;
            case LogLevel::Info:    levelStr = "\033[93m" "[INFO]"    "\033[0m"; break;
            case LogLevel::Warning: levelStr = "\033[33m" "[WARNING]" "\033[0m"; break;
            case LogLevel::Error:   levelStr = "\033[91m" "[ERROR]"   "\033[0m"; break;
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
