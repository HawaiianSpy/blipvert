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
			Assert::AreEqual(reinterpret_cast<void*>(UYVY_to_RGB32), reinterpret_cast<void*>(func), L"FindValidTransform_UnitTest returned the wrong function pointer.");

			func = FindVideoTransform(MVFMT_cyuv, MVFMT_Y411);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(UYVY_to_IYU1), reinterpret_cast<void*>(func), L"FindValidTransform_UnitTest returned the wrong function pointer.");
		}

		TEST_METHOD(FindInvalidTransform_UnitTest)
		{
			t_transformfunc func = FindVideoTransform(MVFMT_UYVY, MVFMT_Plum);
			Assert::IsNull(reinterpret_cast<void*>(func), L"FindVideoTransform returned a non-null function pointer.");
		}
	};

	TEST_CLASS(GetVideoFormatInfo_UnitTests)
	{
	public:

		TEST_METHOD(GetValidVideoFormatInfo_UnitTest)
		{
			Fourcc outfourcc;
			Fourcc outxRefFourcc;
			int16_t outeffctiveBitsPerPixel;

			Assert::IsTrue(GetVideoFormatInfo(MVFMT_YUNV, outfourcc, outxRefFourcc, outeffctiveBitsPerPixel), L"GetVideoFormatInfo: Expected to find format info.");
			Assert::AreEqual(FOURCC_YUNV, outfourcc, L"GetVideoFormatInfo: Wrong fourcc returned for media ID.");
			Assert::AreEqual(FOURCC_YUY2, outxRefFourcc, L"GetVideoFormatInfo: Wrong cross-referenced fourcc returned for media ID.");
			Assert::AreEqual(static_cast<int32_t>(outeffctiveBitsPerPixel), 16, L"GetVideoFormatInfo: Wrong bpp returned for media ID.");
		}

		TEST_METHOD(GetInvalidVideoFormatInfo_UnitTest)
		{
			Fourcc outfourcc;
			Fourcc outxRefFourcc;
			int16_t outeffctiveBitsPerPixel;

			Assert::IsFalse(GetVideoFormatInfo(MVFMT_UNDEFINED, outfourcc, outxRefFourcc, outeffctiveBitsPerPixel), L"GetVideoFormatInfo: Expected to not find format info.");
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
