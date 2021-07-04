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
#include "CppUnitTest.h"

#include "blipvert.h"
#include "Utilities.h"
#include "RGBtoYUV.h"
#include "YUVtoRGB.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"
#include "ToFillColor.h"

#include "BufferChecks.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_CLASS(YUVtoRGBUnitTests)
	{
	public:

		//
		// YUY2 to RGB
		//

		TEST_METHOD(YUY2_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_RGB32);
		}

		TEST_METHOD(YUY2_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_RGB24);
		}

		TEST_METHOD(YUY2_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_YUY2, MVFMT_RGB565);
		}

		TEST_METHOD(YUY2_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_YUY2, MVFMT_RGB555);
		}

		//
		// UYVY to RGB
		//

		TEST_METHOD(UYVY_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_RGB32);
		}

		TEST_METHOD(UYVY_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_RGB24);
		}

		TEST_METHOD(UYVY_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_UYVY, MVFMT_RGB565);
		}

		TEST_METHOD(UYVY_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_UYVY, MVFMT_RGB555);
		}

		//
		// YVYU to RGB
		//

		TEST_METHOD(YVYU_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_RGB32);
		}

		TEST_METHOD(YVYU_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_RGB24);
		}

		TEST_METHOD(YVYU_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_YVYU, MVFMT_RGB565);
		}

		TEST_METHOD(YVYU_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_YVYU, MVFMT_RGB555);
		}

		//
		// VYUY to RGB
		//

		TEST_METHOD(VYUY_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_RGB32);
		}

		TEST_METHOD(VYUY_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_RGB24);
		}

		TEST_METHOD(VYUY_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_VYUY, MVFMT_RGB565);
		}

		TEST_METHOD(VYUY_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_VYUY, MVFMT_RGB555);
		}

		//
		// IYUV to RGB
		//

		TEST_METHOD(IYUV_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_RGB32);
		}

		TEST_METHOD(IYUV_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_RGB24);
		}

		TEST_METHOD(IYUV_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_IYUV, MVFMT_RGB565);
		}

		TEST_METHOD(IYUV_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_IYUV, MVFMT_RGB555);
		}

		//
		// YV12 to RGB
		//

		TEST_METHOD(YV12_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_RGB32);
		}

		TEST_METHOD(YV12_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_RGB24);
		}

		TEST_METHOD(YV12_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_YV12, MVFMT_RGB565);
		}

		TEST_METHOD(YV12_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_YV12, MVFMT_RGB555);
		}

		//
		// YUV9 to RGB
		//

		TEST_METHOD(YUV9_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_RGB32);
		}

		TEST_METHOD(YUV9_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_RGB24);
		}

		TEST_METHOD(YUV9_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_YUV9, MVFMT_RGB565);
		}

		TEST_METHOD(YUV9_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_YUV9, MVFMT_RGB555);
		}

		//
		// YVU9 to RGB
		//

		TEST_METHOD(YVU9_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_RGB32);
		}

		TEST_METHOD(YVU9_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_RGB24);
		}

		TEST_METHOD(YVU9_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_YVU9, MVFMT_RGB565);
		}

		TEST_METHOD(YVU9_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_YVU9, MVFMT_RGB555);
		}

		//
		// IYU2 to RGB
		//

		TEST_METHOD(IYU2_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_RGB32);
		}

		TEST_METHOD(IYU2_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_RGB24);
		}

		TEST_METHOD(IYU2_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_IYU2, MVFMT_RGB565);
		}

		TEST_METHOD(IYU2_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_IYU2, MVFMT_RGB555);
		}

	private:
		void Run8bitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			RunSingle8bitTest(yuvFormat, rgbFormat, 128, 128, 128, 255);
			RunSingle8bitTest(yuvFormat, rgbFormat, 255, 255, 255, 255);
			RunSingle8bitTest(yuvFormat, rgbFormat, 0, 0, 0, 255);
			RunSingle8bitTest(yuvFormat, rgbFormat, 255, 0, 0, 255);
			RunSingle8bitTest(yuvFormat, rgbFormat, 0, 255, 0, 255);
			RunSingle8bitTest(yuvFormat, rgbFormat, 0, 0, 255, 255);
		}

		void RunSingle8bitTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height);

			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			fullBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, 0);

			encodeTransPtr(width, height, rgbBufPtr, 0, yuvBufPtr, 0, false, nullptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}

		void Run565bitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			RunSingle565bitTest(yuvFormat, rgbFormat, 128, 128, 128, 255);
			RunSingle565bitTest(yuvFormat, rgbFormat, 255, 255, 255, 255);
			RunSingle565bitTest(yuvFormat, rgbFormat, 0, 0, 0, 255);
			RunSingle565bitTest(yuvFormat, rgbFormat, 255, 0, 0, 255);
			RunSingle565bitTest(yuvFormat, rgbFormat, 0, 255, 0, 255);
			RunSingle565bitTest(yuvFormat, rgbFormat, 0, 0, 255, 255);
		}

		void RunSingle565bitTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height);

			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red & 0xF8, green & 0xFC, blue & 0xF8, &Y, &U, &V);

			fullBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, 0);

			encodeTransPtr(width, height, rgbBufPtr, 0, yuvBufPtr, 0, false, nullptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}

		void Run555bitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			RunSingle555bitTest(yuvFormat, rgbFormat, 128, 128, 128, 255);
			RunSingle555bitTest(yuvFormat, rgbFormat, 255, 255, 255, 255);
			RunSingle555bitTest(yuvFormat, rgbFormat, 0, 0, 0, 255);
			RunSingle555bitTest(yuvFormat, rgbFormat, 255, 0, 0, 255);
			RunSingle555bitTest(yuvFormat, rgbFormat, 0, 255, 0, 255);
			RunSingle555bitTest(yuvFormat, rgbFormat, 0, 0, 255, 255);
		}

		void RunSingle555bitTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height);

			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red , green, blue, &Y, &U, &V);

			fullBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, 0);

			encodeTransPtr(width, height, rgbBufPtr, 0, yuvBufPtr, 0, false, nullptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, 0), L"RGB buffer did not contain expected values.");
		}
	};
}