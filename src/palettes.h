#pragma once

#include <vector>
#include <tuple>

namespace palettes {

struct RGB {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

inline std::vector<RGB> def = {
    {0x55,0x55,0x55},
    {0x00,0x10,0x7E},
    {0x0F,0x07,0x85},
    {0x40,0x04,0x6A},
    {0x65,0x01,0x37},
    {0x74,0x00,0x00},
    {0x65,0x00,0x00},
    {0x39,0x10,0x00},
    {0x01,0x22,0x00},
    {0x00,0x2E,0x00},
    {0x00,0x31,0x00},
    {0x00,0x2C,0x20},
    {0x00,0x20,0x59},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},

    {0xA5,0xA5,0xA5},
    {0x00,0x4E,0xD6},
    {0x3F,0x36,0xE6},
    {0x88,0x22,0xCA},
    {0xC0,0x17,0x8B},
    {0xD6,0x1A,0x39},
    {0xC5,0x28,0x00},
    {0x8F,0x41,0x00},
    {0x46,0x5A,0x00},
    {0x00,0x6D,0x00},
    {0x00,0x73,0x00},
    {0x00,0x71,0x51},
    {0x00,0x63,0xA0},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},

    {0xFE,0xFF,0xFF},
    {0x3E,0x96,0xFF},
    {0x72,0x78,0xFF},
    {0xBB,0x6E,0xFF},
    {0xFF,0x74,0xEC},
    {0xFF,0x7A,0xA2},
    {0xFF,0x82,0x5A},
    {0xF7,0x91,0x11},
    {0xB1,0xAA,0x00},
    {0x65,0xBF,0x0F},
    {0x28,0xCB,0x4A},
    {0x0B,0xCB,0x9C},
    {0x15,0xBF,0xEE},
    {0x3C,0x3C,0x3C},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},

    {0xFE,0xFF,0xFF},
    {0xA7,0xD4,0xFF},
    {0xB9,0xC2,0xFF},
    {0xDA,0xBE,0xFF},
    {0xFF,0xC5,0xFD},
    {0xFF,0xC5,0xDD},
    {0xFF,0xC9,0xBD},
    {0xFF,0xD0,0xA0},
    {0xE4,0xDA,0x94},
    {0xC4,0xE3,0x99},
    {0xAA,0xE9,0xAF},
    {0x9B,0xEA,0xD0},
    {0x9D,0xE6,0xF2},
    {0xAF,0xAF,0xAF},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
};

} // palettes
