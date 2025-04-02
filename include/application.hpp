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
#include <memory>

#include "audio/capture/sdl2_capture.hpp"

namespace ca::grantelliott::voice {

/**
 * @brief The main application class
 *
 * It should prepare the audioin, ASR, LLM, TTS, audioout and connect them
 *
 * asr {audioIn, Whisper}
 * tts {Piper, audioOut}
 * llm {inputText}
 *
 * manage high-level threading
 */
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
    audio::capture::Sdl2Capture mAudioCapture;
};
}  // namespace ca::grantelliott::voice
