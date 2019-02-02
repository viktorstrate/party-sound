//
// Created by Viktor Hundahl Strate on 30/01/2019.
//


#pragma once

#include "Time.h"
#include <iostream>

#define CHUNK_SIZE 1152

struct SoundChunk {
    long long timestamp;
    int data[CHUNK_SIZE*2];

    SoundChunk() : data() {
        timestamp = Time::getNowTimestamp();
    }
};

