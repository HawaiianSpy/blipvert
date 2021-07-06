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
#include "YUVtoYUV.h"
#include "ToFillColor.h"

#include "BufferChecks.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_CLASS(YUVtoYUVUnitTests)
	{
	public:

		//
		// AYUV to YUVx
		//

		TEST_METHOD(AYUV_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_YUY2);
		}

		TEST_METHOD(AYUV_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_UYVY);
		}

		TEST_METHOD(AYUV_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_YVYU);
		}

		TEST_METHOD(AYUV_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_VYUY);
		}

		TEST_METHOD(AYUV_to_IYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IYUV);
		}

		TEST_METHOD(AYUV_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_YV12);
		}

		TEST_METHOD(AYUV_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_YVU9);
		}

		TEST_METHOD(AYUV_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_YUV9);
		}

		TEST_METHOD(AYUV_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IYU1);
		}

		TEST_METHOD(AYUV_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IYU2);
		}

		TEST_METHOD(AYUV_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_Y800);
		}

		TEST_METHOD(AYUV_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_Y16);
		}

		TEST_METHOD(AYUV_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_CLJR);
		}

		TEST_METHOD(AYUV_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_Y41P);
		}

		//
		// YUY2 to YUVx
		//

		TEST_METHOD(YUY2_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_UYVY);
		}

		TEST_METHOD(YUY2_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_YVYU);
		}

		TEST_METHOD(YUY2_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_VYUY);
		}

		TEST_METHOD(YUY2_to_IYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IYUV);
		}

		TEST_METHOD(YUY2_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_YV12);
		}

		TEST_METHOD(YUY2_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_YVU9);
		}

		TEST_METHOD(YUY2_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_YUV9);
		}

		TEST_METHOD(YUY2_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IYU1);
		}

		TEST_METHOD(YUY2_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IYU2);
		}

		TEST_METHOD(YUY2_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_Y800);
		}

		TEST_METHOD(YUY2_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_Y16);
		}

		TEST_METHOD(YUY2_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_CLJR);
		}

		TEST_METHOD(YUY2_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_Y41P);
		}

		TEST_METHOD(YUY2_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_AYUV);
		}

		//
		// UYVY to YUVx
		//

		TEST_METHOD(UYVY_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_YUY2);
		}

		TEST_METHOD(UYVY_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_YVYU);
		}

		TEST_METHOD(UYVY_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_VYUY);
		}

		TEST_METHOD(UYVY_to_IYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IYUV);
		}

		TEST_METHOD(UYVY_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_YV12);
		}

		TEST_METHOD(UYVY_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_YVU9);
		}

		TEST_METHOD(UYVY_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_YUV9);
		}

		TEST_METHOD(UYVY_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IYU1);
		}

		TEST_METHOD(UYVY_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IYU2);
		}

		TEST_METHOD(UYVY_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_Y800);
		}

		TEST_METHOD(UYVY_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_Y16);
		}

		TEST_METHOD(UYVY_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_CLJR);
		}

		TEST_METHOD(UYVY_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_Y41P);
		}

		TEST_METHOD(UYVY_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_AYUV);
		}

		//
		// UYVY to YUVx
		//

		TEST_METHOD(YVYU_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_YUY2);
		}

		TEST_METHOD(YVYU_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_UYVY);
		}

		TEST_METHOD(YVYU_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_VYUY);
		}

		TEST_METHOD(YVYU_to_IYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IYUV);
		}

		TEST_METHOD(YVYU_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_YV12);
		}

		TEST_METHOD(YVYU_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_YVU9);
		}

		TEST_METHOD(YVYU_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_YUV9);
		}

		TEST_METHOD(YVYU_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IYU1);
		}

		TEST_METHOD(YVYU_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IYU2);
		}

		TEST_METHOD(YVYU_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_Y800);
		}

		TEST_METHOD(YVYU_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_Y16);
		}

		TEST_METHOD(YVYU_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_CLJR);
		}

		TEST_METHOD(YVYU_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_Y41P);
		}

		TEST_METHOD(YVYU_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_AYUV);
		}

		//
		// VYUY to YUVx
		//

		TEST_METHOD(VYUY_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_YUY2);
		}

		TEST_METHOD(VYUY_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_UYVY);
		}

		TEST_METHOD(VYUY_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_YVYU);
		}

		TEST_METHOD(VYUY_to_IYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IYUV);
		}

		TEST_METHOD(VYUY_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_YV12);
		}

		TEST_METHOD(VYUY_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_YVU9);
		}

		TEST_METHOD(VYUY_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_YUV9);
		}

		TEST_METHOD(VYUY_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IYU1);
		}

		TEST_METHOD(VYUY_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IYU2);
		}

		TEST_METHOD(VYUY_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_Y800);
		}

		TEST_METHOD(VYUY_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_Y16);
		}

		TEST_METHOD(VYUY_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_CLJR);
		}

		TEST_METHOD(VYUY_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_Y41P);
		}

		TEST_METHOD(VYUY_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_AYUV);
		}

		//
		// IYUV to YUVx
		//

		TEST_METHOD(IYUV_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_YVU9);
		}

		TEST_METHOD(IYUV_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_YUV9);
		}

		TEST_METHOD(IYUV_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_YV12);
		}

		TEST_METHOD(IYUV_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_YUY2);
		}

		TEST_METHOD(IYUV_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_UYVY);
		}

		TEST_METHOD(IYUV_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_YVYU);
		}

		TEST_METHOD(IYUV_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_VYUY);
		}

		TEST_METHOD(IYUV_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_IYU1);
		}

		TEST_METHOD(IYUV_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_IYU2);
		}

		TEST_METHOD(IYUV_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_Y800);
		}

		TEST_METHOD(IYUV_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_Y16);
		}

		TEST_METHOD(IYUV_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_CLJR);
		}

		TEST_METHOD(IYUV_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_Y41P);
		}

		TEST_METHOD(IYUV_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYUV, MVFMT_AYUV);
		}

	private:
		void Run8bitTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			RunSingle8bitTest(inFormat, outFormat, 128, 128, 128, 255);
			RunSingle8bitTest(inFormat, outFormat, 255, 255, 255, 255);
			RunSingle8bitTest(inFormat, outFormat, 0, 0, 0, 255);
			RunSingle8bitTest(inFormat, outFormat, 255, 0, 0, 255);
			RunSingle8bitTest(inFormat, outFormat, 0, 255, 0, 255);
			RunSingle8bitTest(inFormat, outFormat, 0, 0, 255, 255);
		}

		void RunSingle8bitTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fullBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fullBufFunctPtr), L"fullBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height);

			std::unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
			uint8_t* inBufPtr = inBuf.get();
			memset(inBufPtr, 0, inBufBize);

			std::unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
			uint8_t* outBufPtr = outBuf.get();
			memset(outBufPtr, 0, outBufBize);

			uint8_t Y;
			uint8_t U;
			uint8_t V;
			FastRGBtoYUV(red, green, blue, &Y, &U, &V);

			fullBufFunctPtr(Y, U, V, alpha, width, height, inBufPtr, 0);

			encodeTransPtr(width, height, outBufPtr, 0, inBufPtr, 0, false, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, U, V, alpha, width, height, outBufPtr, 0), L"YUV buffer did not contain expected values.");
		}
	};
}
