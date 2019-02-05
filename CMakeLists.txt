cmake_minimum_required(VERSION 3.10)
project(party_sound)

SET(USE_PREBUILT TRUE)
set(PREBUILT_PATH ${CMAKE_SOURCE_DIR}/libs)

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

if (NOT USE_PREBUILT)
    find_package(portaudio REQUIRED)
    find_package(mad REQUIRED)
endif()

add_subdirectory(libs/enet)

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

if (NOT PORTAUDIO_FOUND OR USE_PREBUILT)
    message("Portaudio: using bundled prebuilt version")

    find_path(PORTAUDIO_INCLUDE_DIRS portaudio.h ${PREBUILT_PATH})
    find_library(PORTAUDIO_LIBRARIES portaudio ${PREBUILT_PATH})
endif ()

if (NOT MAD_FOUND OR USE_PREBUILT)
    message("MAD: using bundled prebuilt version")

    find_path(MAD_INCLUDE_DIRS mad.h ${PREBUILT_PATH}/mad/include)
    find_library(MAD_LIBRARIES mad ${PREBUILT_PATH})

endif()

include_directories(${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(party_sound ${PORTAUDIO_LIBRARIES})

include_directories(${MAD_INCLUDE_DIRS})
target_link_libraries(party_sound ${MAD_LIBRARIES})

if (ENet_FOUND)
    include_directories(${ENet_INCLUDE_DIRS})
endif()
target_link_libraries(party_sound enet)