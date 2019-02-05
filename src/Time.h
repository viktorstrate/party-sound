//
// Created by Viktor Hundahl Strate on 30/01/2019.
//

#pragma once

#include <string>

namespace Time {

    long long getNowTimestamp();

    std::string formatTimestamp(long long &timestamp);

	void sleep(unsigned long ms);

};

