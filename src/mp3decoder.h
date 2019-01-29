//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#pragma once

#include <list>
#include "mad.h"
#include "Speaker.h"

void decodeMP3File(unsigned char const* start, unsigned long length, void (*onChunk)(sChunk&));