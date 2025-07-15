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
#include "RGBtoRGB.h"
#include "ToFillColor.h"
#include "BufferChecks.h"

#include <memory>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_CLASS(MT_RGBtoRGBUnitTests)
	{
	public:

		//
		// RGBA to RGBx
		//

		TEST_METHOD(MT_RGBA_to_RGB32_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_RGBA, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGBA_to_ARGB1555_UnitTest)
		{
			Run8bitAlphaTestSeries(MVFMT_RGBA, MVFMT_ARGB1555);
		}

		//
		// RGB32 to RGBx
		//

		TEST_METHOD(MT_RGB32_to_RGBA_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_RGBA);
		}

		TEST_METHOD(MT_RGB32_to_RGB24_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_RGB24);
		}

		// Don't use faster-looping for multi-threaded transforming
		//TEST_METHOD(MT_RGB32_to_RGB24_Fast_UnitTest)
		//{
		//	bool savef = get_UseFasterLooping();
		//	set_UseFasterLooping(true);

		//	Run8bitTestSeries(MVFMT_RGB32, MVFMT_RGB24);

		//	set_UseFasterLooping(savef);
		//}

		TEST_METHOD(MT_RGB32_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_RGB565);
		}

		TEST_METHOD(MT_RGB32_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB32, MVFMT_RGB555);
		}

		//
		// RGB24 to RGBx
		//

		TEST_METHOD(MT_RGB24_to_RGB32_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGB24_Fast_to_RGB32_UnitTest)
		{
			bool savef = get_UseFasterLooping();
			set_UseFasterLooping(true);

			Run8bitTestSeries(MVFMT_RGB24, MVFMT_RGB32);

			set_UseFasterLooping(savef);
		}

		TEST_METHOD(MT_RGB24_to_RGB565_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_RGB565);
		}

		TEST_METHOD(MT_RGB24_to_RGB555_UnitTest)
		{
			Run8bitTestSeries(MVFMT_RGB24, MVFMT_RGB555);
		}

		//
		// RGB565 to RGBx
		//

		TEST_METHOD(MT_RGB565_to_RGB32_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGB565_to_RGB24_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_RGB24);
		}

		TEST_METHOD(MT_RGB565_to_RGB555_UnitTest)
		{
			Run565bitTestSeries(MVFMT_RGB565, MVFMT_RGB555);
		}

		//
		// RGB555 to RGBx
		//

		TEST_METHOD(MT_RGB555_to_RGB32_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGB555_to_RGBA_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_RGBA);
		}

		TEST_METHOD(MT_RGB555_to_RGB24_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_RGB24);
		}

		TEST_METHOD(MT_RGB555_to_RGB565_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_RGB565);
		}

		TEST_METHOD(MT_RGB555_to_ARGB1555_UnitTest)
		{
			Run555bitTestSeries(MVFMT_RGB555, MVFMT_ARGB1555);
		}

		//
		// ARGB1555 to RGBx
		//

		TEST_METHOD(MT_ARGB1555_to_RGBA_UnitTest)
		{
			Run555AlphabitTestSeries(MVFMT_ARGB1555, MVFMT_RGBA);
		}

		TEST_METHOD(MT_ARGB1555_to_RGB555_UnitTest)
		{
			Run555AlphabitTestSeries(MVFMT_ARGB1555, MVFMT_RGB555);
		}

		//
		// RGB8 to RGBx
		//

		TEST_METHOD(MT_RGB8_to_RGB32_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGB8_to_RGB24_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_RGB24);
		}

		// Don't use faster-looping for multi-threaded transforming
		//TEST_METHOD(MT_RGB8_to_RGB24_Fast_UnitTest)
		//{
		//	bool savef = get_UseFasterLooping();
		//	set_UseFasterLooping(true);

		//	Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_RGB24);

		//	set_UseFasterLooping(savef);
		//}

		TEST_METHOD(MT_RGB8_to_RGB565_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_RGB565);
		}

		TEST_METHOD(MT_RGB8_to_RGB555_UnitTest)
		{
			Run8bitPalletizedTestSeries(MVFMT_RGB8, MVFMT_RGB555);
		}

		//
		// RGB4 to RGBx
		//

		TEST_METHOD(MT_RGB4_to_RGB32_UnitTest)
		{
			Run4bitPalletizedTestSeries(MVFMT_RGB4, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGB4_to_RGB24_UnitTest)
		{
			Run4bitPalletizedTestSeries(MVFMT_RGB4, MVFMT_RGB24);
		}

		// Don't use faster-looping for multi-threaded transforming
		//TEST_METHOD(MT_RGB4_to_RGB24_Fast_UnitTest)
		//{
		//	bool savef = get_UseFasterLooping();
		//	set_UseFasterLooping(true);

		//	Run4bitPalletizedTestSeries(MVFMT_RGB4, MVFMT_RGB24);

		//	set_UseFasterLooping(savef);
		//}

		TEST_METHOD(MT_RGB4_to_RGB565_UnitTest)
		{
			Run4bitPalletizedTestSeries(MVFMT_RGB4, MVFMT_RGB565);
		}

		TEST_METHOD(MT_RGB4_to_RGB555_UnitTest)
		{
			Run4bitPalletizedTestSeries(MVFMT_RGB4, MVFMT_RGB555);
		}

		//
		// RGB1 to RGBx
		//

		TEST_METHOD(MT_RGB1_to_RGB32_UnitTest)
		{
			Run1bitPalletizedTestSeries(MVFMT_RGB1, MVFMT_RGB32);
		}

		TEST_METHOD(MT_RGB1_to_RGB24_UnitTest)
		{
			Run1bitPalletizedTestSeries(MVFMT_RGB1, MVFMT_RGB24);
		}

		// Don't use faster-looping for multi-threaded transforming
		//TEST_METHOD(MT_RGB1_to_RGB24_Fast_UnitTest)
		//{
		//	bool savef = get_UseFasterLooping();
		//	set_UseFasterLooping(true);

		//	Run1bitPalletizedTestSeries(MVFMT_RGB1, MVFMT_RGB24);

		//	set_UseFasterLooping(savef);
		//}

		TEST_METHOD(MT_RGB1_to_RGB565_UnitTest)
		{
			Run1bitPalletizedTestSeries(MVFMT_RGB1, MVFMT_RGB565);
		}

		TEST_METHOD(MT_RGB1_to_RGB555_UnitTest)
		{
			Run1bitPalletizedTestSeries(MVFMT_RGB1, MVFMT_RGB555);
		}


	private:

		void Run8bitTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

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

		void Run8bitAlphaTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

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

		void RunSingle8bitTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"bufCheckFunctPtr returned a null function pointer.");

			uint32_t width = MTTestBufferWidth;
			uint32_t height = MTTestBufferHeight;

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

			fillBufFunctPtr(red, green, blue, alpha, width, height, inBufPtr, in_stride);

			t_stagetransformfunc pstage_in = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_in), L"FindTransformStage for inFormat returned a null function pointer.");

			t_stagetransformfunc pstage_out = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_out), L"FindTransformStage for outFormat returned a null function pointer.");

			RunMultiThreadedTransform(encodeTransPtr,width, height, pstage_in, inBufPtr, in_stride, false, nullptr, pstage_out, outBufPtr, out_stride, false, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(red, green, blue, alpha, width, height, outBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run565bitTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle565bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle565bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingle565bitTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = MTTestBufferWidth;
			uint32_t height = MTTestBufferHeight;

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

			fillBufFunctPtr(red, green, blue, alpha, width, height, inBufPtr, in_stride);

			t_stagetransformfunc pstage_in = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_in), L"FindTransformStage for inFormat returned a null function pointer.");

			t_stagetransformfunc pstage_out = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_out), L"FindTransformStage for outFormat returned a null function pointer.");

			RunMultiThreadedTransform(encodeTransPtr, width, height, pstage_in, inBufPtr, in_stride, false, nullptr, pstage_out, outBufPtr, out_stride, false, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(red & 0xF8, green & 0xFC, blue & 0xF8, alpha, width, height, outBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run555AlphabitTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle555bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::AlphaTestMetaData)
			{
				RunSingle555bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}


		void Run555bitTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle555bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (const RGBATestData& testData : BlipvertUnitTests::TestMetaData)
			{
				RunSingle555bitTest(inFormat, outFormat, testData.red, testData.green, testData.blue, testData.alpha);
			}

			StrideBump = saveb;
		}

		void RunSingle555bitTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
		{
			// RGB to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(fillBufFunctPtr), L"fillBufFunctPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = MTTestBufferWidth;
			uint32_t height = MTTestBufferHeight;

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

			fillBufFunctPtr(red, green, blue, alpha, width, height, inBufPtr, in_stride);

			t_stagetransformfunc pstage_in = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_in), L"FindTransformStage for inFormat returned a null function pointer.");

			t_stagetransformfunc pstage_out = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_out), L"FindTransformStage for outFormat returned a null function pointer.");

			RunMultiThreadedTransform(encodeTransPtr, width, height, pstage_in, inBufPtr, in_stride, false, nullptr, pstage_out, outBufPtr, out_stride, false, nullptr);

			Assert::IsTrue(bufCheckFunctPtr(red & 0xF8, green & 0xF8, blue & 0xF8, alpha, width, height, outBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run8bitPalletizedTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

			for (int index = 0; index < 6; index++)
			{
				Run8bitPalletizedTest(inFormat, outFormat, index);
			}

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

		void Run8bitPalletizedTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t index)
		{
			static xRGBQUAD  rgbpalette[6] = {
				{128, 128, 128, 255},
				{255, 255, 255, 25},
				{0, 0, 0, 25},
				{255, 0, 0, 255},
				{0, 255, 0, 255},
				{0, 0, 255, 255}
			};

			Assert::IsTrue(IsPalletizedEncoding(inFormat), L"Expected a palletized input format.");

			// RGB to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = MTTestBufferWidth;
			uint32_t height = MTTestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
			uint8_t* inBufPtr = inBuf.get();
			memset(inBufPtr, index, inBufBize);

			std::unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
			uint8_t* outBufPtr = outBuf.get();
			memset(outBufPtr, 0, outBufBize);

			t_stagetransformfunc pstage_in = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_in), L"FindTransformStage for inFormat returned a null function pointer.");

			t_stagetransformfunc pstage_out = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_out), L"FindTransformStage for outFormat returned a null function pointer.");

			RunMultiThreadedTransform(encodeTransPtr, width, height, pstage_in, inBufPtr, in_stride, false, rgbpalette, pstage_out, outBufPtr, out_stride, false, rgbpalette);

			Assert::IsTrue(bufCheckFunctPtr(rgbpalette[index].rgbRed, rgbpalette[index].rgbGreen, rgbpalette[index].rgbBlue, 255, width, height, outBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run4bitPalletizedTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

			for (int index = 0; index < 6; index++)
			{
				Run4bitPalletizedTest(inFormat, outFormat, index);
			}

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			for (int index = 0; index < 6; index++)
			{
				Run4bitPalletizedTest(inFormat, outFormat, index);
			}

			StrideBump = saveb;
		}

		void Run4bitPalletizedTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t index)
		{
			static xRGBQUAD  rgbpalette[6] = {
				{128, 128, 128, 255},
				{255, 255, 255, 25},
				{0, 0, 0, 25},
				{255, 0, 0, 255},
				{0, 255, 0, 255},
				{0, 0, 255, 255}
			};

			Assert::IsTrue(IsPalletizedEncoding(inFormat), L"Expected a palletized input format.");

			// RGB to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = MTTestBufferWidth;
			uint32_t height = MTTestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
			uint8_t* inBufPtr = inBuf.get();
			memset(inBufPtr, index << 4 | index, inBufBize);

			std::unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
			uint8_t* outBufPtr = outBuf.get();
			memset(outBufPtr, 0, outBufBize);

			t_stagetransformfunc pstage_in = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_in), L"FindTransformStage for inFormat returned a null function pointer.");

			t_stagetransformfunc pstage_out = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_out), L"FindTransformStage for outFormat returned a null function pointer.");

			RunMultiThreadedTransform(encodeTransPtr, width, height, pstage_in, inBufPtr, in_stride, false, rgbpalette, pstage_out, outBufPtr, out_stride, false, rgbpalette);

			Assert::IsTrue(bufCheckFunctPtr(rgbpalette[index].rgbRed, rgbpalette[index].rgbGreen, rgbpalette[index].rgbBlue, 255, width, height, outBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}

		void Run1bitPalletizedTestSeries(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
		{
			thread_count = GetCommonMaxThreadCount(inFormat, outFormat, MTTestBufferWidth, MTTestBufferHeight, thread::hardware_concurrency());

			uint32_t saveb = StrideBump;
			StrideBump = StrideBumpTestValue;

			Run1bitPalletizedTest(inFormat, outFormat, 0);
			Run1bitPalletizedTest(inFormat, outFormat, 1);

			StrideBump = saveb;
		}

		void Run1bitPalletizedTest(const MediaFormatID& inFormat, const MediaFormatID& outFormat, uint8_t index)
		{
			static xRGBQUAD  rgbpalette[2] = {
				{255, 0, 0, 255},
				{0, 255, 0, 255}
			};

			Assert::IsTrue(IsPalletizedEncoding(inFormat), L"Expected a palletized input format.");

			// RGB to RGB
			t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(encodeTransPtr), L"encodeTransPtr returned a null function pointer.");

			t_buffercheckfunc bufCheckFunctPtr = FindBufferCheckFunction(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(bufCheckFunctPtr), L"yuvCheckFunctPtr returned a null function pointer.");

			uint32_t width = MTTestBufferWidth;
			uint32_t height = MTTestBufferHeight;

			uint32_t in_stride = CalculateStrideBump(inFormat, width, height);
			uint32_t inBufBize = CalculateBufferSize(inFormat, width, height, in_stride);
			Assert::IsTrue(inBufBize != 0, L"inBufBize size retuned zero.");

			uint32_t out_stride = CalculateStrideBump(outFormat, width, height);
			uint32_t outBufBize = CalculateBufferSize(outFormat, width, height, out_stride);
			Assert::IsTrue(outBufBize != 0, L"outBufBize size retuned zero.");

			std::unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
			uint8_t* inBufPtr = inBuf.get();
			memset(inBufPtr, index ? 0xFF : 0x00, inBufBize);

			std::unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
			uint8_t* outBufPtr = outBuf.get();
			memset(outBufPtr, 0, outBufBize);

			t_stagetransformfunc pstage_in = FindTransformStage(inFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_in), L"FindTransformStage for inFormat returned a null function pointer.");

			t_stagetransformfunc pstage_out = FindTransformStage(outFormat);
			Assert::IsNotNull(reinterpret_cast<void*>(pstage_out), L"FindTransformStage for outFormat returned a null function pointer.");

			RunMultiThreadedTransform(encodeTransPtr, width, height, pstage_in, inBufPtr, in_stride, false, rgbpalette, pstage_out, outBufPtr, out_stride, false, rgbpalette);

			Assert::IsTrue(bufCheckFunctPtr(rgbpalette[index].rgbRed, rgbpalette[index].rgbGreen, rgbpalette[index].rgbBlue, 255, width, height, outBufPtr, out_stride), L"RGB buffer did not contain expected values.");
		}
	};
}
