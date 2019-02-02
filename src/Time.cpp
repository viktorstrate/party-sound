//
// Created by Viktor Hundahl Strate on 30/01/2019.
//

#include "Time.h"

#include <chrono>

namespace Time {

    static long long initialTime = getNowTimestamp();

    long long getNowTimestamp() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    std::string formatTimestamp(long long &timestamp) {

        return std::to_string(timestamp - initialTime);
    }


}