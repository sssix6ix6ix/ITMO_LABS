#pragma once

#include "BMPImage.h"
#include "sandpile.h"

#include <cinttypes>
#include <string>

class Parser{
public:
    uint32_t height;
    uint32_t width;
    std::string input;
    std::string output;
    uint32_t max_iter;
    uint32_t freq;

    void Parsing(int argc, char** argv);
};
