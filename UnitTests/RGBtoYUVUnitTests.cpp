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
	TEST_CLASS(RGBtoYUVUnitTests)
	{
	public:

		//
		// RGBA to YUV
		//

		TEST_METHOD(RGBA_to_AYUV_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_RGBA, MVFMT_AYUV);
		}

		TEST_METHOD(RGBA_to_Y42T_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_RGBA, MVFMT_Y42T);
		}

		TEST_METHOD(RGBA_to_Y41T_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_RGBA, MVFMT_Y41T);
		}

		//
		// RGB32 to YUV
		//

		TEST_METHOD(RGB32_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_YUY2);
		}

		TEST_METHOD(RGB32_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_UYVY);
		}

		TEST_METHOD(RGB32_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_YVYU);
		}

		TEST_METHOD(RGB32_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_VYUY);
		}

		TEST_METHOD(RGB32_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_I420);
		}

		TEST_METHOD(RGB32_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_YV12);
		}

		TEST_METHOD(RGB32_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_YVU9);
		}

		TEST_METHOD(RGB32_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_YUV9);
		}

		TEST_METHOD(RGB32_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_IYU1);
		}

		TEST_METHOD(RGB32_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_IYU2);
		}

		TEST_METHOD(RGB32_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_Y41P);
		}

		TEST_METHOD(RGB32_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_CLJR);
		}

		TEST_METHOD(RGB32_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_Y800);
		}

		TEST_METHOD(RGB32_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_Y16);
		}

		TEST_METHOD(RGB32_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_AYUV);
		}

		TEST_METHOD(RGB32_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_IMC1);
		}

		TEST_METHOD(RGB32_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_IMC2);
		}

		TEST_METHOD(RGB32_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_IMC3);
		}

		TEST_METHOD(RGB32_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_IMC4);
		}

		TEST_METHOD(RGB32_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_NV12);
		}

		TEST_METHOD(RGB32_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_Y42T);
		}

		TEST_METHOD(RGB32_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_Y42T);
		}

		//
		// RGB24 to YUV
		//

		TEST_METHOD(RGB24_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_YUY2);
		}

		TEST_METHOD(RGB24_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_UYVY);
		}

		TEST_METHOD(RGB24_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_YVYU);
		}

		TEST_METHOD(RGB24_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_VYUY);
		}

		TEST_METHOD(RGB24_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_I420);
		}

		TEST_METHOD(RGB24_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_YV12);
		}

		TEST_METHOD(RGB24_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_YVU9);
		}

		TEST_METHOD(RGB24_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_YUV9);
		}

		TEST_METHOD(RGB24_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_IYU1);
		}

		TEST_METHOD(RGB24_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_IYU2);
		}

		TEST_METHOD(RGB24_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_Y41P);
		}

		TEST_METHOD(RGB24_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_CLJR);
		}

		TEST_METHOD(RGB24_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_Y800);
		}

		TEST_METHOD(RGB24_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_Y16);
		}

		TEST_METHOD(RGB24_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_AYUV);
		}

		TEST_METHOD(RGB24_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_IMC1);
		}

		TEST_METHOD(RGB24_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_IMC2);
		}

		TEST_METHOD(RGB24_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_IMC3);
		}

		TEST_METHOD(RGB24_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_IMC4);
		}

		TEST_METHOD(RGB24_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_NV12);
		}

		TEST_METHOD(RGB24_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_Y42T);
		}

		TEST_METHOD(RGB24_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_Y41T);
		}

		//
		// RGB565 to YUV
		//

		TEST_METHOD(RGB565_to_YUY2_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_YUY2);
		}

		TEST_METHOD(RGB565_to_UYVY_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_UYVY);
		}

		TEST_METHOD(RGB565_to_YVYU_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_YVYU);
		}

		TEST_METHOD(RGB565_to_VYUY_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_VYUY);
		}

		TEST_METHOD(RGB565_to_I420_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_I420);
		}

		TEST_METHOD(RGB565_to_YV12_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_YV12);
		}

		TEST_METHOD(RGB565_to_YVU9_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_YVU9);
		}

		TEST_METHOD(RGB565_to_YUV9_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_YUV9);
		}

		TEST_METHOD(RGB565_to_IYU1_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_IYU1);
		}

		TEST_METHOD(RGB565_to_IYU2_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_IYU2);
		}

		TEST_METHOD(RGB565_to_Y41P_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_Y41P);
		}

		TEST_METHOD(RGB565_to_CLJR_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_CLJR);
		}

		TEST_METHOD(RGB565_to_Y800_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_Y800);
		}

		TEST_METHOD(RGB565_to_Y16_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_Y16);
		}

		TEST_METHOD(RGB565_to_AYUV_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_AYUV);
		}

		TEST_METHOD(RGB565_to_IMC1_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_IMC1);
		}

		TEST_METHOD(RGB565_to_IMC2_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_IMC2);
		}

		TEST_METHOD(RGB565_to_IMC3_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_IMC3);
		}

		TEST_METHOD(RGB565_to_IMC4_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_IMC4);
		}

		TEST_METHOD(RGB565_to_NV12_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_NV12);
		}

		TEST_METHOD(RGB565_to_Y42T_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_Y42T);
		}

		TEST_METHOD(RGB565_to_Y41T_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_Y41T);
		}

		//
		// RGB555 to YUV
		//

		TEST_METHOD(RGB555_to_YUY2_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_YUY2);
		}

		TEST_METHOD(RGB555_to_UYVY_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_UYVY);
		}

		TEST_METHOD(RGB555_to_YVYU_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_YVYU);
		}

		TEST_METHOD(RGB555_to_VYUY_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_VYUY);
		}

		TEST_METHOD(RGB555_to_I420_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_I420);
		}

		TEST_METHOD(RGB555_to_YV12_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_YV12);
		}

		TEST_METHOD(RGB555_to_YVU9_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_YVU9);
		}

		TEST_METHOD(RGB555_to_YUV9_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_YUV9);
		}

		TEST_METHOD(RGB555_to_IYU1_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_IYU1);
		}

		TEST_METHOD(RGB555_to_IYU2_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_IYU2);
		}

		TEST_METHOD(RGB555_to_Y41P_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_Y41P);
		}

		TEST_METHOD(RGB555_to_CLJR_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_CLJR);
		}

		TEST_METHOD(RGB555_to_Y800_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_Y800);
		}

		TEST_METHOD(RGB555_to_Y16_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_Y16);
		}

		TEST_METHOD(RGB555_to_AYUV_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_AYUV);
		}

		TEST_METHOD(RGB555_to_IMC1_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_IMC1);
		}

		TEST_METHOD(RGB555_to_IMC2_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_IMC2);
		}

		TEST_METHOD(RGB555_to_IMC3_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_IMC3);
		}

		TEST_METHOD(RGB555_to_IMC4_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_IMC4);
		}

		TEST_METHOD(RGB555_to_NV12_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_NV12);
		}

		TEST_METHOD(RGB555_to_Y42T_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_Y42T);
		}

		TEST_METHOD(RGB555_to_Y41T_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_Y41T);
		}

		//
		// ARGB1555 to RGB
		//

		TEST_METHOD(ARGB1555_to_AYUV_UnitTest)
		{
			Run555AlphabitTestSeries(MVFMT_ARGB1555, MVFMT_AYUV);
		}

		TEST_METHOD(ARGB1555_to_Y42T_UnitTest)
		{
			Run555AlphabitTestSeries(MVFMT_ARGB1555, MVFMT_Y42T);
		}

		TEST_METHOD(ARGB1555_to_Y41T_UnitTest)
		{
			Run555AlphabitTestSeries(MVFMT_ARGB1555, MVFMT_Y41T);
		}

		//
		// RGB8 to YUV
		//

		TEST_METHOD(RGB8_to_YUY2_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_YUY2);
		}

		TEST_METHOD(RGB8_to_UYVY_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_UYVY);
		}

		TEST_METHOD(RGB8_to_YVYU_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_YVYU);
		}

		TEST_METHOD(RGB8_to_VYUY_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_VYUY);
		}

		TEST_METHOD(RGB8_to_I420_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_I420);
		}

		TEST_METHOD(RGB8_to_YV12_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_YV12);
		}

		TEST_METHOD(RGB8_to_YVU9_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_YVU9);
		}

		TEST_METHOD(RGB8_to_YUV9_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_YUV9);
		}

		TEST_METHOD(RGB8_to_IYU1_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_IYU1);
		}

		TEST_METHOD(RGB8_to_IYU2_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_IYU2);
		}

		TEST_METHOD(RGB8_to_Y41P_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_Y41P);
		}

		TEST_METHOD(RGB8_to_CLJR_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_CLJR);
		}

		TEST_METHOD(RGB8_to_Y800_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_Y800);
		}

		TEST_METHOD(RGB8_to_Y16_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_Y16);
		}

		TEST_METHOD(RGB8_to_AYUV_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_AYUV);
		}

		TEST_METHOD(RGB8_to_IMC1_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_IMC1);
		}

		TEST_METHOD(RGB8_to_IMC2_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_IMC2);
		}

		TEST_METHOD(RGB8_to_IMC3_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_IMC3);
		}

		TEST_METHOD(RGB8_to_IMC4_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_IMC4);
		}

		TEST_METHOD(RGB8_to_Y42T_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_Y42T);
		}

		TEST_METHOD(RGB8_to_Y41T_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_Y41T);
		}

	private:

		void Run8bitTestSeries(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat)
		{
			RunSingle8bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			RunSingle8bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			StrideBump = saveb;
		}

		void Run8bitAlphaTestSeries(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat)
		{
			RunSingle8bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			RunSingle8bitTest(rgbFormat, yuvFormat, 128, 128, 128, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 255, 255, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 0, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 0, 0, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 255, 0, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 255, 0);

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			RunSingle8bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			RunSingle8bitTest(rgbFormat, yuvFormat, 128, 128, 128, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 255, 255, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 0, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 255, 0, 0, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 255, 0, 0);
			RunSingle8bitTest(rgbFormat, yuvFormat, 0, 0, 255, 0);

			StrideBump = saveb;
		}

		void RunSingle8bitTest(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, in_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, out_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);
			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			fillBufFunctPtr(red, green, blue, alpha, width, height, rgbBufPtr, in_stride);

			encodeTransPtr(width, height, yuvBufPtr, out_stride, rgbBufPtr, in_stride, false, nullptr);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			Assert::IsTrue(bufCheckFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}

		void Run565bitTestSeries(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat)
		{
			RunSingle565bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			RunSingle565bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle565bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			StrideBump = saveb;
		}

		void RunSingle565bitTest(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, in_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, out_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);
			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			fillBufFunctPtr(red, green, blue, alpha, width, height, rgbBufPtr, in_stride);

			encodeTransPtr(width, height, yuvBufPtr, out_stride, rgbBufPtr, in_stride, false, nullptr);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red & 0xF8, green & 0xFC, blue & 0xF8, &Y, &U, &V);

			Assert::IsTrue(bufCheckFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}

		void Run555bitTestSeries(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat)
		{
			RunSingle555bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			RunSingle555bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			StrideBump = saveb;
		}

		void Run555AlphabitTestSeries(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat)
		{
			// Alpha/transparency bit set
			RunSingle555bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			// Alpha/transparency bit clear
			RunSingle555bitTest(rgbFormat, yuvFormat, 128, 128, 128, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 255, 255, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 0, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 0, 0, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 255, 0, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 255, 0);

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			// Alpha/transparency bit set
			RunSingle555bitTest(rgbFormat, yuvFormat, 128, 128, 128, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 255, 255, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 0, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 255, 0, 255);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 255, 255);

			// Alpha/transparency bit clear
			RunSingle555bitTest(rgbFormat, yuvFormat, 128, 128, 128, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 255, 255, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 0, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 255, 0, 0, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 255, 0, 0);
			RunSingle555bitTest(rgbFormat, yuvFormat, 0, 0, 255, 0);

			StrideBump = saveb;
		}

		void RunSingle555bitTest(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(rgbFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, in_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, out_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);

			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			fillBufFunctPtr(red, green, blue, alpha, width, height, rgbBufPtr, in_stride);

			encodeTransPtr(width, height, yuvBufPtr, out_stride, rgbBufPtr, in_stride, false, nullptr);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red & 0xF8, green & 0xF8, blue & 0xF8, &Y, &U, &V);

			Assert::IsTrue(bufCheckFunctPtr(Y, U, V, alpha, width, height, yuvBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}

		void Run8bitPalletizedTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			Run8bitPalletizedTest(inFormat, outFormat, 0);
			Run8bitPalletizedTest(inFormat, outFormat, 1);
			Run8bitPalletizedTest(inFormat, outFormat, 2);
			Run8bitPalletizedTest(inFormat, outFormat, 3);
			Run8bitPalletizedTest(inFormat, outFormat, 4);
			Run8bitPalletizedTest(inFormat, outFormat, 5);

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			Run8bitPalletizedTest(inFormat, outFormat, 0);
			Run8bitPalletizedTest(inFormat, outFormat, 1);
			Run8bitPalletizedTest(inFormat, outFormat, 2);
			Run8bitPalletizedTest(inFormat, outFormat, 3);
			Run8bitPalletizedTest(inFormat, outFormat, 4);
			Run8bitPalletizedTest(inFormat, outFormat, 5);

			StrideBump = saveb;
		}

		void Run8bitPalletizedTest(const MediaFormatID& rgbFormat, const MediaFormatID& yuvFormat, uint8_t index)
		{
			static xRGBQUAD  rgbpalette[6] = {
				{128, 128, 128, 255},
				{255, 255, 255, 25},
				{0, 0, 0, 25},
				{255, 0, 0, 255},
				{0, 255, 0, 255},
				{0, 0, 255, 255}
			};

			Assert::IsTrue(IsPalletizedEncoding(rgbFormat), L"Expected a palletized input format.");

			// RGB to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(rgbFormat, yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(MVFMT_Y800);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(yuvFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(rgbFormat, width, height);
			uint32_t rgbBufBize = CalculateBufferSize(rgbFormat, width, height, in_stride);
			Assert::IsTrue(rgbBufBize != 0, L"RGB buffer size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(yuvFormat, width, height);
			uint32_t yuvBufBize = CalculateBufferSize(yuvFormat, width, height, out_stride);
			Assert::IsTrue(yuvBufBize != 0, L"YUV buffer size retuned zero.");

			std::unique_ptr<uint8_t[]> rgbBuf(new uint8_t[rgbBufBize]);
			uint8_t* rgbBufPtr = rgbBuf.get();
			memset(rgbBufPtr, 0, rgbBufBize);

			std::unique_ptr<uint8_t[]> yuvBuf(new uint8_t[yuvBufBize]);
			uint8_t* yuvBufPtr = yuvBuf.get();
			memset(yuvBufPtr, 0, yuvBufBize);

			fillBufFunctPtr(index, 0, 0, 255, width, height, rgbBufPtr, in_stride);

			encodeTransPtr(width, height, yuvBufPtr, out_stride, rgbBufPtr, in_stride, false, rgbpalette);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(rgbpalette[index].rgbRed, rgbpalette[index].rgbGreen, rgbpalette[index].rgbBlue, &Y, &U, &V);

			Assert::IsTrue(bufCheckFunctPtr(Y, U, V, 255, width, height, yuvBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}
	};
}