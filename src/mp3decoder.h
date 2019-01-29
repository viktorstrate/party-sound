//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#pragma once

#include <vector>
#include "mad.h"
#include "Speaker.h"

sBuffer decodeMP3File(unsigned char const* start, unsigned long length);