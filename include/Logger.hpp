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

namespace FF {
class Logger {
public:
  static Logger& Get() {
    static Logger logger;
    return logger;
  }
  
  template <typename ...Args>
  static void Info(const std::string& msg, Args&&... args) {
    Get().spdlog_logger.get()->info(msg, std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Warn(const std::string& msg, Args&&... args) {
    Get().spdlog_logger.get()->warn(msg, std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Debug(const std::string& msg, Args&&... args) {
    Get().spdlog_logger.get()->debug(msg, std::forward<Args>(args)...);
  }

  template <typename ...Args>
  static void Error(const std::string& msg, Args&&... args) {
    Get().spdlog_logger.get()->error(msg, std::forward<Args>(args)...);
  }

private:
  Logger();
  ~Logger();

private:
  std::shared_ptr<spdlog::logger> spdlog_logger;
};
}

#endif