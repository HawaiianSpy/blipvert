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


#include "blipverttypes.h"
#include "Staging.h"

namespace blipvert
{
    // RGB to RGB transform functions
    void RGBA_to_RGB32(Stage* in, Stage* out);
    void RGBA_to_ARGB1555(Stage* in, Stage* out);

    void RGB32_to_RGBA(Stage* in, Stage* out);
    void RGB32_to_RGB24(Stage* in, Stage* out);
    void RGB32_to_RGB565(Stage* in, Stage* out);
    void RGB32_to_RGB555(Stage* in, Stage* out);

    void RGB24_to_RGB32(Stage* in, Stage* out);
    void RGB24_to_RGB565(Stage* in, Stage* out);
    void RGB24_to_RGB555(Stage* in, Stage* out);

    void RGB555_to_RGB32(Stage* in, Stage* out);
    void RGB555_to_RGBA(Stage* in, Stage* out);
    void RGB555_to_RGB24(Stage* in, Stage* out);
    void RGB555_to_RGB565(Stage* in, Stage* out);
    void RGB555_to_ARGB1555(Stage* in, Stage* out);

    void ARGB1555_to_RGBA(Stage* in, Stage* out);
    void ARGB1555_to_RGB555(Stage* in, Stage* out);

    void RGB565_to_RGB32(Stage* in, Stage* out);
    void RGB565_to_RGB24(Stage* in, Stage* out);
    void RGB565_to_RGB555(Stage* in, Stage* out);

    void RGB8_to_RGB32(Stage* in, Stage* out);
    void RGB8_to_RGB24(Stage* in, Stage* out);
    void RGB8_to_RGB565(Stage* in, Stage* out);
    void RGB8_to_RGB555(Stage* in, Stage* out);

    void RGB4_to_RGB32(Stage* in, Stage* out);
    void RGB4_to_RGB24(Stage* in, Stage* out);
    void RGB4_to_RGB565(Stage* in, Stage* out);
    void RGB4_to_RGB555(Stage* in, Stage* out);

    void RGB1_to_RGB32(Stage* in, Stage* out);
    void RGB1_to_RGB24(Stage* in, Stage* out);
    void RGB1_to_RGB565(Stage* in, Stage* out);
    void RGB1_to_RGB555(Stage* in, Stage* out);
}

