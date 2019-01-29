//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#include "mp3decoder.h"

#include <unistd.h>
#include <iostream>

struct sBuffer {
    unsigned char* start;
    unsigned long length;
};

struct user_data {
    sBuffer input;
    std::list<sChunk>* output;
};

// Read everything at once
static mad_flow input(void* data,
                      struct mad_stream* stream) {
    auto* userData = (struct user_data*)data;

    auto* buffer = &userData->input;

    if (buffer->length == 0)
        return MAD_FLOW_STOP;

    mad_stream_buffer(stream, (unsigned char*)buffer->start, buffer->length);

    buffer->length = 0;

    return MAD_FLOW_CONTINUE;
}

// Down scale to 16 bits
static inline signed int scale(mad_fixed_t sample) {
    /* round */
    sample += (1L << (MAD_F_FRACBITS - 16));

    /* clip */
    if (sample >= MAD_F_ONE)
        sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
        sample = -MAD_F_ONE;

    /* quantize */
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static mad_flow error(void* data,
                      struct mad_stream* stream,
                      struct mad_frame* frame) {

    auto* userData = (struct user_data*)data;
    auto* buffer = &userData->input;

    std::cerr << "Decoding error: " << stream->error << " at byte offset " << (stream->this_frame - (unsigned char*)buffer->start)
              << std::endl;

    /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

    return MAD_FLOW_CONTINUE;
}

static mad_flow output(void* data, struct mad_header const* header, struct mad_pcm* pcm) {

    auto* userData = (struct user_data*)data;

    unsigned int nchannels, nsamples;
    mad_fixed_t const* left_ch, * right_ch;

    /* pcm->samplerate contains the sampling frequency */

    nchannels = pcm->channels;
    nsamples = pcm->length;
    left_ch = pcm->samples[0];
    right_ch = pcm->samples[1];

    std::cout << "Decoding chunk, length: " << nsamples << " sample rate: " << pcm->samplerate << std::endl;

    sChunk chunk{};
    unsigned int offset = 0;

    while (nsamples--) {
        chunk.data[offset++] = *left_ch++;
        chunk.data[offset++] = *right_ch++;
    }

    userData->output->push_back(chunk);

    return MAD_FLOW_CONTINUE;
}

std::list<sChunk> decodeMP3File(unsigned char const* start, unsigned long length) {
    mad_decoder decoder{};
    user_data userData{};

    auto outputVal = std::list<sChunk>();

    /* initialize our private message structure */
    userData.input.start = (unsigned char*)start;
    userData.input.length = length;
    userData.output = &outputVal;

    /* configure input, output, and error functions */
    mad_decoder_init(&decoder, &userData,
                     input, 0 /* header */, 0 /* filter */, output,
                     error, 0 /* message */);

    /* start decoding */
    int result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

    /* release the decoder */
    mad_decoder_finish(&decoder);


    return outputVal;
}
