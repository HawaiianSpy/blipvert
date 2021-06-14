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

		//TEST_METHOD(RGB32_to_IYUV_UnitTest)
		//{
		//}

		//TEST_METHOD(RGB32_to_YV12_UnitTest)
		//{
		//}

		//TEST_METHOD(RGB32_to_YVU9_UnitTest)
		//{
		//}

		//TEST_METHOD(RGB32_to_YUV9_UnitTest)
		//{
		//}

		TEST_METHOD(RGB32_to_Y800_UnitTest)
		{
			TestY800Results(MVFMT_RGB32, 128, 128, 128, 255);
			TestY800Results(MVFMT_RGB32, 255, 255, 255, 255);
			TestY800Results(MVFMT_RGB32, 0, 0, 0, 255);
			TestY800Results(MVFMT_RGB32, 255, 0, 0, 255);
			TestY800Results(MVFMT_RGB32, 0, 255, 0, 255);
			TestY800Results(MVFMT_RGB32, 0, 0, 255, 255);
		}

		//TEST_METHOD(RGB32_to_Y41P_UnitTest)
		//{
		//}

		//TEST_METHOD(RGB32_to_CLJR_UnitTest)
		//{
		//}

	private:
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

			t_rgbcheckfunc rgbCheckFunctPtr = FindRGBCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(rgbCheckFunctPtr), L"rgbCheckFunctPtr returned a null function pointer.");

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

			Assert::IsTrue(rgbCheckFunctPtr(R1, G1, B1, A1, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}

		void TestY800Results(const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, MVFMT_Y800);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			// YUV to RGB
			t_transformfunc decodeTransPtr = FindVideoTransform(MVFMT_Y800, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(decodeTransPtr), L"decodeTransPtr returned a null function pointer.");

			t_rgbcheckfunc rgbCheckFunctPtr = FindRGBCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(rgbCheckFunctPtr), L"rgbCheckFunctPtr returned a null function pointer.");

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

			uint8_t R1;
			uint8_t G1;
			uint8_t B1;
			uint8_t A1 = alpha;
			FastYUVtoRGB(Y, U, V, &R1, &G1, &B1);

			Assert::IsTrue(rgbCheckFunctPtr(Y, Y, Y, 255, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}
	};
}