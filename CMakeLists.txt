cmake_minimum_required(VERSION 3.12)
project(party_sound)

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(portaudio REQUIRED)
find_package(mad REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(party_sound src/main.cpp src/Speaker.cpp src/Speaker.h src/mp3decoder.cpp)

include_directories(${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(party_sound ${PORTAUDIO_LIBRARIES})

include_directories(${MAD_INCLUDE_DIRS})
target_link_libraries(party_sound ${MAD_LIBRARIES})