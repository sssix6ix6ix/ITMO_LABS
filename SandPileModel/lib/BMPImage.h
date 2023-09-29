#pragma once

#include <cstdint>
#include <deque>
#include <fstream>
#include <string>

const uint32_t kINFOHEADERSIZE = 40;
const uint32_t kHeaderSize = 14;
const uint32_t kSizeOfOnePixel = 3;
const size_t kMultiplicityOfWidth = 4;
const uint8_t kAddition = 0;

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

const Color kWhite = {255, 255, 255};
const Color kGreen = {120, 214, 0};
const Color kPurple = {255, 0, 139};
const Color kYellow = {0, 255, 255};
const Color kBlack = {0, 0, 0};

#pragma pack(push, 1)
class BMPImage {
public:
    char bfType[2] = {'B', 'M'};
    uint32_t bf_size = 0;
    uint32_t bf_reserved = 0;
    uint32_t bf_0ffBits = 54;

    uint32_t bi_size = 40;

    uint32_t bi_width = 0;
    uint32_t bi_height = 0;
    uint16_t bi_planes = 1;
    uint16_t bi_bitCount = 24;
    uint32_t bi_compression = 0;

    uint32_t bi_sizeImage = 0;

    uint32_t bi_X_pels_per_meter = 0;
    uint32_t bi_Y_pels_per_meter = 0;
    uint32_t bi_clr_used = 0;
    uint32_t bi_clr_important = 0;

};
#pragma pack(pop)

void DrawImage(BMPImage& image, const std::deque<std::deque<uint64_t>>& pixels, const std::string& out);
