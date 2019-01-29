//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#include <iostream>
#include "Speaker.h"

#define SAMPLE_RATE 44100

Speaker::Speaker(sBuffer buffer) : m_Buffer(buffer) {
    PaError err = Pa_Initialize();
    if (err != paNoError)
        std::cout << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(&m_PaStream,
                               0,          /* no input channels */
                               2,          /* stereo output */
                               paInt32,
                               SAMPLE_RATE,
                               1152,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                               paCallback, /* this is your callback function */
                               &m_Buffer); /*This is a pointer that will be passed to
                                                   your callback*/
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

    Pa_Sleep(100000);

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

int Speaker::paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {

/* Cast data passed through stream to our structure. */
    auto* buffer = (sBuffer*)userData;

    auto* out = (int*) outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for (i = 0; i < framesPerBuffer; i++) {

        int* left;
        int* right;

        if ((left = (int*) buffer->next()) == nullptr || (right = (int*) buffer->next()) == nullptr) {
            return PaStreamCallbackResult::paComplete;
        }

        *out++ = *left; // left
        *out++ = *right; // right

    }

    return 0;
}
