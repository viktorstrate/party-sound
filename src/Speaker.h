//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#pragma once

#include <portaudio.h>

struct sBuffer {
    const int* start;
    unsigned long length;

private:
    unsigned long cursor;

public:
    const int* next(unsigned int step = 1) {

        if (cursor >= length - 1) {
            return nullptr;
        }

        cursor += step;
        return start + cursor;
    }
};

class Speaker {
private:
    sBuffer m_Buffer;
    PaStream* m_PaStream;

public:
    Speaker(sBuffer buffer);
    ~Speaker();

    bool start();
    bool stop();

private:
    static int paCallback(const void* inputBuffer, void* outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void* userData);
};

