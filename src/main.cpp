#include <iostream>

#include <math.h>
#include <mad.h>

#include "Speaker.h"
#include "mp3decoder.h"
#include "Network.h"
#include "CLI.h"


Speaker speaker;

void handleChunk(SoundChunk& chunk) {
    speaker.pushChunk(chunk);
}

int main(int argc, char* argv[]) {
    std::cout << "Party Sound" << std::endl;

    CLI::start();


//    Network::Server server;
//    server.start();
//
//    // Make speaker ready to receive chunks to play
//    speaker.start();
//
//    if (argc < 2) {
//        std::cout << "Please provide a path to a .mp3" << std::endl;
//    }
//
//    std::string filePath = argv[1];
//
//    decodeMP3File(filePath, handleChunk);
//
//    speaker.join();

    return 0;
}

