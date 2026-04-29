#pragma once

#ifndef SHIRANET_LOG_LEVEL
#define SHIRANET_LOG_LEVEL 3
#endif

#include <functional>
#include <string>

namespace ShiraNet {

enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3
};

class Logger {
  public:
    const static LogLevel selectedLogLevel = static_cast<LogLevel>(SHIRANET_LOG_LEVEL);
    using LogCallback = std::function<void(LogLevel, const std::string &)>;

    static void setCallback(LogCallback callback);
    static void log(LogLevel level, const std::string &message);

    static void debug(const std::string &message);
    static void info(const std::string &message);
    static void warning(const std::string &message);
    static void error(const std::string &message);

  private:
    static LogCallback logCallback;
};

} // namespace ShiraNet
