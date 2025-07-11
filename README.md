# blipvert
A C++ library for converting between [fourcc video format bitmaps](https://www.fourcc.org/).

I created this library as part of an old Windows C++ video project (circa 2005). One objective of that project was the support of every possible webcam and video capture card. The biggest hurdle at that time was handling all the weird bitmap formats output from these devices. So, using the information at fourcc.org, I set out to write conversion functions so that even the one-of-a-kind bitmaps generated from obscure webcams could be supported by the project.

I originally called the project "blipvert" because I was a fan of the Max Headroom series. I decided to keep the name because it makes a catchy C++ namespace name, too.

After looking at the code, you may be asking yourself, "Why the heck would I *ever* want to convert a Y41P bitmap to IYU2?" Probably never, nowadays. Yet, I would suggest you view this act of video programming obsessive-compulsive disorder like achieveing a gold badge on HackerRank before it was cool. Instead of whiling away the hours moving nonsensecal numbers around in random arrays, as one does on HackerRank, at least I had some, more or less, useful code to show for it afterwards. =:D

As of 25 July, 2021, Blipvert now has a complete unit test suite that covers all the code in the transforms.

Regards,

Don Jordan

San Diego, California, USA

P.S. If you found this library helpful and have improvements/bug fixes to contribute, I would appreciate it if you would submit them back here for review. Thanks.

******************************

#### The ```TransformFramerateTests``` project is a Windows console application that tests and displays the frame rates for various transforms at the HD (1920 x 1080) and 4K (3840 x 2160) video resolutions.


******************************


# Ten-Cent Tour Of The API

## 27 October, 2021: I have made a major change to the API: The staging for each transform pass is now setup ahead of time to eliminate redundantly doing it for each frame. This will make it more efficient and allow the use of multiple threads to perform the transforms on large bitmaps in real time. The multi-thread part has not been tested, but the single tread operation has been tested with the updated API. This documentation is still for the old API. Until I update this document one can examine the TransformFramerateTests source code to see how the new API works. I have a full-time job now, so that takes up most of my time. I'll set aside a weekend soon to update this documentation and produce tests for multi-threaded processing of the transforms.

## Update 11, July 2025: Removed the multi-threaded frame rate test because it doen't work correctly. Needs more testing.

## Update 11, July 2025 @ 1:19 PM PDT: Reworked the broken MCIx bitmap transforms. The buffer calculations were wrong. Also revised the rules for the bitmap dimensions. See below. 

Read the header files for the details. Fairly self-explainatory.

### Header file: blipvert.h (Start Here)

#### IMPORTANT: For reasons related to the bitmap format definitions, all input parameters must follow these rules:

1. The width value must be >= 8, and an even multiple of 8.
2. The height value must be >= 16, and an even multiple of 4.
3. The stride value must be >= the minimum number of bytes-per-line needed for the width of the bitmap format.

The minimum granularity for the bitmap dimensions is 8 pixels for both width and height. Transforms, like Y41P, horizontally represent the pixels in 8-pixel chunks. Other transforms, like YUV9 or YVU9, use 4x4 chunks.  All the digital video bitmaps I have seen have their dimensions in pixels at this granularity anyway.

More Information: [Wackypedia article on webcams](https://en.wikipedia.org/wiki/Webcam) and [Understanding Video Resolutions](https://web.archive.org/web/20210604160142/https://www.borrowlenses.com/blog/understanding-video-resolutions/).

#### Data Types:

#### ```MediaFormatID```
A string value representing a video bitmap format. These strings can be converted to fourcc codes if it happens to match one. But there are some formats (e.g. RGBs) that don't have corresponding fourcc codes.
#
#### ```Fourcc```
A 32-bit unsigned integer containing a real fourcc code.
#
#### ```t_transformfunc```
The function pointer definition used for all of the transform functions.
#
#### ```t_greyscalefunc```
The function pointer definition used for in-place conversion to greyscale.
#
#### ```t_fillcolorfunc```
The function pointer definition used for in-place color fills.
#
#### ```t_setpixelfunc```
The function pointer definition used for setting individual pixels in a bitmap. This is a hack for the unit testing for now.
#
#### ```t_flipverticalfunc```
The function pointer definition used for vertically flipping a bitmap in place. This is a hack for the unit testing for now.
#
#### ```t_calcbuffsizefunc```
The function pointer definition used for calculating the size of a bitmap's buffer. Easier to use the existing ```CalculateBufferSize``` function, but that function does call this.
#
#### ```VideoFormatInfo```
Structure containing info for a particular video format.
#
#### Functions:
See the comments in the header files for details on parameters, etc.


#### ```void InitializeLibrary(void);```
*Always call this function first to initialize the library!* This sets up the lookup tables and maps that makes this code do it's thing.
#
#### ```bool get_UseFasterLooping();```
#### ```void set_UseFasterLooping(bool value);```
If set to true, the library uses faster loops with RGB24 bitmaps. The overwriting can go outside of the expected buffer size. See header file.
#
#### ```t_transformfunc FindVideoTransform(const MediaFormatID& inFormat, const MediaFormatID& outFormat);```
Returns a function pointer that will convert the requested input format to the requested output format.
#
#### ```t_greyscalefunc FindGreyscaleTransform(const MediaFormatID& inFormat);```
Returns a function pointer that will perform an in-place conversion of the bitmap to greyscale.
#
#### ```t_fillcolorfunc FindFillColorTransform(const MediaFormatID& inFormat);```
Returns a function pointer that will perform an in-place color fill of the bitmap.
#
#### ```t_setpixelfunc FindSetPixelColor(const MediaFormatID& inFormat);```
Returns a function pointer for an individual pixel setter for the given format.
#
#### ```t_flipverticalfunc FindFlipVerticalTransform(const MediaFormatID& inFormat);```
Returns a function pointer for a vertical flip in place video transform for the given input media format.
#
#### ```t_calcbuffsizefunc FindBufSizeCalculator(const MediaFormatID& inFormat);```
Returns a function pointer for a buffer size calculation function of the given format.
#
#### ```bool GetVideoFormatInfo(const MediaFormatID& inFormat, VideoFormatInfo& info);```
Returns useful information about the media type.
#
#### ```bool GetVideoFormatID(Fourcc fourcc, MediaFormatID& outFormat);```
Returns the MediaFormatID for the given fourcc code.

******************************

### Header file: Utilities.h (Some useful stuff here)

#### Functions:

#### ```void SlowYUVtoRGB(double Y, double U, double V, double* R, double* G, double* B);```
#### ```void SlowRGBtoYUV(double R, double G, double B, double* Y, double* U, double* V);```
Accurate floating-point colorspace conversion functions. Slow but percise.
#
#### ```void FastYUVtoRGB(uint8_t Y, uint8_t U, uint8_t V, uint8_t* R, uint8_t* G, uint8_t* B);```
#### ```void FastRGBtoYUV(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* U, uint8_t* V);```
Fast colospace conversion using lookup tables. Good enough for 99.5% of the applications out there for 8-bit channels. Maximum variation is 1/256 + or -.
#
#### ```uint32_t CalculateBufferSize(const MediaFormatID& inFormat, uint32_t width, uint32_t height, uint32_t in_stride = 0);```
 Calculates size of the buffer given the video format and dimensions.
 #
#### ```int32_t CalculateMinimumLineStride(const MediaFormatID& inFormat, uint32_t width, uint32_t height);```
 Returns the minimum size in bytes needed to contain one horizontal line of the bitmap.
#

#### ```bool IsRGBColorspace(const MediaFormatID& encoding);```
#### ```bool IsRGBColorspace(const Fourcc fourcc);```
#### ```bool IsYUVColorspace(const MediaFormatID& encoding);```
#### ```bool IsYUVColorspace(const Fourcc fourcc));```
#### ```bool IsPalletizedEncoding(const MediaFormatID& encoding);```
#### ```bool IsPalletizedEncoding(const Fourcc fourcc);```
Returns *true* if the input is what the function name states, and returns *false* otherwise.
