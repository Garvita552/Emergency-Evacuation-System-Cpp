#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Futura {

enum class LogLevel {
    INFO,
    WARNING,
    CRITICAL,
    SENSOR
};

struct LogMessage {
    std::string timestamp;
    LogLevel level;
    std::string message;
};

class Logger {
private:
    std::vector<LogMessage> logs;
    std::mutex logMutex;
    size_t maxLogs = 500;

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string& msg) {
        std::lock_guard<std::mutex> lock(logMutex);
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::puttime(std::localtime(&in_time_t), "%H:%M:%S");

        logs.push_back({ss.str(), level, msg});
        if (logs.size() > maxLogs) {
            logs.erase(logs.begin());
        }
    }

    std::vector<LogMessage> getRecentLogs(size_t count = 50) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logs.size() <= count) return logs;
        return std::vector<LogMessage>(logs.end() - count, logs.end());
    }

    void clear() {
        std::lock_guard<std::mutex> lock(logMutex);
        logs.clear();
    }
};

} // namespace Futura

#endif // LOGGER_HPP
