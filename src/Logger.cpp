#include "../include/Logger.hpp"

namespace FF {
Logger::Logger()
: spdlog_logger(spdlog::stdout_color_mt("console")) {}

Logger::~Logger() {}
}