
//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021-2025 Don Jordan
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
#include "framework.h"
#include "CommonMacros.h"

using namespace blipvert;

// 32-bit bitmasks

const uint32_t blipvert::RGB555_RED_MASK = 0x00007C00;
const uint32_t blipvert::RGB555_GREEN_MASK = 0x000003E0;
const uint32_t blipvert::RGB555_BLUE_MASK = 0x0000001F;

const uint32_t blipvert::RGB565_RED_MASK = 0x0000F800;
const uint32_t blipvert::RGB565_GREEN_MASK = 0x000007E0;
const uint32_t blipvert::RGB565_BLUE_MASK = 0x0000001F;

const uint32_t blipvert::RGB32_ALPHA_MASK = 0xFF000000;
const uint32_t blipvert::RGB32_RED_MASK = 0x00FF0000;
const uint32_t blipvert::RGB32_GREEN_MASK = 0x0000FF00;
const uint32_t blipvert::RGB32_BLUE_MASK = 0x000000FF;

const uint32_t blipvert::RGB32_BLUEX_MASK = 0x00FF0000;
const uint32_t blipvert::RGB32_REDX_MASK = 0x000000FF;
