//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021-2025 Don Jordan
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
#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_MODULE_INITIALIZE(BlipvertUnitTests_ModuleInit)
	{
		InitializeLibrary();
	}

	TEST_CLASS(FindVideoTransform_UnitTests)
	{
	public:

		TEST_METHOD(FindValidTransform_UnitTest)
		{
			t_transformfunc func = FindVideoTransform(MVFMT_UYVY, MVFMT_RGB32);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(PackedY422_to_RGB32), reinterpret_cast<void*>(func), L"FindVideoTransform returned the wrong function pointer.");

			func = FindVideoTransform(MVFMT_RGB555, MVFMT_RGBA);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(RGB555_to_RGBA), reinterpret_cast<void*>(func), L"FindVideoTransform returned the wrong function pointer.");

			 func = FindVideoTransform(MVFMT_RGB32, MVFMT_RGB565);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(RGB32_to_RGB565), reinterpret_cast<void*>(func), L"FindVideoTransform returned the wrong function pointer.");


			func = FindVideoTransform(MVFMT_cyuv, MVFMT_Y411);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(PackedY422_to_IYU1), reinterpret_cast<void*>(func), L"FindVideoTransform returned the wrong function pointer.");
		}

		TEST_METHOD(FindInvalidTransform_UnitTest)
		{
			t_transformfunc func = FindVideoTransform(MVFMT_UYVY, MediaFormatID("wowzo"));
			Assert::IsNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a non-null function pointer.");
		}

		TEST_METHOD(FindGreyscaleTransform_UnitTest)
		{
			t_greyscalefunc func = FindGreyscaleTransform(MVFMT_UYVY);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindGreyscaleTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(UYVY_to_Greyscale), reinterpret_cast<void*>(func), L"FFindGreyscaleTransform returned the wrong function pointer.");

			func = FindGreyscaleTransform(MVFMT_cyuv);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindGreyscaleTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(UYVY_to_Greyscale), reinterpret_cast<void*>(func), L"FindGreyscaleTransform returned the wrong function pointer.");
		}

		TEST_METHOD(FindInvalidGreyscaleTransform_UnitTest)
		{
			t_greyscalefunc func = FindGreyscaleTransform(MediaFormatID("wowzo"));
			Assert::IsNull(reinterpret_cast<void*>(func), L"FindGreyscaleTransform returned a non-null function pointer.");
		}

		TEST_METHOD(FindFillColorTransform_UnitTest)
		{
			t_fillcolorfunc func = FindFillColorTransform(MVFMT_UYVY);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindFillColorTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(Fill_UYVY), reinterpret_cast<void*>(func), L"FindFillColorTransform returned the wrong function pointer.");

			func = FindFillColorTransform(MVFMT_cyuv);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindFillColorTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(Fill_UYVY), reinterpret_cast<void*>(func), L"FindFillColorTransform returned the wrong function pointer.");

			func = FindFillColorTransform(MVFMT_RGB565);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindFillColorTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(Fill_RGB565), reinterpret_cast<void*>(func), L"FindFillColorTransform returned the wrong function pointer.");
		}

		TEST_METHOD(FindInvalidFillColorTransform_UnitTest)
		{
			t_fillcolorfunc func = FindFillColorTransform(MediaFormatID("wowzo"));
			Assert::IsNull(reinterpret_cast<void*>(func), L"FindFillColorTransform returned a non-null function pointer.");
		}
	};

	TEST_CLASS(GetVideoFormatInfo_UnitTests)
	{
	public:

		TEST_METHOD(GetValidVideoFormatInfo_UnitTest)
		{
			VideoFormatInfo info;

			Assert::IsTrue(GetVideoFormatInfo(MVFMT_YUNV, info), L"GetVideoFormatInfo: Expected to find format info.");
			Assert::AreEqual(FOURCC_YUNV, info.fourcc, L"GetVideoFormatInfo: Wrong fourcc returned for media ID.");
			Assert::AreEqual(FOURCC_YUY2, info.xRefFourcc, L"GetVideoFormatInfo: Wrong cross-referenced fourcc returned for media ID.");
			Assert::AreEqual(static_cast<int32_t>(info.effectiveBitsPerPixel), 16, L"GetVideoFormatInfo: Wrong bpp returned for media ID.");
		}

		TEST_METHOD(GetInvalidVideoFormatInfo_UnitTest)
		{
			VideoFormatInfo info;

			Assert::IsFalse(GetVideoFormatInfo(MVFMT_UNDEFINED, info), L"GetVideoFormatInfo: Expected to not find format info.");
		}
	};

	TEST_CLASS(GetVideoFormatID_UnitTests)
	{
	public:

		TEST_METHOD(GetValidVideoFormatID_UnitTest)
		{
			MediaFormatID outMediaID;

			Assert::IsTrue(GetVideoFormatID(FOURCC_YUY2, outMediaID), L"GetVideoFormatID: Did not return a media ID.");
			Assert::AreEqual(MVFMT_YUY2, outMediaID, L"GetVideoFormatID did not return the correct media ID.");
		}

		TEST_METHOD(GetInvalidVideoFormatID_UnitTest)
		{
			MediaFormatID outMediaID;

			Assert::IsFalse(GetVideoFormatID(FOURCC_UNDEFINED, outMediaID), L"GetVideoFormatID: Returned a valid media ID.");
		}
	};
}
