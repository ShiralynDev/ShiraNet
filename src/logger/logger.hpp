#pragma once

#include <functional>
#include <string>

namespace ShiraNet {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    class Logger {
        public:
            using LogCallback = std::function<void(LogLevel, const std::string&)>;

            static void setCallback(LogCallback callback);
            static void log(LogLevel level, const std::string& message);
            
            static void debug(const std::string& message);
            static void info(const std::string& message);
            static void warning(const std::string& message);
            static void error(const std::string& message);

        private:
            static LogCallback logCallback;
    };

}
