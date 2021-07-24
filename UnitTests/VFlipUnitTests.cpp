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
			InitSourceImages();
			m_SrcFormatName = utf8ToUtf16Str(m_SourceMFmt);

			// RGB to YUV
			for (const MediaFormatID* in_format : RGBFormats)
			{
				for (const MediaFormatID* out_format : YUVFormats)
				{
					RunSingleTest(*in_format, *out_format);
				}
			}

			// YUV to RGB
			//for (const MediaFormatID* in_format : YUVFormats)
			//{
			//	for (const MediaFormatID* out_format : RGBFormats)
			//	{
			//		RunSingleTest(*in_format, *out_format);
			//	}
			//}

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
			//for (const MediaFormatID* in_format : YUVFormats)
			//{
			//	for (const MediaFormatID* out_format : YUVFormats)
			//	{
			//		if (*in_format != *out_format)
			//		{
			//			RunSingleTest(*in_format, *out_format);
			//		}
			//	}
			//}

			CleanupSourceImages();
		}

	private:
		static const uint32_t		m_Red{ 0xFFFF0000 };
		static const uint32_t		m_Green{ 0xFF00FF00 };
		static const uint32_t		m_Blue{ 0xFF0000FF };
		static const uint32_t		m_Grey{ 0xFF808080 };
		static const uint32_t		m_RedT{ 0x00FF0000 };
		static const uint32_t		m_GreenT{ 0x0000FF00 };
		static const uint32_t		m_BlueT{ 0x0000F00FF };
		static const uint32_t		m_GreyT{ 0x00808080 };

		vector<const MediaFormatID*> RGBFormats = {
			&MVFMT_RGBA,
			&MVFMT_RGB32,
			&MVFMT_RGB24/*,
			&MVFMT_RGB565,
			&MVFMT_RGB555,
			&MVFMT_ARGB1555*/
		};

		vector<const MediaFormatID*> YUVFormats = {
			&MVFMT_AYUV,
			&MVFMT_YUY2,
			&MVFMT_UYVY,
			&MVFMT_YVYU,
			&MVFMT_VYUY,
			/*&MVFMT_Y42T,*/
			&MVFMT_Y800,
			&MVFMT_Y16,
			&MVFMT_IYU1,
			&MVFMT_IYU2,
			&MVFMT_Y41P,
			/*&MVFMT_CLJR,*/
			&MVFMT_I420,
			&MVFMT_YV12,
			&MVFMT_YUV9,
			&MVFMT_YVU9,
			&MVFMT_IMC1,
			&MVFMT_IMC2,
			&MVFMT_IMC3,
			&MVFMT_IMC4,
			&MVFMT_NV12,
			/*&MVFMT_Y41T*/
		};

		MediaFormatID		m_SourceMFmt{ MVFMT_RGB32 };
		uint32_t			m_SourceSize;
		uint32_t			m_SourceStride;
		uint8_t*			m_SourceImage{ nullptr };
		uint8_t*			m_SourceImageVFlipped{ nullptr };
		wstring				m_SrcFormatName;
		wstring				m_InFormatName;
		wstring				m_OutFormatName;

		wstring utf8ToUtf16Str(const string& str)
		{
			vector<wchar_t> buf(str.size());
			use_facet<ctype<wchar_t>>(locale{}).widen(str.data(), str.data() + str.size(), buf.data());
			return wstring(buf.data(), buf.size());
		}

		void CleanupSourceImages()
		{
			if (m_SourceImage)
			{
				delete m_SourceImage;
				m_SourceImage = nullptr;
			}

			if (m_SourceImageVFlipped)
			{
				delete m_SourceImageVFlipped;
				m_SourceImageVFlipped = nullptr;
			}
		}

		void InitSourceImages()
		{
			m_SourceStride = CalculateStrideBump(m_SourceMFmt, TestBufferWidth);
			if (!m_SourceStride)
			{
				m_SourceStride = CalculateMinimumLineSize(m_SourceMFmt, TestBufferWidth);
			}

			m_SourceSize = CalculateBufferSize(m_SourceMFmt, TestBufferWidth, TestBufferHeight);

			m_SourceImage = new uint8_t[m_SourceSize];
			memset(m_SourceImage, 0, m_SourceSize);

			m_SourceImageVFlipped = new uint8_t[m_SourceSize];
			memset(m_SourceImageVFlipped, 0, m_SourceSize);

			uint8_t* yp = m_SourceImage;
			uint8_t* yfp = m_SourceImageVFlipped + (m_SourceStride * (TestBufferHeight - 1));

			for (uint32_t y = 0; y < TestBufferHeight; y++)
			{
				uint32_t* xp = reinterpret_cast<uint32_t*>(yp);
				uint32_t* xfp = reinterpret_cast<uint32_t*>(yfp);

				uint32_t leftColor = y < TestBufferHeight / 2 ? m_Red : m_Blue;
				uint32_t rightColor = y < TestBufferHeight / 2 ? m_Green : m_Grey;

				for (uint32_t x = 0; x < TestBufferWidth; x++)
				{
					if (x < TestBufferWidth / 2)
					{
						xp[x] = leftColor;
						xfp[x] = leftColor;
					}
					else
					{
						xp[x] = rightColor;
						xfp[x] = rightColor;
					}
				}

				yp += m_SourceStride;
				yfp -= m_SourceStride;
			}
		}

		void RunSingleTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			if (inFormat == m_SourceMFmt || outFormat == m_SourceMFmt)
			{
				return;
			}

			m_InFormatName = utf8ToUtf16Str(inFormat);
			m_OutFormatName = utf8ToUtf16Str(outFormat);

			// Get the transform function to be tested.
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), wstring(L"encodeTransPtr returned a null function pointer: " + m_InFormatName + L" to " + m_OutFormatName).c_str());

			// The the transform functions to make the source and destination test images.
			t_transformfunc sourceTransPtr = FindVideoTransform(m_SourceMFmt, inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), wstring(L"sourceTransPtr returned a null function pointer: " + m_SrcFormatName + L" to " + m_InFormatName).c_str());

			t_transformfunc destTransPtr = FindVideoTransform(m_SourceMFmt, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), wstring(L"destTransPtr returned a null function pointer: " + m_SrcFormatName + L" to " + m_InFormatName).c_str());

			// Setup the test buffers for the transform to be tested.
			uint32_t width = TestBufferWidth;
			uint32_t height = TestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, wstring(L"inBufBize size retuned zero: " + m_InFormatName + L" to " + m_OutFormatName).c_str());

			uint32_t out_stride = CalculateStrideBump(outFormat, width);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, wstring(L"outBufBize size retuned zero: " + m_InFormatName + L" to " + m_OutFormatName).c_str());

			unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
			uint8_t* inBufPtr = inBuf.get();
			memset(inBufPtr, 0, inBufBize);

			unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
			uint8_t* outBufPtr = outBuf.get();
			memset(outBufPtr, 0, outBufBize);

			// Fill the input buffer with the test image.
			sourceTransPtr(width, height, inBufPtr, in_stride, m_SourceImage, m_SourceStride, false, nullptr);

			// Setup the test image for the output buffer
			unique_ptr<uint8_t[]> outTestBuf(new uint8_t[outBufBize]);
			uint8_t* outTestBufPtr = outTestBuf.get();
			memset(outTestBufPtr, 0, outBufBize);

			destTransPtr(width, height, outTestBufPtr, out_stride, m_SourceImageVFlipped, m_SourceStride, false, nullptr);

			// Run the teansform we want to test with the flipping on.
			encodeTransPtr(width, height, outBufPtr, out_stride, inBufPtr, in_stride, true, nullptr);

			Assert::AreEqual(0, memcmp(outBufPtr, outTestBufPtr, outBufBize), wstring(L"outBufPtr did not match outTestBufPtr: " + m_InFormatName + L" to " + m_OutFormatName).c_str());
		}
	};
}
