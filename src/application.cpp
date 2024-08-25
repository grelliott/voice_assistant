/*
 * Filename: /home/grant/src/test_c++/src/application.cpp
 * Path: /home/grant/src/test_c++/src
 * Created Date: Thursday, August 15th 2024, 2:07:30 am
 * Author: Grant Elliott
 *
 * Copyright (c) 2024 Grant Elliott
 */

#include "application.hpp"

#include <chrono>
#include <cstddef>
#include <thread>

#include "log.hpp"

namespace ca::grantelliott::testapp {

using namespace std::literals::chrono_literals;

static constexpr auto NUMBER_OF_LOOPS = 1 * 10;

Application::Application() : mRun{true} {
    log::info("Creating Application");
}

Application::~Application() {
    log::info("Destroying Application");
}

void Application::init() {
    log::info("Application::init");
}

void Application::start() {
    log::info("Application::start");
    std::thread worker_thread{[&]() {
        for (std::size_t i = 0; i < NUMBER_OF_LOOPS; i++) {
            log::info(".");
            std::this_thread::sleep_for(250ms);
        }
    }};
    worker_thread.join();
}

void Application::terminate() noexcept {
    log::info("Application::terminate");
    mRun.store(false);
}

}  // namespace ca::grantelliott::testapp
