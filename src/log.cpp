/*
 * Filename: /home/grant/src/test_c++/src/log.cpp
 * Path: /home/grant/src/test_c++/src
 * Created Date: Saturday, August 24th 2024, 8:00:22 am
 * Author: Grant Elliott
 *
 * Copyright (c) 2024 Grant Elliott
 */

#include "log.hpp"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace ca::grantelliott::log {

void initialize() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

}  // namespace ca::grantelliott::log
