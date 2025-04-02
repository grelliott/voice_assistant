/*
 * Filename: /home/grant/src/voice_assistant/src/audio/capture/sdl2_capture.cpp
 * Path: /home/grant/src/voice_assistant/src/audio/capture
 * Created Date: Sunday, March 30th 2025, 7:56:44 pm
 * Author: Grant Elliott
 *
 * Copyright (c) 2025 Grant Elliott
 */

#include "audio/capture/sdl2_capture.hpp"

#include <SDL.h>
#include <SDL_audio.h>

#include "log.hpp"

namespace ca::grantelliott::voice::audio::capture {
Sdl2Capture::Sdl2Capture(const SDL_AudioDeviceID deviceId,
                         const uint64_t captureLength,
                         const uint32_t sampleRate)
    : mCaptureLength_ms{captureLength},
      mRunning{false},
      mAudioPos{0},
      mAudioLen{0} {
    log::info("Creating Sdl2Capture instance");
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize SDL: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL");
    }

    SDL_SetHintWithPriority(SDL_HINT_AUDIO_RESAMPLING_MODE, "medium",
                            SDL_HINT_OVERRIDE);

    // Debug output
    {
        int nDevices = SDL_GetNumAudioDevices(SDL_TRUE);
        std::stringstream ss;
        ss << "Found " << nDevices << " capture devices: \n";

        for (int i = 0; i < nDevices; i++) {
            ss << "\t - Capture device #" << i << ": '"
               << SDL_GetAudioDeviceName(i, SDL_TRUE) << "'\n";
        }
        log::debug(ss.str());
    }

    SDL_AudioSpec capture_spec_requested;
    SDL_AudioSpec capture_spec_obtained;

    SDL_zero(capture_spec_requested);
    SDL_zero(capture_spec_obtained);

    capture_spec_requested.freq = sampleRate;
    capture_spec_requested.format = AUDIO_F32;
    capture_spec_requested.channels = 1;
    capture_spec_requested.samples = 1024;
    capture_spec_requested.callback = [](void* userdata, uint8_t* stream,
                                         int len) {
        Sdl2Capture* audio = (Sdl2Capture*)userdata;
        audio->callback(stream, len);
    };
    capture_spec_requested.userdata = this;

    if (deviceId >= 0) {
        log::info("attempt to open capture device %d: %s", deviceId,
                  SDL_GetAudioDeviceName(deviceId, SDL_TRUE));
        mDeviceId = SDL_OpenAudioDevice(
            SDL_GetAudioDeviceName(deviceId, SDL_TRUE), SDL_TRUE,
            &capture_spec_requested, &capture_spec_obtained, 0);
    } else {
        log::info("attempt to open default capture device ...");
        mDeviceId =
            SDL_OpenAudioDevice(nullptr, SDL_TRUE, &capture_spec_requested,
                                &capture_spec_obtained, 0);
    }

    if (!mDeviceId) {
        throw std::runtime_error(
            std::string{"Failed to open audio device for capture: "} +
            SDL_GetError());
    } else {
        std::stringstream ss;
        ss << "obtained spec for input device (SDL Id = " << mDeviceId
           << "):\n";
        ss << "\t - sample rate:       " << capture_spec_obtained.freq << "\n";
        ss << "\t - format:            " << capture_spec_obtained.format
           << " (required: " << capture_spec_obtained.format << ")\n";
        ss << "\t - channels:          " << capture_spec_obtained.channels
           << " (required: " << capture_spec_requested.channels << ")\n";
        ss << "\t - samples per frame: " << capture_spec_obtained.samples
           << "\n";
        log::debug(ss.str());
    }

    mSampleRate = capture_spec_obtained.freq;

    mAudio.resize((mSampleRate * mCaptureLength_ms) / 1000);
}

Sdl2Capture::~Sdl2Capture() {
    terminate();
    SDL_CloseAudioDevice(mDeviceId);
}

// TODO move to constructor
void Sdl2Capture::init() {
    log::debug("+init");
}

void Sdl2Capture::start() {
    if (!mRunning) {
        resume();
    }
}

void Sdl2Capture::pause() {
    if (!mRunning) {
        log::warn("already paused");
    }

    SDL_PauseAudioDevice(mDeviceId, 1);

    mRunning = false;
}

void Sdl2Capture::resume() {
    if (mRunning) {
        log::warn("already running");
    }

    SDL_PauseAudioDevice(mDeviceId, 0);

    mRunning = true;
}

void Sdl2Capture::clear() {
    if (!mRunning) {
        log::warn("not running");
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);

        mAudioPos = 0;
        mAudioLen = 0;
    }
}

void Sdl2Capture::terminate() {
    log::debug("+terminate");
    if (mRunning) {
        pause();
    }
}

// callback to be called by SDL
void Sdl2Capture::callback(uint8_t* stream, int len) {
    if (!mRunning) {
        return;
    }

    size_t n_samples = len / sizeof(float);

    if (n_samples > mAudio.size()) {
        n_samples = mAudio.size();

        stream += (len - (n_samples * sizeof(float)));
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (mAudioPos + n_samples > mAudio.size()) {
            const size_t n0 = mAudio.size() - mAudioPos;

            memcpy(&mAudio[mAudioPos], stream, n0 * sizeof(float));
            memcpy(&mAudio[0], stream + n0 * sizeof(float),
                   (n_samples - n0) * sizeof(float));
        } else {
            memcpy(&mAudio[mAudioPos], stream, n_samples * sizeof(float));
        }
        mAudioPos = (mAudioPos + n_samples) % mAudio.size();
        mAudioLen = std::min(mAudioLen + n_samples, mAudio.size());
    }
}

void Sdl2Capture::getSamples(int ms, std::vector<float>& audio) {
    // TODO likely consider removing this?
    if (!mRunning) {
        log::warn("not running");
        return;
    }

    audio.clear();

    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (ms <= 0) {
            ms = mCaptureLength_ms;
        }

        size_t n_samples = (mSampleRate * ms) / 1000;
        if (n_samples > mAudioLen) {
            n_samples = mAudioLen;
        }

        audio.resize(n_samples);

        int s0 = mAudioPos - n_samples;
        if (s0 < 0) {
            s0 += mAudio.size();
        }

        if (s0 + n_samples > mAudio.size()) {
            const size_t n0 = mAudio.size() - s0;

            memcpy(audio.data(), &mAudio[s0], n0 * sizeof(float));
            memcpy(&audio[n0], &mAudio[0], (n_samples - n0) * sizeof(float));
        } else {
            memcpy(audio.data(), &mAudio[s0], n_samples * sizeof(float));
        }
    }
}

}  // namespace ca::grantelliott::voice::audio::capture
