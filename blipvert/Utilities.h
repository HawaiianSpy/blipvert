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
    // For a slow, but more accurate transform of single pixel values.
    void SlowYUVtoRGB(double Y, double U, double V, double* R, double* G, double* B);
    void SlowRGBtoYUV(double R, double G, double B, double* Y, double* U, double* V);

    // Lookup table transforms of single pixel value.
    void FastYUVtoRGB(uint8_t Y, uint8_t U, uint8_t V, uint8_t* R, uint8_t* G, uint8_t* B);
    void FastRGBtoYUV(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* U, uint8_t* V);

    // For interlacing de-interlacing bitmaps.
    // This function was written for a couple of the transforms. Don't know if it would work
    // as a general-purpose de-interlacing function for uncompressed video.
    void Progressive_to_Interlaced(int32_t height, int32_t line_bytes, bool flipped, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride);
    void Interlaced_to_Progressive(int32_t height, int32_t line_bytes, bool flipped, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride);

    // Calculates size of the buffer given the video format and dimensions.
    //
    // Parameters:
    //      inFormat:           The media ID to calculate.
    //      width & height:     The dimensions of the bitmap in pixels.
    //      in_stride:          The number of bytes per row in the bitmap. 0 (zero) tells the function to use the default size for the given the format.
    // Returns the size of the buffer in bytes. Returns 0 if the format is not found, a parameter is invalid, or the internal format info in incomplete.
    //
    // IMPORTANT:   For reasons related to the bitmap format definitions, all input parameters must follow these rules:
    //              1. Both the width and height values must be even multiples of 8.
    //              2. The width values must be >= 8;
    //              3. The height value must be >= 16.
    //              4. The stride value must be >= the minimum number of bytes-per-line needed for the width of the bitmap format.
    uint32_t CalculateBufferSize(const MediaFormatID& inFormat, uint32_t width, uint32_t height, int32_t stride = 0);

    // Returns the minimum size in bytes needed to contain one horizontal line of the bitmap.
    //
    // Parameters:
    //      inFormat:           The media ID to calculate.
    //      width & height:     The dimensions of the bitmap in pixels.
    // Returns the size of the line in bytes.
     //
    // IMPORTANT:   For reasons related to the bitmap format definitions, all input parameters must follow these rules:
    //              1. Both the width and height values must be even multiples of 8.
    //              2. The width values must be >= 8;
    //              3. The height value must be >= 16.
    //              4. The stride value must be >= the minimum number of bytes-per-line needed for the width of the bitmap format.
    int32_t CalculateMinimumLineStride(const MediaFormatID& inFormat, uint32_t width, uint32_t height);

    // Returns TRUE if the colorspace is RGB
    bool IsRGBColorspace(const MediaFormatID& encoding);
    bool IsRGBColorspace(const Fourcc fourcc);

    // Returns TRUE if the colorspace is YUV
    bool IsYUVColorspace(const MediaFormatID& encoding);
    bool IsYUVColorspace(const Fourcc fourcc);

    // Returns TRUE if the encoding is RGB with a palette
    bool IsPalletizedEncoding(const MediaFormatID& encoding);
    bool IsPalletizedEncoding(const Fourcc fourcc);

    // Returns TRUE if a format uses planar chroma layout (e.g. YV12, I420)
    bool IsPlanarYUV(const MediaFormatID& encoding);
    bool IsPlanarYUV(const Fourcc fourcc);

    bool IsGloballyValidBitmapDimension(int32_t width, int32_t height);
}

