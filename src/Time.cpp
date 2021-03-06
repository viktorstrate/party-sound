//
// Created by Viktor Hundahl Strate on 30/01/2019.
//

#include "Time.h"

#include <chrono>

#if defined __APPLE__ || defined __linux__
#include <unistd.h>
#else
#include <Windows.h>
#endif

namespace Time {

    static long long initialTime = getNowTimestamp();

    long long getNowTimestamp() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    std::string formatTimestamp(long long &timestamp) {

        return std::to_string(timestamp - initialTime);
    }

	void sleep(unsigned long ms) {
		#if defined __APPLE__ || defined __linux__
		usleep(1000 * ms);
		#else
		Sleep(ms);
		#endif
	}


}