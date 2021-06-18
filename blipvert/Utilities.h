#pragma once

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

#include "blipvert.h"

namespace blipvert
{
    // For a slow, but more accurate conversion of single pixel values
    void SlowYUVtoRGB(uint8_t Y, uint8_t U, uint8_t V, uint8_t* R, uint8_t* G, uint8_t* B);
    void SlowRGBtoYUV(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* U, uint8_t* V);

    // Lookup table conversions of single pixel value..
    void FastYUVtoRGB(uint8_t Y, uint8_t U, uint8_t V, uint8_t* R, uint8_t* G, uint8_t* B);
    void FastRGBtoYUV(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* U, uint8_t* V);

    // For interlacing de-interlacing bitmaps
    void Progressive_to_Interlaced(int32_t height, int32_t line_bytes, bool flipped, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride);
    void Interlaced_to_Progressive(int32_t height, int32_t line_bytes, bool flipped, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride);

    /***************************************************************************************
    * IMPORTANT: All pixel bitmap dimensions (width & height) should be multiples of 4
    * to accomodate the packed YUV formats and general algorithmic speed improvements.
    * You can get away with non-modulo-four values for many of the transforms, but
    * following that rule will insure maximum compatibilty across the library. The
    * consequences for living on the edge? Some of the transforms may try coloring outside
    * the lines. Of course, you can experiment with the buffer sizes to work around that. 
    *****************************************************************************************/

    // Calculates size of the buffer given the video format and dimensions.
    //
    // Parameters:
    //      inFormat:           The media ID to calculate.
    //      width & height:     The dimensions of the bitmap in pixels.
    //      in_stride:          The number of bytes per row in the bitmap. 0 (zero) tells the function to use the default size for the given the format.
    // Returns the size of the buffer in bytes. Returns 0 if the format is not found, a parameter is invalid, or the internalformat info in incomplete.
    uint32_t CalculateBufferSize(const MediaFormatID& inFormat, uint32_t width, uint32_t height, uint32_t in_stride = 0);

    // Returns TRUE if the encoding is RGB
    bool IsRGBEncoding(const MediaFormatID& encoding);

    // Returns TRUE if the encoding is RGB with a palette
    bool IsPalletizedEncoding(const MediaFormatID& encoding);

    // Returns TRUE if the fourcc is RGB
    bool IsRGBFourcc(Fourcc fourcc);
}

