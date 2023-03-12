#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <spdlog-src/include/spdlog/spdlog.h>
#include <spdlog-src/include/spdlog/common.h>
#include <spdlog-src/include/spdlog/sinks/ansicolor_sink.h>
#include <spdlog-src/include/spdlog/sinks/stdout_color_sinks.h>
#include <spdlog-src/include/spdlog/spdlog.h>
#include <spdlog-src/include/spdlog/sinks/stdout_sinks.h>
#include <spdlog-src/include/spdlog/sinks/base_sink.h>
#include <spdlog-src/include/spdlog/details/os.h>
#include <memory>

#define LOGGING_ENABLED

#ifdef LOGGING_ENABLED
  #define FF_LOG_DEBUG(str, ...) SPDLOG_LOGGER_CALL(FF::Logger::Get().spdlog_logger, spdlog::level::debug, str, ##__VA_ARGS__)
  #define FF_LOG_INFO(str, ...) SPDLOG_LOGGER_CALL(FF::Logger::Get().spdlog_logger, spdlog::level::info, str, ##__VA_ARGS__)
  #define FF_LOG_WARN(str, ...) SPDLOG_LOGGER_CALL(FF::Logger::Get().spdlog_logger, spdlog::level::warn, str, ##__VA_ARGS__)
  #define FF_LOG_ERROR(str, ...) SPDLOG_LOGGER_CALL(FF::Logger::Get().spdlog_logger, spdlog::level::err, str, ##__VA_ARGS__)
  #define FF_LOG_CRIT(str, ...) SPDLOG_LOGGER_CALL(FF::Logger::Get().spdlog_logger, spdlog::level::critical, str, ##__VA_ARGS__)
#else
  #define FF_LOG_DEBUG(str, ...) {}
  #define FF_LOG_INFO(str, ...) {}
  #define FF_LOG_WARN(str, ...) {}
  #define FF_LOG_ERROR(str, ...) {}
  #define FF_LOG_CRIT(str, ...) {}
#endif

namespace FF {
class Logger {
public:
  static Logger& Get() {
    static Logger logger;
    return logger;
  }
  
// Making these private for now
private:
  template <typename ...Args>
  static void Info(const std::string& msg, Args&&... args) {
    SPDLOG_LOGGER_CALL(Get().spdlog_logger, spdlog::level::info, fmt::runtime(msg), std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Warn(const std::string& msg, Args&&... args) {
    SPDLOG_LOGGER_CALL(Get().spdlog_logger, spdlog::level::warn, fmt::runtime(msg), std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Debug(const std::string& msg, Args&&... args) {
    SPDLOG_LOGGER_CALL(Get().spdlog_logger, spdlog::level::debug, fmt::runtime(msg), std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Error(const std::string& msg, Args&&... args) {
    SPDLOG_LOGGER_CALL(Get().spdlog_logger, spdlog::level::err, fmt::runtime(msg), std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Critical(const std::string& msg, Args&&... args) {
    SPDLOG_LOGGER_CALL(Get().spdlog_logger, spdlog::level::critical, fmt::runtime(msg), std::forward<Args>(args)...);
  }

private:
  Logger();
  ~Logger();

public:
  std::shared_ptr<spdlog::logger> spdlog_logger;
};
}

#endif