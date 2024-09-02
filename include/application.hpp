/*
 * Filename: /home/grant/src/test_c++/include/application.hpp
 * Path: /home/grant/src/test_c++/include
 * Created Date: Thursday, August 15th 2024, 2:07:23 am
 * Author: Grant Elliott
 *
 * Copyright (c) 2024 Grant Elliott
 */

#pragma once
#include <atomic>

namespace ca::grantelliott::voice {
class Application {
 public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void init();
    void start();
    void terminate() noexcept;

 private:
    std::atomic<bool> mRun;
};
}  // namespace ca::grantelliott::voice
