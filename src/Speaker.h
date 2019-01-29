//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#pragma once

#include <portaudio.h>
#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>

struct sChunk {
    int data[1152*2];
};

class Speaker {
private:
    std::list<sChunk> m_UpcomingChunks;
    std::list<sChunk> m_QueuedChunks;

    std::mutex m_MutUpcomming;
    std::condition_variable m_Upcoming_changed;

    std::thread m_SoundThread;

    PaStream* m_PaStream;

    bool m_StopRequested = false;


public:
    Speaker();
    ~Speaker();

    void start();
    void stop();
    void wait();

    void pushChunk(sChunk chunk);

private:
    void streamSound();
//    static int paCallback(const void* inputBuffer, void* outputBuffer,
//                              unsigned long framesPerBuffer,
//                              const PaStreamCallbackTimeInfo* timeInfo,
//                              PaStreamCallbackFlags statusFlags,
//                              void* userData);
};

