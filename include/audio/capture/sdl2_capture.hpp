/*
 * Filename:
 * /home/grant/src/voice_assistant/include/audio/capture/sdl2_capture.hpp Path:
 * /home/grant/src/voice_assistant/include/audio/capture Created Date: Sunday,
 * March 30th 2025, 7:46:02 pm Author: Grant Elliott
 *
 * Copyright (c) 2025 Grant Elliott
 */

#pragma once

#include <SDL.h>
#include <SDL_audio.h>

#include <atomic>
#include <mutex>
#include <vector>

#include "audio/capture/sdl2_capture.hpp"

namespace ca::grantelliott::voice::audio::capture {

class Sdl2Capture {
 public:
    Sdl2Capture(const SDL_AudioDeviceID deviceId, const uint64_t captureLength, const uint32_t sampleRate);
    ~Sdl2Capture();

    void init();
    void start();
    void pause();
    void resume();
    void clear();
    void terminate();

    // callback to be called by SDL
    void callback(uint8_t* stream, int len);

    void getSamples(int ms, std::vector<float>& audio);

 private:
    SDL_AudioDeviceID mDeviceId;
    uint64_t mCaptureLength_ms;
    uint32_t mSampleRate;

    std::atomic_bool mRunning;
    std::mutex mMutex;

    std::vector<float> mAudio;
    size_t mAudioPos;
    size_t mAudioLen;
};

}  // namespace ca::gelliott::voice::audio::capture
