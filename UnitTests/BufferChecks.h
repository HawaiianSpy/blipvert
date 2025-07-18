#pragma once

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

#include <cstdint>
#include <vector>

#include "blipvert.h"

namespace BlipvertUnitTests
{
	typedef struct RGBATestData {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;

	} RGBATestData;

	extern const std::vector<RGBATestData> AlphaTestMetaData;
	extern const std::vector<RGBATestData> TestMetaData;
	extern const std::vector<RGBATestData> ColorbarMetaData;

	extern const uint32_t TestBufferWidth;
	extern const uint32_t TestBufferHeight;
	extern const uint32_t StrideBumpTestValue;
	extern uint32_t StrideBump;

	extern const uint32_t MTTestBufferWidth;
	extern const uint32_t MTTestBufferHeight;
	extern int thread_count;

	extern uint32_t CalculateStrideBump(const blipvert::MediaFormatID& inFormat, uint32_t width, uint32_t height);

	typedef bool(__cdecl* t_buffercheckfunc) (uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);

	extern void GenerateVerticalColorBars(const blipvert::MediaFormatID& target, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);

	t_buffercheckfunc FindBufferCheckFunction(const blipvert::MediaFormatID& target);

	void RunMultiThreadedTransform(blipvert::t_transformfunc encodeTransPtr, uint32_t width, uint32_t height,
		blipvert::t_stagetransformfunc pstage_in, uint8_t* inBufPtr, uint32_t in_stride, bool in_flipped, blipvert::xRGBQUAD* in_palette,
		blipvert::t_stagetransformfunc pstage_out, uint8_t* outBufPtr, uint32_t out_stride, bool out_flipped, blipvert::xRGBQUAD* out_palette);

	bool Check_YUY2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_UYVY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_YVYU(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_VYUY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_I420(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_YV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_YVU9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_YUV9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_IYU1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_IYU2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_Y41P(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_CLJR(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_Y800(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_Y16(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_AYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_IMC1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_IMC2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_IMC3(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_IMC4(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_NV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_NV21(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_Y42T(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_Y41T(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_YV16(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);

	bool Check_RGBA(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_RGB32(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_RGB24(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_RGB565(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_RGB555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
	bool Check_ARGB1555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
}
