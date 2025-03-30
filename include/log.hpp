/*
 * Filename: /home/grant/src/test_c++/include/log.hpp
 * Path: /home/grant/src/test_c++/include
 * Created Date: Saturday, August 24th 2024, 6:15:13 am
 * Author: Grant Elliott
 *
 * Copyright (c) 2025 Grant Elliott
 */

#pragma once

#include <filesystem>
#include <format>
#include <source_location>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

namespace ca::grantelliott::log {

// Add options to
// - set log-level
// - log to stdout, file, etc
// - set custom logfile name
void initialize();

[[nodiscard]] constexpr auto
get_log_source_location(const std::source_location &location) {
  return spdlog::source_loc{location.file_name(),
                            static_cast<std::int32_t>(location.line()),
                            location.function_name()};
}

struct format_with_location {
  std::string_view value;
  spdlog::source_loc loc;

  template <typename String>
  format_with_location(const String &s, const std::source_location &location =
                                            std::source_location::current())
      : value{s}, loc{get_log_source_location(location)} {}
};

template <typename... Args>
void debug(format_with_location fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::debug, fmt.value,
                                    std::forward<Args>(args)...);
}

template <typename... Args>
void info(format_with_location fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::info, fmt.value,
                                    std::forward<Args>(args)...);
}

template <typename... Args>
void warn(format_with_location fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::warn, fmt.value,
                                    std::forward<Args>(args)...);
}

template <typename... Args>
void error(format_with_location fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::err, fmt.value,
                                    std::forward<Args>(args)...);
}

template <typename... Args>
void critical(format_with_location fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::critical, fmt.value,
                                    std::forward<Args>(args)...);
}

} // namespace ca::grantelliott::log
