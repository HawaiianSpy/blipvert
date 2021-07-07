//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#include "pch.h"
#include "LookupTables.h"
#include "CommonMacros.h"

using namespace blipvert;

#define SATTAB_OFFSET 300

int32_t blipvert::yr_table[256];
int32_t blipvert::yg_table[256];
int32_t blipvert::yb_table[256];
int32_t blipvert::ur_table[256];
int32_t blipvert::ug_table[256];
int32_t blipvert::ub_table[256];
int32_t blipvert::vr_table[256];
int32_t blipvert::vg_table[256];
int32_t blipvert::vb_table[256];

int32_t blipvert::luminance_table[256];
int32_t blipvert::u_table[256];
int32_t blipvert::v_table[256];
int32_t blipvert::uv_table[256][256];
uint8_t blipvert::saturation_table[900];

uint32_t blipvert::ayuv_greyscale[256];
uint32_t blipvert::rgba_greyscale[256];
uint32_t blipvert::rgb32_greyscale[256];
uint16_t blipvert::rgb565_greyscale[256];
uint16_t blipvert::rgb555_greyscale[256];

xRGBQUAD blipvert::rgb1_greyscale_palette[2] { { 0, 0, 0, 255} , {255, 255, 255, 255} };
xRGBQUAD blipvert::rgb4_greyscale_palette[16];
xRGBQUAD blipvert::rgb8_greyscale_palette[256];

void blipvert::InitLookupTables()
{
    //    Build the RGB to YUV conversion lookup tables.
     //
     //    RGB to YUV conversion formula used:
     //
     //    Y =  0.257R + 0.504G + 0.098B + 16
     //    U = -0.148R - 0.291G + 0.439B + 128
     //    V =  0.439R - 0.368G - 0.071B + 128

    xRGBQUAD quad = { 0xFF, 0xFF,0xFF,0xFF };

    for (int16_t index = 0; index < 256; index++)
    {
        double scalar = static_cast<double>(index) * 32768.0;
        yr_table[index] = static_cast<int32_t>((0.257 * scalar) + 0.5);
        yg_table[index] = static_cast<int32_t>((0.504 * scalar) + 0.5);
        yb_table[index] = static_cast<int32_t>((0.098 * scalar) + 0.5);

        ur_table[index] = static_cast<int32_t>((-0.148 * scalar) + 0.5);
        ug_table[index] = static_cast<int32_t>((-0.291 * scalar) + 0.5);
        ub_table[index] = static_cast<int32_t>((0.439 * scalar) + 0.5);

        vr_table[index] = static_cast<int32_t>((0.439 * scalar) + 0.5);
        vg_table[index] = static_cast<int32_t>((-0.368 * scalar) + 0.5);
        vb_table[index] = static_cast<int32_t>((-0.071 * scalar) + 0.5);

        // Greyscale extensions
        ayuv_greyscale[index] = (0xFF000000 | (static_cast<uint32_t>(index) << 16) | (static_cast<uint32_t>(index) << 8) | static_cast<uint32_t>(index));
        rgba_greyscale[index] = ((static_cast<uint32_t>(index) << 16) | (static_cast<uint32_t>(index) << 8) | static_cast<uint32_t>(index));
        rgb32_greyscale[index] = (0xFF000000 | (static_cast<uint32_t>(index) << 16) | (static_cast<uint32_t>(index) << 8) | static_cast<uint32_t>(index));
        PackRGB565Word(rgb565_greyscale[index], static_cast<uint16_t>(index), static_cast<uint16_t>(index), static_cast<uint16_t>(index));
        PackRGB555Word(rgb555_greyscale[index], static_cast<uint16_t>(index), static_cast<uint16_t>(index), static_cast<uint16_t>(index));

        quad.rgbBlue = static_cast<uint8_t>(index);
        quad.rgbGreen = static_cast<uint8_t>(index);
        quad.rgbRed = static_cast<uint8_t>(index);
        rgb8_greyscale_palette[index] = quad;
    }

    for (int16_t index = 0; index <= 16; index++)
    {
        uint8_t value = index * 16;
        if (value > 255) value = 255;
        quad.rgbBlue = value;
        quad.rgbGreen = value;
        quad.rgbRed = value;
        rgb8_greyscale_palette[index] = quad;
    }

    //    Build the YUV to RGB conversion lookup tables.
    //
    //    YUV to RGB conversion formula used:
    //
    //    B = 1.164(Y - 16) + 2.018(U - 128)
    //    G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
    //    R = 1.164(Y - 16) + 1.596(V - 128)
    //
    // Which simplifies to:
    //
    //    B = 1.164Y + 2.018U - 276.928
    //    G = 1.164Y - 0.813V - 0.391U + 154.112
    //    R = 1.164Y + 1.596V - 204.288

    for (int16_t index = 0; index < 256; index++)
    {
        luminance_table[index] = static_cast<int32_t>(1.164 * index) + SATTAB_OFFSET;
        u_table[index] = static_cast<int32_t>(2.018 * index - 276.928);
        v_table[index] = static_cast<int32_t>(1.596 * index - 204.288);
    }

    for (int16_t u = 0; u < 256; u++)
    {
        for (int16_t v = 0; v < 256; v++)
        {
            uv_table[u][v] = static_cast<int32_t>(-0.813 * v - 0.391 * u + 154.112);
        }
    }

    for (int16_t val = -SATTAB_OFFSET; val < 600; val++)
    {
        if (val < 0)
            saturation_table[val + SATTAB_OFFSET] = 0;
        else if (val > 255)
            saturation_table[val + SATTAB_OFFSET] = 255;
        else
            saturation_table[val + SATTAB_OFFSET] = static_cast<uint8_t>(val);
    }
}