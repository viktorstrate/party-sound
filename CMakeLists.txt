cmake_minimum_required(VERSION 3.10)
project(party_sound)

SET(USE_PREBUILT FALSE CACHE BOOL "Whether or not the bundled prebuilt libraries should be used even if external ones were found")
set(PREBUILT_PATH ${CMAKE_SOURCE_DIR}/libs)

if(WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
endif()

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

if (NOT USE_PREBUILT)
    find_package(portaudio REQUIRED)
    find_package(mad REQUIRED)
    find_package(enet REQUIRED)
endif()

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

    find_path(PORTAUDIO_INCLUDE_DIRS portaudio.h HINTS ${PREBUILT_PATH}/portaudio/include)
    find_library(PORTAUDIO_LIBRARIES NAMES portaudio libportaudio portaudio_x86 HINTS ${PREBUILT_PATH}/portaudio/)
endif ()

if (NOT MAD_FOUND OR USE_PREBUILT)
    message("MAD: using bundled prebuilt version")

    find_path(MAD_INCLUDE_DIRS NAMES mad.h HINTS ${PREBUILT_PATH}/mad/include)
    find_library(MAD_LIBRARIES NAMES libmad mad HINTS ${PREBUILT_PATH}/mad/)

endif()

if (NOT ENet_FOUND OR USE_PREBUILT)
    message("ENet: using bundled prebuilt version")

    find_path(ENet_INCLUDE_DIRS NAMES enet/enet.h HINTS ${PREBUILT_PATH}/enet/include)
    find_library(ENet_LIBRARIES NAMES libenet enet HINTS ${PREBUILT_PATH}/enet/)
endif()

include_directories(${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(party_sound ${PORTAUDIO_LIBRARIES})

include_directories(${MAD_INCLUDE_DIRS})
target_link_libraries(party_sound ${MAD_LIBRARIES})

include_directories(${PREBUILT_PATH}/enet/include)
target_link_libraries(party_sound ${ENet_LIBRARIES})