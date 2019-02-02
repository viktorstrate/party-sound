//
// Created by Viktor Hundahl Strate on 28/01/2019.
//

#pragma once

#include <list>
#include "mad.h"
#include "Speaker.h"

void decodeMP3File(std::string& filePath, void (*onChunk)(SoundChunk&));