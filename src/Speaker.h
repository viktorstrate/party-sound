//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#pragma once

#include <portaudio.h>
#include <list>

struct sChunk {
    int data[1152*2];
};

class Speaker {
private:
    std::list<sChunk> m_Buffer;
    PaStream* m_PaStream;

public:
    Speaker(std::list<sChunk> &buffer);
    ~Speaker();

    bool start();
    bool stop();

private:
//    static int paCallback(const void* inputBuffer, void* outputBuffer,
//                              unsigned long framesPerBuffer,
//                              const PaStreamCallbackTimeInfo* timeInfo,
//                              PaStreamCallbackFlags statusFlags,
//                              void* userData);
};

