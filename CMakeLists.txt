cmake_minimum_required(VERSION 3.10)
project(party_sound)

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(portaudio REQUIRED)
find_package(mad REQUIRED)
find_package(enet REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/main.cpp
        src/Speaker.cpp
        src/Speaker.h
        src/mp3decoder.cpp
        src/SoundChunk.h
        src/Time.cpp
        src/Time.h
        src/NetworkServer.cpp
        src/NetworkClient.cpp
        src/Network.h
        src/CLI.cpp src/CLI.h)

add_executable(party_sound ${SOURCES})

include_directories(${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(party_sound ${PORTAUDIO_LIBRARIES})

include_directories(${MAD_INCLUDE_DIRS})
target_link_libraries(party_sound ${MAD_LIBRARIES})

include_directories(${ENet_INCLUDE_DIRS})
target_link_libraries(party_sound ${ENet_LIBRARIES})