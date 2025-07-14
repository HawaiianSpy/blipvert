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
		// CLJR to RGB
		//

		TEST_METHOD(CLJR_to_RGB32_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_RGB32);
		}

		TEST_METHOD(CLJR_to_RGB24_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_RGB24);
		}

		TEST_METHOD(CLJR_to_RGB565_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_RGB565);
		}

		TEST_METHOD(CLJR_to_RGB555_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_RGB555);
		}

		//
		// Y41P to RGB
		//

		TEST_METHOD(Y41P_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_RGB32);
		}

		TEST_METHOD(Y41P_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_RGB24);
		}

		TEST_METHOD(Y41P_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_Y41P, MVFMT_RGB565);
		}

		TEST_METHOD(Y41P_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_Y41P, MVFMT_RGB555);
		}

		//
		// IYU1 to RGB
		//

		TEST_METHOD(IYU1_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_RGB32);
		}

		TEST_METHOD(IYU1_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_RGB24);
		}

		TEST_METHOD(IYU1_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_IYU1, MVFMT_RGB565);
		}

		TEST_METHOD(IYU1_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_IYU1, MVFMT_RGB555);
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
		// I420 to RGB
		//

		TEST_METHOD(I420_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_RGB32);
		}

		TEST_METHOD(I420_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_RGB24);
		}

		TEST_METHOD(I420_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_I420, MVFMT_RGB565);
		}

		TEST_METHOD(I420_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_I420, MVFMT_RGB555);
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
		// Y800 to RGB
		//

		TEST_METHOD(Y800_to_RGB32_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_RGB32);
		}

		TEST_METHOD(Y800_to_RGB24_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_RGB24);
		}

		TEST_METHOD(Y800_to_RGB565_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_RGB565);
		}

		TEST_METHOD(Y800_to_RGB555_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_RGB555);
		}

		//
		// Y16 to RGB
		//

		TEST_METHOD(Y16_to_RGB32_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_RGB32);
		}

		TEST_METHOD(Y16_to_RGB24_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_RGB24);
		}

		TEST_METHOD(Y16_to_RGB565_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_RGB565);
		}

		TEST_METHOD(Y16_to_RGB555_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_RGB555);
		}

		//
		// AYUV to RGB
		//

		TEST_METHOD(AYUV_to_RGBA_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_RGBA);
		}

		TEST_METHOD(AYUV_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_RGB32);
		}

		TEST_METHOD(AYUV_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_RGB24);
		}

		TEST_METHOD(AYUV_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_AYUV, MVFMT_RGB565);
		}

		TEST_METHOD(AYUV_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_AYUV, MVFMT_RGB555);
		}

		TEST_METHOD(AYUV_to_ARGB1555_UnitTest)
		{
			Run555AlphabitTestSeries(MVFMT_AYUV, MVFMT_ARGB1555);
		}

		//
		// IMC1 to RGB
		//

		TEST_METHOD(IMC1_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_RGB32);
		}

		TEST_METHOD(IMC1_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_RGB24);
		}

		TEST_METHOD(IMC1_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_RGB565);
		}

		TEST_METHOD(IMC1_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_RGB555);
		}

		//
		// IMC2 to RGB
		//

		TEST_METHOD(IMC2_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_RGB32);
		}

		TEST_METHOD(IMC2_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_RGB24);
		}

		TEST_METHOD(IMC2_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_RGB565);
		}

		TEST_METHOD(IMC2_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_RGB555);
		}

		//
		// IMC3 to RGB
		//

		TEST_METHOD(IMC3_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_RGB32);
		}

		TEST_METHOD(IMC3_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_RGB24);
		}

		TEST_METHOD(IMC3_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_RGB565);
		}

		TEST_METHOD(IMC3_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_RGB555);
		}

		//
		// IMC4 to RGB
		//

		TEST_METHOD(IMC4_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_RGB32);
		}

		TEST_METHOD(IMC4_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_RGB24);
		}

		TEST_METHOD(IMC4_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_RGB565);
		}

		TEST_METHOD(IMC4_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_RGB555);
		}

		//
		// NV12 to RGB
		//

		TEST_METHOD(NV12_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_RGB32);
		}

		TEST_METHOD(NV12_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_RGB24);
		}

		TEST_METHOD(NV12_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_RGB565);
		}

		TEST_METHOD(NV12_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_RGB555);
		}

		//
		// NV21 to RGB
		//

		TEST_METHOD(NV21_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_RGB32);
		}

		TEST_METHOD(NV21_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_RGB24);
		}

		TEST_METHOD(NV21_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_RGB565);
		}

		TEST_METHOD(NV21_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_RGB555);
		}

		//
		// Y42T to RGB
		//

		TEST_METHOD(Y42T_to_RGBA_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y42T, MVFMT_RGBA);
		}

		TEST_METHOD(Y42T_to_RGB32_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y42T, MVFMT_RGB32);
		}

		TEST_METHOD(Y42T_to_RGB24_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y42T, MVFMT_RGB24);
		}

		TEST_METHOD(Y42T_to_RGB565_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y42T, MVFMT_RGB565);
		}

		TEST_METHOD(Y42T_to_RGB555_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y42T, MVFMT_RGB555);
		}

		TEST_METHOD(Y42T_to_ARGB1555_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y42T, MVFMT_ARGB1555);
		}

		//
		// Y41T to RGB
		//

		TEST_METHOD(Y41T_to_RGBA_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y41T, MVFMT_RGBA);
		}

		TEST_METHOD(Y41T_to_RGB32_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y41T, MVFMT_RGB32);
		}

		TEST_METHOD(Y41T_to_RGB24_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y41T, MVFMT_RGB24);
		}

		TEST_METHOD(Y41T_to_RGB565_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y41T, MVFMT_RGB565);
		}

		TEST_METHOD(Y41T_to_RGB555_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y41T, MVFMT_RGB555);
		}

		TEST_METHOD(Y41T_to_ARGB1555_UnitTest)
		{
			Run8bitYAlphaTestSeries(MVFMT_Y41T, MVFMT_ARGB1555);
		}

		//
		// YV12 to RGB
		//

		TEST_METHOD(YV16_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_RGB32);
		}

		TEST_METHOD(YV16_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_RGB24);
		}

		TEST_METHOD(YV16_to_RGB565_UnitTest)
		{
			Run565bitTestSeries(MVFMT_YV16, MVFMT_RGB565);
		}

		TEST_METHOD(YV16_to_RGB555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_YV12, MVFMT_RGB555);
		}

	private:
		void Run8bitYAlphaTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingleYAlphaTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingleYAlphaTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void Run8bitAlphaTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle8bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle8bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void Run8bitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle8bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle8bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingle8bitTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, in_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, out_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for yuvFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, yuvBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for rgbFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, rgbBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void RunSingleYAlphaTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, in_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, out_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for yuvFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, yuvBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for rgbFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, rgbBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y & 0xFE, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run565bitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle565bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle565bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingle565bitTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, in_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, out_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for yuvFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, yuvBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for rgbFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, rgbBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run555bitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle555bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle555bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void Run555AlphabitTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle555bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle555bitTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingle555bitTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, in_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, out_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for yuvFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, yuvBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for rgbFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, rgbBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y, U, V, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void RunGreyscaleTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleGreyscaleTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleGreyscaleTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingleGreyscaleTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, in_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, out_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for yuvFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, yuvBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for rgbFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, rgbBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, Y, Y, alpha, width, height, rgbBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void RunCLJRTestSeries(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleCLJRTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleCLJRTest(yuvFormat, rgbFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingleCLJRTest(const MediaFormatID& yuvFormat, const MediaFormatID& rgbFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(yuvFormat, rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, in_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, out_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for yuvFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, yuvBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for rgbFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, rgbBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			uint8_t R;
			uint8_t G;
			uint8_t B;
			FastYUVtoRGB(Y & 0xF8, U & 0xFC, V & 0xFC, &R, &G, &B);

			Assert::IsTrue(bufCheckFunctPtr(R, G, B, alpha, width, height, rgbBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}
	};
}