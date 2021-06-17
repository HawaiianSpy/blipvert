#pragma once

//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
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


namespace blipvert
{
    extern int32_t yr_table[256];
    extern int32_t yg_table[256];
    extern int32_t yb_table[256];
    extern int32_t ur_table[256];
    extern int32_t ug_table[256];
    extern int32_t ub_table[256];
    extern int32_t vr_table[256];
    extern int32_t vg_table[256];
    extern int32_t vb_table[256];

    extern int32_t luminance_table[256];
    extern int32_t u_table[256];
    extern int32_t v_table[256];
    extern int32_t uv_table[256][256];
    extern uint8_t saturation_table[900];

    extern uint32_t rgb32_greyscale[256];
    extern uint16_t rgb565_greyscale[256];
    extern uint16_t rgb555_greyscale[256];

    void InitLookupTables();
}

