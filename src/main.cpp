#include <iostream>
#include <fstream>
#include <math.h>
#include <mad.h>

#include "Speaker.h"
#include "mp3decoder.h"

int main(int argc, char* argv[]) {
    std::cout << "Party Sound" << std::endl;

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

        sBuffer buffer = decodeMP3File(memblock, static_cast<unsigned long>(size));

        Speaker speaker(buffer);

        speaker.start();

        delete[] memblock;
    } else std::cout << "Unable to open file";

    return 0;
}

