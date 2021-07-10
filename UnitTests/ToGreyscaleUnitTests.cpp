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
#include "ToGreyscale.h"
#include "ToFillColor.h"

#include "BufferChecks.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_CLASS(ToGreyscaleUnitTests)
	{
	public:

		TEST_METHOD(RGBA_UnitTest)
		{
			RunRGB8bitAlphaTestSeries(MVFMT_RGBA);
		}

		TEST_METHOD(RGB32_UnitTest)
		{
			RunRGB8bitTestSeries(MVFMT_RGB32);
		}

		TEST_METHOD(RGB24_UnitTest)
		{
			RunRGB8bitTestSeries(MVFMT_RGB24);
		}

		TEST_METHOD(RGB565_UnitTest)
		{
			RunRGB565TestSeries(MVFMT_RGB565);
		}

		TEST_METHOD(RGB555_UnitTest)
		{
			RunRGB555TestSeries(MVFMT_RGB555);
		}

		TEST_METHOD(UYVY_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_UYVY);
		}

		TEST_METHOD(YVYU_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_YVYU);
		}

		TEST_METHOD(VYUY_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_VYUY);
		}

		TEST_METHOD(YUY2_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_YUY2);
		}

		TEST_METHOD(IYUV_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_IYUV);
		}

		TEST_METHOD(YV12_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_YV12);
		}

		TEST_METHOD(YVU9_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_YVU9);
		}

		TEST_METHOD(YUV9_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_YUV9);
		}

		TEST_METHOD(IYU1_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_IYU1);
		}

		TEST_METHOD(IYU2_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_IYU2);
		}

		TEST_METHOD(Y41P_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_Y41P);
		}

		TEST_METHOD(CLJR_UnitTest)
		{
			RunYUV8bitTestSeries(MVFMT_CLJR);
		}

		TEST_METHOD(AYUV_UnitTest)
		{
			RunYUV8bitAlphaTestSeries(MVFMT_AYUV);
		}

	private:

		void RunRGB8bitTestSeries(const MediaFormatID& format)
		{
			RunRGBSingle8bitTest(format, 128, 128, 128, 255);
			RunRGBSingle8bitTest(format, 255, 255, 255, 255);
			RunRGBSingle8bitTest(format, 0, 0, 0, 255);
			RunRGBSingle8bitTest(format, 255, 0, 0, 255);
			RunRGBSingle8bitTest(format, 0, 255, 0, 255);
			RunRGBSingle8bitTest(format, 0, 0, 255, 255);
		}

		void RunRGB8bitAlphaTestSeries(const MediaFormatID& format)
		{
			RunRGBSingle8bitTest(format, 128, 128, 128, 255);
			RunRGBSingle8bitTest(format, 255, 255, 255, 255);
			RunRGBSingle8bitTest(format, 0, 0, 0, 255);
			RunRGBSingle8bitTest(format, 255, 0, 0, 255);
			RunRGBSingle8bitTest(format, 0, 255, 0, 255);
			RunRGBSingle8bitTest(format, 0, 0, 255, 255);

			RunRGBSingle8bitTest(format, 128, 128, 128, 0);
			RunRGBSingle8bitTest(format, 255, 255, 255, 0);
			RunRGBSingle8bitTest(format, 0, 0, 0, 0);
			RunRGBSingle8bitTest(format, 255, 0, 0, 0);
			RunRGBSingle8bitTest(format, 0, 255, 0, 0);
			RunRGBSingle8bitTest(format, 0, 0, 255, 0);
		}

		void RunRGBSingle8bitTest(const MediaFormatID& format, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			t_greyscalefunc encodeTransPtr = FindGreyscaleTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(format);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t bufBize = CalculateBufferSize(format, width, height);
			Assert::IsTrue(bufBize != 0, L"bufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> buf(new uint8_t[bufBize]);
			uint8_t* bufPtr = buf.get();
			memset(bufPtr, 0, bufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;

			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			fullBufFunctPtr(red, green, blue, alpha, width, height, bufPtr, 0);

			encodeTransPtr(width, height, bufPtr, 0, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, Y, Y, alpha, width, height, bufPtr, 0), L"Greyscale buffer did not contain expected values.");
		}

		void RunRGB565TestSeries(const MediaFormatID& format)
		{
			RunRGB565Single8bitTest(format, 128, 128, 128, 255);
			RunRGB565Single8bitTest(format, 255, 255, 255, 255);
			RunRGB565Single8bitTest(format, 0, 0, 0, 255);
			RunRGB565Single8bitTest(format, 255, 0, 0, 255);
			RunRGB565Single8bitTest(format, 0, 255, 0, 255);
			RunRGB565Single8bitTest(format, 0, 0, 255, 255);
		}

		void RunRGB565Single8bitTest(const MediaFormatID& format, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			t_greyscalefunc encodeTransPtr = FindGreyscaleTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(format);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t bufBize = CalculateBufferSize(format, width, height);
			Assert::IsTrue(bufBize != 0, L"bufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> buf(new uint8_t[bufBize]);
			uint8_t* bufPtr = buf.get();
			memset(bufPtr, 0, bufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;

			FastRGBtoYUV(red & 0xF8, green & 0xFC, blue & 0xF8, &Y, &U, &V);

			fullBufFunctPtr(red, green, blue, alpha, width, height, bufPtr, 0);

			encodeTransPtr(width, height, bufPtr, 0, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, Y , Y, alpha, width, height, bufPtr, 0), L"Greyscale buffer did not contain expected values.");
		}

		void RunRGB555TestSeries(const MediaFormatID& format)
		{
			RunRGB555Single8bitTest(format, 128, 128, 128, 255);
			RunRGB555Single8bitTest(format, 255, 255, 255, 255);
			RunRGB555Single8bitTest(format, 0, 0, 0, 255);
			RunRGB555Single8bitTest(format, 255, 0, 0, 255);
			RunRGB555Single8bitTest(format, 0, 255, 0, 255);
			RunRGB555Single8bitTest(format, 0, 0, 255, 255);

			RunRGB555Single8bitTest(format, 128, 128, 128, 0);
			RunRGB555Single8bitTest(format, 255, 255, 255, 0);
			RunRGB555Single8bitTest(format, 0, 0, 0, 0);
			RunRGB555Single8bitTest(format, 255, 0, 0, 0);
			RunRGB555Single8bitTest(format, 0, 255, 0, 0);
			RunRGB555Single8bitTest(format, 0, 0, 255, 0);
		}

		void RunRGB555Single8bitTest(const MediaFormatID& format, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			t_greyscalefunc encodeTransPtr = FindGreyscaleTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(format);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t bufBize = CalculateBufferSize(format, width, height);
			Assert::IsTrue(bufBize != 0, L"bufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> buf(new uint8_t[bufBize]);
			uint8_t* bufPtr = buf.get();
			memset(bufPtr, 0, bufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;

			FastRGBtoYUV(red & 0xF8, green & 0xFC, blue & 0xF8, &Y, &U, &V);

			fullBufFunctPtr(red, green, blue, alpha, width, height, bufPtr, 0);

			encodeTransPtr(width, height, bufPtr, 0, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, Y, Y, alpha, width, height, bufPtr, 0), L"Greyscale buffer did not contain expected values.");
		}

		void RunYUV8bitAlphaTestSeries(const MediaFormatID& format)
		{
			RunYUVSingle8bitTest(format, 128, 128, 128, 255);
			RunYUVSingle8bitTest(format, 255, 255, 255, 255);
			RunYUVSingle8bitTest(format, 0, 0, 0, 255);
			RunYUVSingle8bitTest(format, 255, 0, 0, 255);
			RunYUVSingle8bitTest(format, 0, 255, 0, 255);
			RunYUVSingle8bitTest(format, 0, 0, 255, 255);

			RunYUVSingle8bitTest(format, 128, 128, 128, 0);
			RunYUVSingle8bitTest(format, 255, 255, 255, 0);
			RunYUVSingle8bitTest(format, 0, 0, 0, 0);
			RunYUVSingle8bitTest(format, 255, 0, 0, 0);
			RunYUVSingle8bitTest(format, 0, 255, 0, 0);
			RunYUVSingle8bitTest(format, 0, 0, 255, 0);
		}

		void RunYUV8bitTestSeries(const MediaFormatID& format)
		{
			RunYUVSingle8bitTest(format, 128, 128, 128, 255);
			RunYUVSingle8bitTest(format, 255, 255, 255, 255);
			RunYUVSingle8bitTest(format, 0, 0, 0, 255);
			RunYUVSingle8bitTest(format, 255, 0, 0, 255);
			RunYUVSingle8bitTest(format, 0, 255, 0, 255);
			RunYUVSingle8bitTest(format, 0, 0, 255, 255);
		}

		void RunYUVSingle8bitTest(const MediaFormatID& format, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			t_greyscalefunc encodeTransPtr = FindGreyscaleTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(format);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(format);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t bufBize = CalculateBufferSize(format, width, height);
			Assert::IsTrue(bufBize != 0, L"bufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> buf(new uint8_t[bufBize]);
			uint8_t* bufPtr = buf.get();
			memset(bufPtr, 0, bufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;

			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			fullBufFunctPtr(Y, U, V, alpha, width, height, bufPtr, 0);

			encodeTransPtr(width, height, bufPtr, 0, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, 0, 0, alpha, width, height, bufPtr, 0), L"Greyscale buffer did not contain expected values.");
		}
	};
}
