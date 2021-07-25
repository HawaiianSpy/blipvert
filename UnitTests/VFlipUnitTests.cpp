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
#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"
#include "ToFillColor.h"
#include "BufferChecks.h"

#include <memory>
#include <iostream>
#include <string>
#include <locale>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;
using namespace std;

namespace BlipvertUnitTests
{
	TEST_CLASS(VFlipUnitTests)
	{
	public:

		TEST_METHOD(FlipBufferUnitTest)
		{
			RunAllTests();

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			RunAllTests();

			StrideBump = saveb;
		}

	private:

		vector<const MediaFormatID*> RGBFormats = {
			&MVFMT_RGBA,
			&MVFMT_RGB32,
			&MVFMT_RGB24,
			&MVFMT_RGB565,
			&MVFMT_RGB555,
			&MVFMT_ARGB1555
		};

		vector<const MediaFormatID*> YUVFormats = {
			&MVFMT_AYUV,
			&MVFMT_YUY2,
			&MVFMT_UYVY,
			&MVFMT_YVYU,
			&MVFMT_VYUY,
			&MVFMT_Y42T,
			&MVFMT_Y800,
			&MVFMT_Y16,
			&MVFMT_IYU1,
			&MVFMT_IYU2,
			&MVFMT_Y41P,
			&MVFMT_CLJR,
			&MVFMT_I420,
			&MVFMT_YV12,
			&MVFMT_YUV9,
			&MVFMT_YVU9,
			&MVFMT_IMC1,
			&MVFMT_IMC2,
			&MVFMT_IMC3,
			&MVFMT_IMC4,
			&MVFMT_NV12,
			&MVFMT_Y41T
		};

		void RunAllTests()
		{
			// RGB to YUV
			for (const MediaFormatID* in_format : RGBFormats)
			{
				for (const MediaFormatID* out_format : YUVFormats)
				{
					RunSingleTest(*in_format, *out_format);
				}
			}

			// YUV to RGB
			for (const MediaFormatID* in_format : YUVFormats)
			{
				for (const MediaFormatID* out_format : RGBFormats)
				{
					RunSingleTest(*in_format, *out_format);
				}
			}

			// RGB to RGB
			for (const MediaFormatID* in_format : RGBFormats)
			{
				for (const MediaFormatID* out_format : RGBFormats)
				{
					if (*in_format != *out_format)
					{
						RunSingleTest(*in_format, *out_format);
					}
				}
			}

			// YUV to YUV
			for (const MediaFormatID* in_format : YUVFormats)
			{
				for (const MediaFormatID* out_format : YUVFormats)
				{
					if (*in_format != *out_format)
					{
						RunSingleTest(*in_format, *out_format);
					}
				}
			}
		}

		wstring utf8ToUtf16Str(const string& str)
		{
			vector<wchar_t> buf(str.size());
			use_facet<ctype<wchar_t>>(locale{}).widen(str.data(), str.data() + str.size(), buf.data());
			return wstring(buf.data(), buf.size());
		}

		void RunSingleTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			wstring inFormatName = utf8ToUtf16Str(inFormat);
			wstring outFormatName = utf8ToUtf16Str(outFormat);

			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), wstring(L"encodeTransPtr returned a null function pointer: " + inFormatName + L" to " + outFormatName).c_str());

			t_setpixelfunc inSetPixelFunctPtr = FindSetPixelColor(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(inSetPixelFunctPtr), wstring(L"inSetPixelFunctPtr returned a null function pointer: " + inFormatName).c_str());

			t_setpixelfunc outSetPixelFunctPtr = FindSetPixelColor(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(outSetPixelFunctPtr), wstring(L"outSetPixelFunctPtr returned a null function pointer: " + outFormatName).c_str());

			t_flipverticalfunc outFlipVerticalFunctPtr = FindFlipVerticalTransform(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(outFlipVerticalFunctPtr), wstring(L"outFlipVerticalFunctPtr returned a null function pointer: " + outFormatName).c_str());

			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, wstring(L"inBufBize size retuned zero: " + inFormatName + L" to " + outFormatName).c_str());

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, wstring(L"outBufBize size retuned zero: " + inFormatName + L" to " + outFormatName).c_str());

			std::unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
			uint8_t* inBufPtr = inBuf.get();
			memset(inBufPtr, 0, inBufBize);

			std::unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
			uint8_t* outBufPtr = outBuf.get();
			memset(outBufPtr, 0, outBufBize);

			std::unique_ptr<uint8_t[]> outTestBuf(new uint8_t[outBufBize]);
			uint8_t* outTestBufPtr = outTestBuf.get();
			memset(outTestBufPtr, 0, outBufBize);

			uint8_t alpha = 255;

			bool inyuv = IsYUVColorspace(inFormat);

			// input transform buffer

			uint8_t upper_ry = 0xFF; // red
			uint8_t upper_gu = 0x00;
			uint8_t upper_bv = 0x00;

			if (inyuv)
			{
				uint8_t y, u, v;
				FastRGBtoYUV(upper_ry, upper_gu, upper_bv, &y, &u, &v);
				upper_ry = y;
				upper_gu = u;
				upper_bv = v;
			}

			for (uint32_t y = 0; y < height / 2; y++)
			{
				for (uint32_t x = 0; x < width; x++)
				{
					inSetPixelFunctPtr(upper_ry, upper_gu, upper_bv, alpha, x, y, width, height, inBufPtr, in_stride);
				}
			}

			uint8_t lower_ry = 0x00; // green
			uint8_t lower_gu = 0xFF;
			uint8_t lower_bv = 0x00;

			if (inyuv)
			{
				uint8_t y, u, v;
				FastRGBtoYUV(lower_ry, lower_gu, lower_bv, &y, &u, &v);
				lower_ry = y;
				lower_gu = u;
				lower_bv = v;
			}

			for (uint32_t y = height / 2; y < height; y++)
			{
				for (uint32_t x = 0; x < width; x++)
				{
					inSetPixelFunctPtr(lower_ry, lower_gu, lower_bv, alpha, x, y, width, height, inBufPtr, in_stride);
				}
			}

			// Use the transform top be tested to convert th inout buffer into the test buffer without flipping.
			encodeTransPtr(width, height, outTestBufPtr, out_stride, inBufPtr, in_stride, false, nullptr);

			// Now, manually vertically flip the test buffer here.
			outFlipVerticalFunctPtr(width, height, outTestBufPtr, out_stride);

			// Run the transform we want to test with the flipping on.
			encodeTransPtr(width, height, outBufPtr, out_stride, inBufPtr, in_stride, true, nullptr);

			// Finally compare the externally flipped bitmap with the flipping done by the transform.
			int result = memcmp(outBufPtr, outTestBufPtr, outBufBize);
			//if (result)
			//{
			//	__debugbreak();
			//}

			Assert::AreEqual(0, result, wstring(L"outBufPtr did not match outTestBufPtr: " + inFormatName + L" to " + outFormatName).c_str());
		}
	};
}
