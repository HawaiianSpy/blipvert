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

		TEST_METHOD(AYUV_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_I420);
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

		TEST_METHOD(AYUV_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IMC1);
		}

		TEST_METHOD(AYUV_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IMC2);
		}

		TEST_METHOD(AYUV_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IMC3);
		}

		TEST_METHOD(AYUV_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_IMC4);
		}

		TEST_METHOD(AYUV_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_NV12);
		}

		TEST_METHOD(AYUV_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_NV21);
		}

		TEST_METHOD(AYUV_to_Y42T_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_AYUV, MVFMT_Y42T);
		}

		TEST_METHOD(AYUV_to_Y41T_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_AYUV, MVFMT_Y41T);
		}

		TEST_METHOD(AYUV_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_AYUV, MVFMT_YV16);
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

		TEST_METHOD(YUY2_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_I420);
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

		TEST_METHOD(YUY2_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IMC1);
		}

		TEST_METHOD(YUY2_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IMC2);
		}

		TEST_METHOD(YUY2_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IMC3);
		}

		TEST_METHOD(YUY2_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_IMC4);
		}

		TEST_METHOD(YUY2_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_NV12);
		}

		TEST_METHOD(YUY2_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_NV21);
		}

		TEST_METHOD(YUY2_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_Y42T);
		}

		TEST_METHOD(YUY2_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_Y41T);
		}

		TEST_METHOD(YUY2_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUY2, MVFMT_YV16);
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

		TEST_METHOD(UYVY_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_I420);
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

		TEST_METHOD(UYVY_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IMC1);
		}

		TEST_METHOD(UYVY_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IMC2);
		}

		TEST_METHOD(UYVY_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IMC3);
		}

		TEST_METHOD(UYVY_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_IMC4);
		}

		TEST_METHOD(UYVY_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_NV12);
		}

		TEST_METHOD(UYVY_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_NV21);
		}

		TEST_METHOD(UYVY_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_Y42T);
		}

		TEST_METHOD(UYVY_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_Y41T);
		}

		TEST_METHOD(UYVY_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_UYVY, MVFMT_YV16);
		}

		//
		// YVYU to YUVx
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

		TEST_METHOD(YVYU_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_I420);
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

		TEST_METHOD(YVYU_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IMC1);
		}

		TEST_METHOD(YVYU_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IMC2);
		}

		TEST_METHOD(YVYU_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IMC3);
		}

		TEST_METHOD(YVYU_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_IMC4);
		}

		TEST_METHOD(YVYU_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_NV12);
		}

		TEST_METHOD(YVYU_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_NV21);
		}

		TEST_METHOD(YVYU_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_Y42T);
		}

		TEST_METHOD(YVYU_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_Y41T);
		}

		TEST_METHOD(YVYU_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVYU, MVFMT_YV16);
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

		TEST_METHOD(VYUY_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_I420);
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

		TEST_METHOD(VYUY_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IMC1);
		}

		TEST_METHOD(VYUY_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IMC2);
		}

		TEST_METHOD(VYUY_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IMC3);
		}

		TEST_METHOD(VYUY_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_IMC4);
		}

		TEST_METHOD(VYUY_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_NV12);
		}

		TEST_METHOD(VYUY_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_NV21);
		}
		TEST_METHOD(VYUY_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_Y42T);
		}

		TEST_METHOD(VYUY_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_Y41T);
		}

		TEST_METHOD(VYUY_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_VYUY, MVFMT_YV16);
		}

		//
		// I420 to YUVx
		//

		TEST_METHOD(I420_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_YVU9);
		}

		TEST_METHOD(I420_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_YUV9);
		}

		TEST_METHOD(I420_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_YV12);
		}

		TEST_METHOD(I420_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_YUY2);
		}

		TEST_METHOD(I420_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_UYVY);
		}

		TEST_METHOD(I420_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_YVYU);
		}

		TEST_METHOD(I420_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_VYUY);
		}

		TEST_METHOD(I420_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_IYU1);
		}

		TEST_METHOD(I420_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_IYU2);
		}

		TEST_METHOD(I420_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_Y800);
		}

		TEST_METHOD(I420_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_Y16);
		}

		TEST_METHOD(I420_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_CLJR);
		}

		TEST_METHOD(I420_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_Y41P);
		}

		TEST_METHOD(I420_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_AYUV);
		}

		TEST_METHOD(I420_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_IMC1);
		}

		TEST_METHOD(I420_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_IMC2);
		}

		TEST_METHOD(I420_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_IMC3);
		}

		TEST_METHOD(I420_to_IMC41_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_IMC4);
		}

		TEST_METHOD(I420_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_NV12);
		}

		TEST_METHOD(I420_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_NV21);
		}

		TEST_METHOD(I420_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_Y42T);
		}

		TEST_METHOD(I420_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_Y41T);
		}

		TEST_METHOD(I420_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_I420, MVFMT_YV16);
		}

		//
		// YV12 to YUVx
		//

		TEST_METHOD(YV12_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_YVU9);
		}

		TEST_METHOD(YV12_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_YUV9);
		}

		TEST_METHOD(YV12_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_I420);
		}

		TEST_METHOD(YV12_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_YUY2);
		}

		TEST_METHOD(YV12_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_UYVY);
		}

		TEST_METHOD(YV12_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_YVYU);
		}

		TEST_METHOD(YV12_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_VYUY);
		}

		TEST_METHOD(YV12_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_IYU1);
		}

		TEST_METHOD(YV12_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_IYU2);
		}

		TEST_METHOD(YV12_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_Y800);
		}

		TEST_METHOD(YV12_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_Y16);
		}

		TEST_METHOD(YV12_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_CLJR);
		}

		TEST_METHOD(YV12_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_Y41P);
		}

		TEST_METHOD(YV12_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_AYUV);
		}

		TEST_METHOD(YV12_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_IMC1);
		}

		TEST_METHOD(YV12_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_IMC2);
		}

		TEST_METHOD(YV12_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_IMC3);
		}

		TEST_METHOD(YV12_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_IMC4);
		}

		TEST_METHOD(YV12_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_NV12);
		}

		TEST_METHOD(YV12_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_NV21);
		}

		TEST_METHOD(YV12_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_Y42T);
		}

		TEST_METHOD(YV12_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_Y41T);
		}

		TEST_METHOD(YV12_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV12, MVFMT_YV16);
		}

		//
		// YVU9 to YUVx
		//

		TEST_METHOD(YVU9_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_I420);
		}

		TEST_METHOD(YVU9_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_YV12);
		}

		TEST_METHOD(YVU9_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_YUV9);
		}

		TEST_METHOD(YVU9_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_YUY2);
		}

		TEST_METHOD(YVU9_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_UYVY);
		}

		TEST_METHOD(YVU9_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_YVYU);
		}

		TEST_METHOD(YVU9_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_VYUY);
		}

		TEST_METHOD(YVU9_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_IYU1);
		}

		TEST_METHOD(YVU9_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_IYU2);
		}

		TEST_METHOD(YVU9_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_Y800);
		}

		TEST_METHOD(YVU9_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_Y16);
		}

		TEST_METHOD(YVU9_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_CLJR);
		}

		TEST_METHOD(YVU9_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_Y41P);
		}

		TEST_METHOD(YVU9_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_AYUV);
		}

		TEST_METHOD(YVU9_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_IMC1);
		}

		TEST_METHOD(YVU9_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_IMC2);
		}

		TEST_METHOD(YVU9_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_IMC3);
		}

		TEST_METHOD(YVU9_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_IMC4);
		}

		TEST_METHOD(YVU9_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_NV12);
		}

		TEST_METHOD(YVU9_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_NV21);
		}

		TEST_METHOD(YVU9_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_Y42T);
		}

		TEST_METHOD(YVU9_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_Y41T);
		}

		TEST_METHOD(YVU9_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YVU9, MVFMT_YV16);
		}

		//
		// YUV9 to YUVx
		//

		TEST_METHOD(YUV9_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_I420);
		}

		TEST_METHOD(YUV9_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_YV12);
		}

		TEST_METHOD(YUV9_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_YVU9);
		}

		TEST_METHOD(YUV9_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_YUY2);
		}

		TEST_METHOD(YUV9_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_UYVY);
		}

		TEST_METHOD(YUV9_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_YVYU);
		}

		TEST_METHOD(YUV9_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_VYUY);
		}

		TEST_METHOD(YUV9_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_IYU1);
		}

		TEST_METHOD(YUV9_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_IYU2);
		}

		TEST_METHOD(YUV9_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_Y800);
		}

		TEST_METHOD(YUV9_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_Y16);
		}

		TEST_METHOD(YUV9_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_CLJR);
		}

		TEST_METHOD(YUV9_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_Y41P);
		}

		TEST_METHOD(YUV9_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_AYUV);
		}

		TEST_METHOD(YUV9_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_IMC1);
		}

		TEST_METHOD(YUV9_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_IMC2);
		}

		TEST_METHOD(YUV9_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_IMC3);
		}

		TEST_METHOD(YUV9_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_IMC4);
		}

		TEST_METHOD(YUV9_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_NV12);
		}

		TEST_METHOD(YUV9_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_NV21);
		}

		TEST_METHOD(YUV9_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_Y42T);
		}

		TEST_METHOD(YUV9_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_Y41T);
		}

		TEST_METHOD(YUV9_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YUV9, MVFMT_YV16);
		}

		//
		// IYU1 to YUVx
		//

		TEST_METHOD(IYU1_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_YUY2);
		}

		TEST_METHOD(IYU1_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_UYVY);
		}

		TEST_METHOD(IYU1_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_YVYU);
		}

		TEST_METHOD(IYU1_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_VYUY);
		}

		TEST_METHOD(IYU1_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_I420);
		}

		TEST_METHOD(IYU1_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_YV12);
		}

		TEST_METHOD(IYU1_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_YVU9);
		}

		TEST_METHOD(IYU1_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_YUV9);
		}

		TEST_METHOD(IYU1_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_IYU2);
		}

		TEST_METHOD(IYU1_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_Y800);
		}

		TEST_METHOD(IYU1_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_Y16);
		}

		TEST_METHOD(IYU1_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_CLJR);
		}

		TEST_METHOD(IYU1_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_Y41P);
		}

		TEST_METHOD(IYU1_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_AYUV);
		}

		TEST_METHOD(IYU1_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_IMC1);
		}

		TEST_METHOD(IYU1_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_IMC2);
		}

		TEST_METHOD(IYU1_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_IMC3);
		}

		TEST_METHOD(IYU1_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_IMC4);
		}

		TEST_METHOD(IYU1_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_NV12);
		}

		TEST_METHOD(IYU1_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_NV21);
		}

		TEST_METHOD(IYU1_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_Y42T);
		}

		TEST_METHOD(IYU1_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_Y41T);
		}

		TEST_METHOD(IYU1_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU1, MVFMT_YV16);
		}

		//
		// IYU2 to YUVx
		//

		TEST_METHOD(IYU2_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_YUY2);
		}

		TEST_METHOD(IYU2_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_UYVY);
		}

		TEST_METHOD(IYU2_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_YVYU);
		}

		TEST_METHOD(IYU2_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_VYUY);
		}

		TEST_METHOD(IYU2_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_I420);
		}

		TEST_METHOD(IYU2_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_YV12);
		}

		TEST_METHOD(IYU2_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_YVU9);
		}

		TEST_METHOD(IYU2_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_YUV9);
		}

		TEST_METHOD(IYU2_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_IYU1);
		}

		TEST_METHOD(IYU2_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_Y800);
		}

		TEST_METHOD(IYU2_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_Y16);
		}

		TEST_METHOD(IYU2_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_CLJR);
		}

		TEST_METHOD(IYU2_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_Y41P);
		}

		TEST_METHOD(IYU2_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_AYUV);
		}

		TEST_METHOD(IYU2_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_IMC1);
		}

		TEST_METHOD(IYU2_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_IMC2);
		}

		TEST_METHOD(IYU2_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_IMC3);
		}

		TEST_METHOD(IYU2_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_IMC4);
		}

		TEST_METHOD(IYU2_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_NV12);
		}

		TEST_METHOD(IYU2_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_NV21);
		}

		TEST_METHOD(IYU2_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_Y42T);
		}

		TEST_METHOD(IYU2_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_Y41T);
		}

		TEST_METHOD(IYU2_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IYU2, MVFMT_YV16);
		}

		//
		// Y800 to YUVx
		//

		TEST_METHOD(Y800_to_YUY2_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_YUY2);
		}

		TEST_METHOD(Y800_to_UYVY_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_UYVY);
		}

		TEST_METHOD(Y800_to_YVYU_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_YVYU);
		}

		TEST_METHOD(Y800_to_VYUY_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_VYUY);
		}

		TEST_METHOD(Y800_to_I420_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_I420);
		}

		TEST_METHOD(Y800_to_YV12_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_YV12);
		}

		TEST_METHOD(Y800_to_YVU9_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_YVU9);
		}

		TEST_METHOD(Y800_to_YUV9_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_YUV9);
		}

		TEST_METHOD(Y800_to_IYU1_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_IYU1);
		}

		TEST_METHOD(Y800_to_IYU2_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_IYU2);
		}

		TEST_METHOD(Y800_to_CLJR_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_CLJR);
		}

		TEST_METHOD(Y800_to_Y41P_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_Y41P);
		}

		TEST_METHOD(Y800_to_AYUV_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_AYUV);
		}

		TEST_METHOD(Y800_to_Y16_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_Y16);
		}

		TEST_METHOD(Y800_to_IMC1_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_IMC1);
		}

		TEST_METHOD(Y800_to_IMC2_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_IMC2);
		}

		TEST_METHOD(Y800_to_IMC3_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_IMC3);
		}

		TEST_METHOD(Y800_to_IMC4_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_IMC4);
		}

		TEST_METHOD(Y800_to_NV12_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_NV12);
		}

		TEST_METHOD(Y800_to_NV21_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_NV21);
		}

		TEST_METHOD(Y800_to_Y42T_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_Y42T);
		}

		TEST_METHOD(Y800_to_Y41T_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_Y41T);
		}

		TEST_METHOD(Y800_to_YV16_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y800, MVFMT_YV16);
		}

		//
		// Y16 to YUVx
		//

		TEST_METHOD(Y16_to_YUY2_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_YUY2);
		}

		TEST_METHOD(Y16_to_UYVY_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_UYVY);
		}

		TEST_METHOD(Y16_to_YVYU_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_YVYU);
		}

		TEST_METHOD(Y16_to_VYUY_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_VYUY);
		}

		TEST_METHOD(Y16_to_I420_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_I420);
		}

		TEST_METHOD(Y16_to_YV12_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_YV12);
		}

		TEST_METHOD(Y16_to_YVU9_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_YVU9);
		}

		TEST_METHOD(Y16_to_YUV9_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_YUV9);
		}

		TEST_METHOD(Y16_to_IYU1_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_IYU1);
		}

		TEST_METHOD(Y16_to_IYU2_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_IYU2);
		}

		TEST_METHOD(Y16_to_CLJR_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_CLJR);
		}

		TEST_METHOD(Y16_to_Y41P_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_Y41P);
		}

		TEST_METHOD(Y16_to_AYUV_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_AYUV);
		}

		TEST_METHOD(Y16_to_Y800_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_Y800);
		}

		TEST_METHOD(Y16_to_IMC1_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_IMC1);
		}

		TEST_METHOD(Y16_to_IMC2_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_IMC2);
		}

		TEST_METHOD(Y16_to_IMC3_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_IMC3);
		}

		TEST_METHOD(Y16_to_IMC4_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_IMC4);
		}

		TEST_METHOD(Y16_to_NV12_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_NV12);
		}

		TEST_METHOD(Y16_to_NV21_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_NV21);
		}

		TEST_METHOD(Y16_to_Y42T_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_Y42T);
		}

		TEST_METHOD(Y16_to_Y41T_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_Y41T);
		}

		TEST_METHOD(Y16_to_YV16_UnitTest)
		{
			RunGreyscaleTestSeries(MVFMT_Y16, MVFMT_YV16);
		}

		//
		// CLJR to YUVx
		//

		TEST_METHOD(CLJR_to_YUY2_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_YUY2);
		}

		TEST_METHOD(CLJR_to_UYVY_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_UYVY);
		}

		TEST_METHOD(CLJR_to_YVYU_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_YVYU);
		}

		TEST_METHOD(CLJR_to_VYUY_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_VYUY);
		}

		TEST_METHOD(CLJR_to_I420_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_I420);
		}

		TEST_METHOD(CLJR_to_YV12_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_YV12);
		}

		TEST_METHOD(CLJR_to_YVU9_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_YVU9);
		}

		TEST_METHOD(CLJR_to_YUV9_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_YUV9);
		}

		TEST_METHOD(CLJR_to_IYU1_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_IYU1);
		}

		TEST_METHOD(CLJR_to_IYU2_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_IYU2);
		}

		TEST_METHOD(CLJR_to_Y800_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_Y800);
		}

		TEST_METHOD(CLJR_to_Y16_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_Y16);
		}

		TEST_METHOD(CLJR_to_Y41P_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_Y41P);
		}

		TEST_METHOD(CLJR_to_AYUV_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_AYUV);
		}

		TEST_METHOD(CLJR_to_IMC1_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_IMC1);
		}

		TEST_METHOD(CLJR_to_IMC2_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_IMC2);
		}

		TEST_METHOD(CLJR_to_IMC3_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_IMC3);
		}

		TEST_METHOD(CLJR_to_IMC4_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_IMC4);
		}

		TEST_METHOD(CLJR_to_NV12_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_NV12);
		}

		TEST_METHOD(CLJR_to_NV21_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_NV21);
		}

		TEST_METHOD(CLJR_to_Y42T_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_Y42T);
		}

		TEST_METHOD(CLJR_to_Y41T_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_Y41T);
		}

		TEST_METHOD(CLJR_to_YV16_UnitTest)
		{
			RunCLJRTestSeries(MVFMT_CLJR, MVFMT_YV16);
		}

		//
		// Y41P to YUVx
		//

		TEST_METHOD(Y41P_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_YUY2);
		}

		TEST_METHOD(Y41P_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_UYVY);
		}

		TEST_METHOD(Y41P_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_YVYU);
		}

		TEST_METHOD(Y41P_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_VYUY);
		}

		TEST_METHOD(Y41P_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_I420);
		}

		TEST_METHOD(Y41P_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_YV12);
		}

		TEST_METHOD(Y41P_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_YVU9);
		}

		TEST_METHOD(Y41P_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_YUV9);
		}

		TEST_METHOD(Y41P_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_IYU1);
		}

		TEST_METHOD(Y41P_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_IYU2);
		}

		TEST_METHOD(Y41P_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_Y800);
		}

		TEST_METHOD(Y41P_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_Y16);
		}

		TEST_METHOD(Y41P_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_CLJR);
		}

		TEST_METHOD(Y41P_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_AYUV);
		}

		TEST_METHOD(Y41P_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_IMC1);
		}

		TEST_METHOD(Y41P_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_IMC2);
		}

		TEST_METHOD(Y41P_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_IMC3);
		}

		TEST_METHOD(Y41P_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_IMC4);
		}

		TEST_METHOD(Y41P_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_NV12);
		}

		TEST_METHOD(Y41P_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_NV21);
		}

		TEST_METHOD(Y41P_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_Y42T);
		}

		TEST_METHOD(Y41P_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_Y41T);
		}

		TEST_METHOD(Y41P_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_Y41P, MVFMT_YV16);
		}

		//
		// IMC1 to YUVx
		//

		TEST_METHOD(IMC1_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_AYUV);
		}

		TEST_METHOD(IMC1_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_YUY2);
		}

		TEST_METHOD(IMC1_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_UYVY);
		}

		TEST_METHOD(IMC1_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_YVYU);
		}

		TEST_METHOD(IMC1_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_VYUY);
		}

		TEST_METHOD(IMC1_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_I420);
		}

		TEST_METHOD(IMC1_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_YV12);
		}

		TEST_METHOD(IMC1_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_YUV9);
		}

		TEST_METHOD(IMC1_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_YVU9);
		}

		TEST_METHOD(IMC1_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_IYU1);
		}

		TEST_METHOD(IMC1_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_IYU2);
		}

		TEST_METHOD(IMC1_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_Y800);
		}

		TEST_METHOD(IMC1_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_Y16);
		}

		TEST_METHOD(IMC1_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_CLJR);
		}

		TEST_METHOD(IMC1_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_Y41P);
		}

		TEST_METHOD(IMC1_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_IMC2);
		}

		TEST_METHOD(IMC1_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_IMC3);
		}

		TEST_METHOD(IMC1_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_IMC4);
		}

		TEST_METHOD(IMC1_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_NV12);
		}

		TEST_METHOD(IMC1_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_NV21);
		}

		TEST_METHOD(IMC1_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_Y42T);
		}

		TEST_METHOD(IMC1_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_Y41T);
		}

		TEST_METHOD(IMC1_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC1, MVFMT_YV16);
		}

		//
		// IMC2 to YUVx
		//

		TEST_METHOD(IMC2_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_AYUV);
		}

		TEST_METHOD(IMC2_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_YUY2);
		}

		TEST_METHOD(IMC2_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_UYVY);
		}

		TEST_METHOD(IMC2_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_YVYU);
		}

		TEST_METHOD(IMC2_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_VYUY);
		}

		TEST_METHOD(IMC2_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_I420);
		}

		TEST_METHOD(IMC2_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_YV12);
		}

		TEST_METHOD(IMC2_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_YUV9);
		}

		TEST_METHOD(IMC2_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_YVU9);
		}

		TEST_METHOD(IMC2_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_IYU1);
		}

		TEST_METHOD(IMC2_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_IYU2);
		}

		TEST_METHOD(IMC2_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_Y800);
		}

		TEST_METHOD(IMC2_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_Y16);
		}

		TEST_METHOD(IMC2_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_CLJR);
		}

		TEST_METHOD(IMC2_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_Y41P);
		}

		TEST_METHOD(IMC2_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_IMC1);
		}

		TEST_METHOD(IMC2_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_IMC3);
		}

		TEST_METHOD(IMC2_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_IMC4);
		}

		TEST_METHOD(IMC2_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_NV12);
		}

		TEST_METHOD(IMC2_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_NV21);
		}

		TEST_METHOD(IMC2_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_Y42T);
		}

		TEST_METHOD(IMC2_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_Y41T);
		}

		TEST_METHOD(IMC2_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC2, MVFMT_YV16);
		}

		//
		// IMC3 to YUVx
		//

		TEST_METHOD(IMC3_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_AYUV);
		}

		TEST_METHOD(IMC3_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_YUY2);
		}

		TEST_METHOD(IMC3_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_UYVY);
		}

		TEST_METHOD(IMC3_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_YVYU);
		}

		TEST_METHOD(IMC3_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_VYUY);
		}

		TEST_METHOD(IMC3_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_I420);
		}

		TEST_METHOD(IMC3_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_YV12);
		}

		TEST_METHOD(IMC3_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_YUV9);
		}

		TEST_METHOD(IMC3_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_YVU9);
		}

		TEST_METHOD(IMC3_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_IYU1);
		}

		TEST_METHOD(IMC3_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_IYU2);
		}

		TEST_METHOD(IMC3_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_Y800);
		}

		TEST_METHOD(IMC3_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_Y16);
		}

		TEST_METHOD(IMC3_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_CLJR);
		}

		TEST_METHOD(IMC3_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_Y41P);
		}

		TEST_METHOD(IMC3_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_IMC1);
		}

		TEST_METHOD(IMC3_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_IMC2);
		}

		TEST_METHOD(IMC3_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_IMC4);
		}

		TEST_METHOD(IMC3_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_NV12);
		}

		TEST_METHOD(IMC3_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_NV21);
		}

		TEST_METHOD(IMC3_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_Y42T);
		}

		TEST_METHOD(IMC3_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_Y41T);
		}

		TEST_METHOD(IMC3_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC3, MVFMT_YV16);
		}

		//
		// IMC4 to YUVx
		//

		TEST_METHOD(IMC4_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_AYUV);
		}

		TEST_METHOD(IMC4_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_YUY2);
		}

		TEST_METHOD(IMC4_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_UYVY);
		}

		TEST_METHOD(IMC4_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_YVYU);
		}

		TEST_METHOD(IMC4_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_VYUY);
		}

		TEST_METHOD(IMC4_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_I420);
		}

		TEST_METHOD(IMC4_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_YV12);
		}

		TEST_METHOD(IMC4_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_YUV9);
		}

		TEST_METHOD(IMC4_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_YVU9);
		}

		TEST_METHOD(IMC4_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_IYU1);
		}

		TEST_METHOD(IMC4_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_IYU2);
		}

		TEST_METHOD(IMC4_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_Y800);
		}

		TEST_METHOD(IMC4_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_Y16);
		}

		TEST_METHOD(IMC4_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_CLJR);
		}

		TEST_METHOD(IMC4_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_Y41P);
		}

		TEST_METHOD(IMC4_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_IMC1);
		}

		TEST_METHOD(IMC4_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_IMC2);
		}

		TEST_METHOD(IMC4_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_IMC3);
		}

		TEST_METHOD(IMC4_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_NV12);
		}

		TEST_METHOD(IMC4_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_NV21);
		}

		TEST_METHOD(IMC4_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_Y42T);
		}

		TEST_METHOD(IMC4_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_Y41T);
		}

		TEST_METHOD(IMC4_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_IMC4, MVFMT_YV16);
		}

		//
		// NV12 to YUVx
		//

		TEST_METHOD(NV12_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_AYUV);
		}

		TEST_METHOD(NV12_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_YUY2);
		}

		TEST_METHOD(NV12_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_UYVY);
		}

		TEST_METHOD(NV12_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_YVYU);
		}

		TEST_METHOD(NV12_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_VYUY);
		}

		TEST_METHOD(NV12_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_I420);
		}

		TEST_METHOD(NV12_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_YV12);
		}

		TEST_METHOD(NV12_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_YUV9);
		}

		TEST_METHOD(NV12_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_YVU9);
		}

		TEST_METHOD(NV12_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_IYU1);
		}

		TEST_METHOD(NV12_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_IYU2);
		}

		TEST_METHOD(NV12_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_Y800);
		}

		TEST_METHOD(NV12_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_Y16);
		}

		TEST_METHOD(NV12_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_CLJR);
		}

		TEST_METHOD(NV12_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_Y41P);
		}

		TEST_METHOD(NV12_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_IMC1);
		}

		TEST_METHOD(NV12_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_IMC2);
		}

		TEST_METHOD(NV12_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_IMC3);
		}

		TEST_METHOD(NV12_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_IMC4);
		}

		TEST_METHOD(NV12_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_NV21);
		}

		TEST_METHOD(NV12_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_Y42T);
		}

		TEST_METHOD(NV12_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_Y41T);
		}

		TEST_METHOD(NV12_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV12, MVFMT_YV16);
		}

		//
		// NV21 to YUV
		//

		TEST_METHOD(NV21_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_AYUV);
		}

		TEST_METHOD(NV21_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_YUY2);
		}

		TEST_METHOD(NV21_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_UYVY);
		}

		TEST_METHOD(NV21_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_YVYU);
		}

		TEST_METHOD(NV21_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_VYUY);
		}

		TEST_METHOD(NV21_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_I420);
		}

		TEST_METHOD(NV21_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_YV12);
		}

		TEST_METHOD(NV21_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_YUV9);
		}

		TEST_METHOD(NV21_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_YVU9);
		}

		TEST_METHOD(NV21_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_IYU1);
		}

		TEST_METHOD(NV21_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_IYU2);
		}

		TEST_METHOD(NV21_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_Y800);
		}

		TEST_METHOD(NV21_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_Y16);
		}

		TEST_METHOD(NV21_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_CLJR);
		}

		TEST_METHOD(NV21_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_Y41P);
		}

		TEST_METHOD(NV21_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_IMC1);
		}

		TEST_METHOD(NV21_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_IMC2);
		}

		TEST_METHOD(NV21_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_IMC3);
		}

		TEST_METHOD(NV21_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_IMC4);
		}

		TEST_METHOD(NV21_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_NV12);
		}

		TEST_METHOD(NV21_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_Y42T);
		}

		TEST_METHOD(NV21_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_Y41T);
		}

		TEST_METHOD(NV21_to_YV16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_NV21, MVFMT_YV16);
		}

		//
		// Y42T to YUV
		//

		TEST_METHOD(Y42T_to_YUY2_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_YUY2);
		}

		TEST_METHOD(Y42T_to_UYVY_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_UYVY);
		}

		TEST_METHOD(Y42T_to_YVYU_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_YVYU);
		}

		TEST_METHOD(Y42T_to_VYUY_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_VYUY);
		}

		TEST_METHOD(Y42T_to_I420_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_I420);
		}

		TEST_METHOD(Y42T_to_YV12_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_YV12);
		}

		TEST_METHOD(Y42T_to_YUV9_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_YUV9);
		}

		TEST_METHOD(Y42T_to_YVU9_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_YVU9);
		}

		TEST_METHOD(Y42T_to_IYU1_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_IYU1);
		}

		TEST_METHOD(Y42T_to_IYU2_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_IYU2);
		}

		TEST_METHOD(Y42T_to_Y800_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_Y800);
		}

		TEST_METHOD(Y42T_to_Y16_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_Y16);
		}

		TEST_METHOD(Y42T_to_CLJR_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_CLJR);
		}

		TEST_METHOD(Y42T_to_Y41P_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_Y41P);
		}

		TEST_METHOD(Y42T_to_AYUV_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_AYUV);
		}

		TEST_METHOD(Y42T_to_IMC1_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_IMC1);
		}

		TEST_METHOD(Y42T_to_IMC2_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_IMC2);
		}

		TEST_METHOD(Y42T_to_IMC3_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_IMC3);
		}

		TEST_METHOD(Y42T_to_IMC4_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_IMC4);
		}

		TEST_METHOD(Y42T_to_NV12_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_NV12);
		}

		TEST_METHOD(Y42T_to_NV21_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_NV21);
		}

		TEST_METHOD(Y42T_to_Y41T_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_Y41T);
		}

		TEST_METHOD(Y42T_to_YV16_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y42T, MVFMT_YV16);
		}

		//
		// Y41T to YUV
		//

		TEST_METHOD(Y41T_to_YUY2_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_YUY2);
		}

		TEST_METHOD(Y41T_to_UYVY_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_UYVY);
		}

		TEST_METHOD(Y41T_to_YVYU_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_YVYU);
		}

		TEST_METHOD(Y41T_to_VYUY_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_VYUY);
		}

		TEST_METHOD(Y41T_to_I420_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_I420);
		}

		TEST_METHOD(Y41T_to_YV12_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_YV12);
		}

		TEST_METHOD(Y41T_to_YUV9_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_YUV9);
		}

		TEST_METHOD(Y41T_to_YVU9_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_YVU9);
		}

		TEST_METHOD(Y41T_to_IYU1_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_IYU1);
		}

		TEST_METHOD(Y41T_to_IYU2_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_IYU2);
		}

		TEST_METHOD(Y41T_to_Y800_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_Y800);
		}

		TEST_METHOD(Y41T_to_Y16_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_Y16);
		}

		TEST_METHOD(Y41T_to_CLJR_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_CLJR);
		}

		TEST_METHOD(Y41T_to_Y41P_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_Y41P);
		}

		TEST_METHOD(Y41T_to_AYUV_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_AYUV);
		}

		TEST_METHOD(Y41T_to_IMC1_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_IMC1);
		}

		TEST_METHOD(Y41T_to_IMC2_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_IMC2);
		}

		TEST_METHOD(Y41T_to_IMC3_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_IMC3);
		}

		TEST_METHOD(Y41T_to_IMC4_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_IMC4);
		}

		TEST_METHOD(Y41T_to_NV12_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_NV12);
		}

		TEST_METHOD(Y41T_to_NV21_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_NV21);
		}

		TEST_METHOD(Y41T_to_Y42T_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_Y42T);
		}

		TEST_METHOD(Y41T_to_YV16_UnitTest)
		{
			RunYAlphaTestSeries(MVFMT_Y41T, MVFMT_YV16);
		}

		//
		// YV16 to YUV
		//

		TEST_METHOD(YV16_to_YUY2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_YUY2);
		}

		TEST_METHOD(YV16_to_UYVY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_UYVY);
		}

		TEST_METHOD(YV16_to_YVYU_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_YVYU);
		}

		TEST_METHOD(YV16_to_VYUY_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_VYUY);
		}

		TEST_METHOD(YV16_to_I420_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_I420);
		}

		TEST_METHOD(YV16_to_YV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_YV12);
		}

		TEST_METHOD(YV16_to_YVU9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_YVU9);
		}

		TEST_METHOD(YV16_to_YUV9_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_YUV9);
		}

		TEST_METHOD(YV16_to_IYU1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_IYU1);
		}

		TEST_METHOD(YV16_to_IYU2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_IYU2);
		}

		TEST_METHOD(YV16_to_Y800_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_Y800);
		}

		TEST_METHOD(YV16_to_Y16_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_Y16);
		}

		TEST_METHOD(YV16_to_CLJR_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_CLJR);
		}

		TEST_METHOD(YV16_to_Y41P_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_Y41P);
		}

		TEST_METHOD(YV16_to_AYUV_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_AYUV);
		}

		TEST_METHOD(YV16_to_IMC1_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_IMC1);
		}

		TEST_METHOD(YV16_to_IMC2_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_IMC2);
		}

		TEST_METHOD(YV16_to_IMC3_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_IMC3);
		}

		TEST_METHOD(YV16_to_IMC4_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_IMC4);
		}

		TEST_METHOD(YV16_to_NV12_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_NV12);
		}

		TEST_METHOD(YV16_to_NV21_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_NV21);
		}

		TEST_METHOD(YV16_to_Y42T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_Y42T);
		}

		TEST_METHOD(YV16_to_Y41T_UnitTest)
		{
			Run8bitTestSeries(MVFMT_YV16, MVFMT_Y41T);
		}

	private:
		void Run8bitAlphaTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle8bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle8bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void Run8bitTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle8bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle8bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingle8bitTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, inBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for inFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, inBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for outFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, outBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, U, V, alpha, width, height, outBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}

		void RunYAlphaTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingleYAlphaTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingleYAlphaTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingleYAlphaTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, inBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for inFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, inBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for outFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, outBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			Assert::IsTrue(bufCheckFunctPtr(Y & 0xFE, U, V, alpha, width, height, outBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}

		void RunGreyscaleTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleGreyscaleTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleGreyscaleTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingleGreyscaleTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, inBufPtr, in_stride);
			t_stagetransformfunc pstage = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for inFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, inBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for outFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, outBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			Assert::IsTrue(bufCheckFunctPtr(Y, 0, 0, alpha, width, height, outBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}

		void RunCLJRTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleCLJRTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingleCLJRTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingleCLJRTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// YUV to YUV
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

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

			fillBufFunctPtr(Y, U, V, alpha, width, height, inBufPtr, in_stride);

			t_stagetransformfunc pstage = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for inFormat returned a null function pointer.");
			Stage inptr;
			pstage(&inptr, 0, 1, width, height, inBufPtr, in_stride, false, nullptr);

			pstage = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage), L"FindTransformStage for outFormat returned a null function pointer.");
			Stage outptr;
			pstage(&outptr, 0, 1, width, height, outBufPtr, out_stride, false, nullptr);

			encodeTransPtr(&inptr, &outptr);

			Assert::IsTrue(bufCheckFunctPtr(Y & 0xF8, U & 0xFC, V & 0xFC, alpha, width, height, outBufPtr, out_stride), L"YUV buffer did not contain expected values.");
		}
	};
}
