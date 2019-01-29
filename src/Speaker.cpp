//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#include <iostream>
#include "Speaker.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 1152

Speaker::Speaker(std::list<sChunk> &buffer) : m_Buffer(buffer) {
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

bool Speaker::start() {

    std::cout << "Starting sound" << std::endl;

    PaError err = Pa_StartStream(m_PaStream);
    if (err != paNoError) {
        std::cout << "PortAudio start stream error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    /* -- Here's the loop where we pass data from input to output -- */
    while (!m_Buffer.empty()) {

        sChunk chunk = m_Buffer.front();
        m_Buffer.pop_front();

        err = Pa_WriteStream(m_PaStream, chunk.data, FRAMES_PER_BUFFER);

        if (err != paNoError) {
            std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        }

    }

    return true;
}

bool Speaker::stop() {
    PaError err = Pa_StopStream(m_PaStream);

    if (err != paNoError) {
        std::cout << "PortAudio stop stream error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    return true;
}
