#pragma once

#include "BMPImage.h"

#include <cinttypes>

const uint8_t kOverSand = 4;

struct Cord {
    int64_t x;
    int64_t y;

    explicit Cord(std::pair<int64_t, int64_t> pair) {
        x = pair.first;
        y = pair.second;
    }
};

class SandPile{
public:
    uint32_t height;
    uint32_t width;

    SandPile(){
        height = 0;
        width = 0;
    };

    void init(const uint32_t& height_by_parser, const uint32_t& width_by_parser);

    void SandPileSimulation(BMPImage& image, const std::string& input, const std::string& out, const uint32_t& max_iter,const uint32_t& freq);
};
