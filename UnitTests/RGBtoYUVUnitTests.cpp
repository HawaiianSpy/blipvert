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

#include "pch.h"
#include "CppUnitTest.h"

#include "blipvert.h"
#include "Utilities.h"
#include "RGBtoYUV.h"
#include "YUVtoRGB.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	typedef bool(__cdecl* t_yuvcheckfunc) (uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);

	bool Check_PackedY422(uint8_t y_level, uint8_t u_level, uint8_t v_level,
		int32_t width, int32_t height,
		uint8_t* out_buf, int32_t out_stride,
		int32_t y0_offset, int32_t y1_offset, int32_t u_offset, int32_t v_offset)
	{
		if (!out_stride)
			out_stride = width * 2;

		uint32_t fill = static_cast<uint32_t>((y_level << (y0_offset * 8)) | (y_level << (y1_offset * 8)) | \
			(u_level << (u_offset * 8)) | (v_level << (v_offset * 8)));

		for (int32_t h = 0; h < height; h++)
		{
			uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
			for (int32_t w = 0; w < width; w += 2)
			{
				if (*pdst++ != fill) return false;
			}

			out_buf += out_stride;
		}

		return true;
	}

	bool Check_YUY2(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		return Check_PackedY422(y_level, u_level, v_level,
			width, height,
			pBuffer, stride,
			0, 2, 1, 3);
	}

	bool Check_UYVY(uint8_t y_level, uint8_t u_level, uint8_t v_level,
		int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		return Check_PackedY422(y_level, u_level, v_level,
			width, height,
			pBuffer, stride,
			1, 3, 0, 2);
	}

	bool Check_YVYU(uint8_t y_level, uint8_t u_level, uint8_t v_level,
		int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		return Check_PackedY422(y_level, u_level, v_level,
			width, height,
			pBuffer, stride,
			0, 2, 3, 1);
	}

	bool Check_VYUY(uint8_t y_level, uint8_t u_level, uint8_t v_level,
		int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		return Check_PackedY422(y_level, u_level, v_level,
			width, height,
			pBuffer, stride,
			1, 3, 2, 0);
	}

	bool Check_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		if (!stride)
			stride = width * 4;

		uint32_t fill = static_cast<uint32_t>(((((alpha) & 0xFF) << 24) | (((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF)));
		do
		{
			uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
			int32_t hcount = width;
			do
			{
				if (*pdst++ != fill) return false;
			} while (--hcount);

			pBuffer += stride;
		} while (--height);

		return true;
	}


	bool Check_Y800(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		if (!stride)
			stride = width;

		for (int32_t h = 0; h < height; h++)
		{
			uint8_t* pdst = pBuffer;
			for (int32_t w = 0; w < width; w++)
			{
				if (*pdst++ != y_level) return false;
			}

			pBuffer += stride;
		}

		return true;
	}

	bool Check_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level,
		int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		if (!stride)
			stride = width * 12 / 8;

		for (int32_t h = 0; h < height; h++)
		{
			uint8_t* pdst = pBuffer;
			for (int32_t w = 0; w < width; w += 4)
			{
				if (pdst[0] != u_level) return false;
				if (pdst[1] != y_level) return false;
				if (pdst[2] != y_level) return false;
				if (pdst[3] != v_level) return false;
				if (pdst[4] != y_level) return false;
				if (pdst[5] != y_level) return false;
				pdst += 6;
			}

			pBuffer += stride;
		}

		return true;
	}

	bool Check_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level,
		int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
	{
		if (!stride)
			stride = width * 3;

		for (int32_t h = 0; h < height; h++)
		{
			uint8_t* pdst = pBuffer;
			for (int32_t w = 0; w < width; w++)
			{
				if (pdst[0] != u_level) return false;
				if (pdst[1] != y_level) return false;
				if (pdst[2] != v_level) return false;
				pdst += 3;
			}

			pBuffer += stride;
		}

		return true;
	}

	typedef struct {
		const MediaFormatID& target;
		t_yuvcheckfunc pProcAddr;
	} YUVCheckEntry;

	YUVCheckEntry YUVCheckFuncTable[] = {
		{MVFMT_YUY2, Check_YUY2},
		{MVFMT_UYVY, Check_UYVY},
		{MVFMT_YVYU, Check_YVYU},
		{MVFMT_VYUY, Check_VYUY},
		{MVFMT_IYU1, Check_IYU1},
		{MVFMT_IYU2, Check_IYU2},
		{MVFMT_Y800, Check_Y800},
		{MVFMT_UNDEFINED, nullptr}
	};

	TEST_CLASS(RGBtoVUVUnitTests)
	{
	public:

		TEST_METHOD(RGB32_to_YUY2_UnitTest)
		{
			RunTestSeries8BitUnpacked(MVFMT_RGB32, MVFMT_YUY2);
		}

		TEST_METHOD(RGB32_to_UYVY_UnitTest)
		{
			RunTestSeries8BitUnpacked(MVFMT_RGB32, MVFMT_UYVY);
		}

		TEST_METHOD(RGB32_to_YVYU_UnitTest)
		{
			RunTestSeries8BitUnpacked(MVFMT_RGB32, MVFMT_YVYU);
		}

		TEST_METHOD(RGB32_to_VYUY_UnitTest)
		{
			RunTestSeries8BitUnpacked(MVFMT_RGB32, MVFMT_VYUY);
		}

		TEST_METHOD(RGB32_to_IYU1_UnitTest)
		{
			RunTestSeries8BitUnpacked(MVFMT_RGB32, MVFMT_IYU1);
		}

		TEST_METHOD(RGB32_to_IYU2_UnitTest)
		{
			RunTestSeries8BitUnpacked(MVFMT_RGB32, MVFMT_IYU2);
		}

		TEST_METHOD(RGB32_to_Y800_UnitTest)
		{
			TestY800Results(MVFMT_RGB32, 128, 128, 128, 255);
			TestY800Results(MVFMT_RGB32, 255, 255, 255, 255);
			TestY800Results(MVFMT_RGB32, 0, 0, 0, 255);
			TestY800Results(MVFMT_RGB32, 255, 0, 0, 255);
			TestY800Results(MVFMT_RGB32, 0, 255, 0, 255);
			TestY800Results(MVFMT_RGB32, 0, 0, 255, 255);
		}

	private:
		t_yuvcheckfunc FindYUVCheckFunction(const MediaFormatID& target)
		{
			int16_t index = 0;
			while (YUVCheckFuncTable[index].pProcAddr != nullptr)
			{
				if (YUVCheckFuncTable[index].target == target)
				{
					return YUVCheckFuncTable[index].pProcAddr;
				}
				index++;
			}

			return nullptr;
		}

		void RunTestSeries8BitUnpacked(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat)
		{
			Test8BitUnpackedResults(rgbFormat, yuvFormat, 128, 128, 128, 255);
			Test8BitUnpackedResults(rgbFormat, yuvFormat, 255, 255, 255, 255);
			Test8BitUnpackedResults(rgbFormat, yuvFormat, 0, 0, 0, 255);
			Test8BitUnpackedResults(rgbFormat, yuvFormat, 255, 0, 0, 255);
			Test8BitUnpackedResults(rgbFormat, yuvFormat, 0, 255, 0, 255);
			Test8BitUnpackedResults(rgbFormat, yuvFormat, 0, 0, 255, 255);
		}

		void Test8BitUnpackedResults(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			// YUV to RGB
			t_transformfunc decodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(decodeTransPtr), L"decodeTransPtr returned a null function pointer.");

			t_yuvcheckfunc yuvCheckFunctPtr = FindYUVCheckFunction(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(yuvCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = 16;
			uint32_t height = 16;

			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height);

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);
			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			Fill_RGB32(red, green, blue, alpha, width, height, rgbBufPtr);

			encodeTransPtr(width, height, yuvBufPtr, 0, rgbBufPtr, 0, false, nullptr);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			Assert::IsTrue(yuvCheckFunctPtr(Y, U, V, width, height, yuvBufPtr, 0), L"YUV buffer did not contain expected values.");

			memset(rgbBufPtr, 0, rgbBufBize);
			decodeTransPtr(width, height, rgbBufPtr, 0, yuvBufPtr, 0, false, nullptr);

			uint8_t R1;
			uint8_t G1;
			uint8_t B1;
			uint8_t A1 = alpha;
			FastYUVtoRGB(Y, U, V, &R1, &G1, &B1);

			Assert::IsTrue(Check_RGB32(R1, G1, B1, A1, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}

		void TestY800Results(const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, MVFMT_Y800);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			// YUV to RGB
			t_transformfunc decodeTransPtr = FindVideoTransform(MVFMT_Y800, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(decodeTransPtr), L"decodeTransPtr returned a null function pointer.");

			t_yuvcheckfunc yuvCheckFunctPtr = FindYUVCheckFunction(MVFMT_Y800);
			Assert::IsNotNull(reinterpret_cast<void*>(yuvCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = 16;
			uint32_t height = 16;

			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(MVFMT_Y800, width, height);

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);
			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			Fill_RGB32(red, green, blue, alpha, width, height, rgbBufPtr);

			encodeTransPtr(width, height, yuvBufPtr, 0, rgbBufPtr, 0, false, nullptr);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			Assert::IsTrue(yuvCheckFunctPtr(Y, U, V, width, height, yuvBufPtr, 0), L"YUV buffer did not contain expected values.");

			memset(rgbBufPtr, 0, rgbBufBize);
			decodeTransPtr(width, height, rgbBufPtr, 0, yuvBufPtr, 0, false, nullptr);

			Assert::IsTrue(Check_RGB32(Y, Y, Y, 255, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}
	};
}