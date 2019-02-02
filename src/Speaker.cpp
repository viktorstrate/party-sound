//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#include "Speaker.h"

#include <iostream>
#include <unistd.h>

Speaker::Speaker() {
    PaError err = Pa_Initialize();
    if (err != paNoError)
        std::cout << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(&m_PaStream,
                               0,          /* no input channels */
                               2,          /* stereo output */
                               paInt32,
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,        /* frames per buffer, i.e. the number of sample frames that PortAudio will request */
                               nullptr,
                               nullptr);


    if (err != paNoError)
        std::cout << "PortAudio open stream error: " << Pa_GetErrorText(err) << std::endl;
}

Speaker::~Speaker() {
    PaError err = Pa_Terminate();
    if (err != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    }
}

void Speaker::start() {

    std::cout << "Starting speaker in another thread" << std::endl;

    m_StopRequested = false;
    m_SoundThread = std::thread(&Speaker::streamSound, this);

}

void Speaker::stop() {
    m_StopRequested = true;
    m_SoundThread.join();
}

void Speaker::join() {
    m_SoundThread.join();
}

void Speaker::streamSound() {
    std::cout << "Starting sound" << std::endl;

    PaError err = Pa_StartStream(m_PaStream);
    if (err != paNoError) {
        std::cout << "PortAudio start stream error: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    auto moveQueue = [this]{
        while (!m_UpcomingChunks.empty()) {
            m_QueuedChunks.push_back(m_UpcomingChunks.front());
            m_UpcomingChunks.pop_front();
        }
    };

    std::unique_lock<std::mutex> upcomingLock(m_MutUpcomming, std::defer_lock);

    while (!m_StopRequested) {

        if (m_QueuedChunks.size() < 3) {
            std::cout << "QueuedChunks is soon empty, trying to refill..." << std::endl;
            if (upcomingLock.try_lock()) {

                moveQueue();

                upcomingLock.unlock();
            }
        }

        if (m_QueuedChunks.empty()) {

            std::cout << "No QueuedChunks left, waiting for new ones..." << std::endl;

            // Wait until data has been added to upcomingChunks and lock
            upcomingLock.lock();
            m_Upcoming_changed.wait(upcomingLock, [this]{
                return !m_UpcomingChunks.empty();
            });

            moveQueue();

            std::cout << "New chunks loaded" << std::endl;

            upcomingLock.unlock();
        }

        SoundChunk chunk = m_QueuedChunks.front();
        m_QueuedChunks.pop_front();

        long long timeNow = Time::getNowTimestamp();
        long long timeToWait = chunk.timestamp - timeNow;

        std::cout << "Time to wait: " << timeToWait << " block timestamp: " << chunk.timestamp << std::endl;

        // Discard old but not played chunks
        if (timeToWait < -10) {

            int count = 0;

            while (chunk.timestamp - timeNow < 0 && !m_QueuedChunks.empty()) {
                chunk = m_QueuedChunks.front();
                m_QueuedChunks.pop_front();
                count++;
            }

            if (count > 0)
                std::cout << "Discarded " << count << " old chunks" << std::endl;

            if (m_QueuedChunks.empty()) {
                std::cout << "Queue empty after discarding, resetting loop" << std::endl;
                usleep(1000*50);
                continue;
            }
        }

        // Wait to sync chunk timestamp with playback
        if (timeToWait > 0) {
            std::cout << "Waiting " << timeToWait << "ms before playing next chunk" << std::endl;
            usleep(1000 * (unsigned int)timeToWait);
        }

        err = Pa_WriteStream(m_PaStream, chunk.data, FRAMES_PER_BUFFER);

        if (err != paNoError) {
            std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        }

    }

}

void Speaker::pushChunk(SoundChunk chunk) {

    std::lock_guard lk(m_MutUpcomming);

    m_UpcomingChunks.push_back(chunk);
    m_Upcoming_changed.notify_one();

}
