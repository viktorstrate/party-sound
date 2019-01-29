#include <iostream>
#include <fstream>
#include <math.h>
#include <mad.h>

#include "Speaker.h"
#include "mp3decoder.h"

Speaker speaker;

void handleChunk(sChunk& chunk) {
    speaker.pushChunk(chunk);
}

int main(int argc, char* argv[]) {
    std::cout << "Party Sound" << std::endl;

    // Make speaker ready to receive chunks to play
    speaker.start();

    std::ifstream sound_file;

    if (argc < 2) {
        std::cout << "Please provide a path to a .mp3" << std::endl;
    }

    std::string filePath = argv[1];

    std::cout << "Reading " << filePath << std::endl;

    std::ifstream music_file(filePath, std::ios::in|std::ios::binary|std::ios::ate);

    if (music_file.is_open()) {
        std::streampos size;
        unsigned char* memblock;

        size = music_file.tellg();
        memblock = new unsigned char[size];
        music_file.seekg(0, std::ios::beg);
        music_file.read((char*)memblock, size);
        music_file.close();
        std::cout << "File size: " << size << " bytes" << std::endl;

        decodeMP3File(memblock, static_cast<unsigned long>(size), handleChunk);

        delete[] memblock;
    } else std::cout << "Unable to open file";

    speaker.wait();

    return 0;
}

