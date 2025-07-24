# blipvert
A C++ library for converting between [fourcc video format bitmaps](https://www.fourcc.org/) and others.

I created this library as part of an old Windows C++ video project (circa 2005). One objective of that project was the support of every possible webcam and video capture card. The biggest hurdle at that time was handling all the weird bitmap formats output from these devices. So, using the information at fourcc.org, I set out to write conversion functions so that even the one-of-a-kind bitmaps generated from obscure webcams could be supported by the project.

I originally called the project "blipvert" because I was a fan of the Max Headroom series. I decided to keep the name because it makes a catchy C++ namespace name, too.

After looking at the code, you may be asking yourself, "Why the heck would I *ever* want to convert a Y41P bitmap to IYU2?" Probably never, nowadays. Yet, I would suggest you view this act of video programming obsessive-compulsive disorder like achieveing a gold badge on HackerRank before it was cool. Instead of whiling away the hours moving nonsensecal numbers around in random arrays, as one does on HackerRank, at least I had some, more or less, useful code to show for it afterwards. =:D

The IDE projects are for Visual Studio 2022 for Windows. However I have tried to keep the core library as generic C++, so it should work, with trivial changes, under other operating systems and compilers.

Blipvert has official support for multi-threading in the transforms. You can split up the transformation of large video frames amoung worker threads.

Blipvert as a complete unit test suite that covers the single and multi-threaded code in the transforms. I have not yet done any multi-threaded testing flipping the bitmaps on the fly. That will be for the future--hopefully a lot sooner than 4 years!

Regards,

Don Jordan

San Diego, California, USA

P.S. If you found this library helpful and have improvements/bug fixes to contribute, I would appreciate it if you would submit them back here for review. Thanks.

P.P.S. I do intend to make a better version of this read me file.


******************************

#### The ```TransformFramerateTests``` project is a single-threaded Windows console application that tests and displays the frame rates for various transforms at the HD (1920 x 1080) and 4K (3840 x 2160) video resolutions.

#### The ```MTTransformFramerateTests``` project is a multi-threaded Windows console application that tests and displays the frame rates for various transforms at the HD (1920 x 1080) and 4K (3840 x 2160) video resolutions. It spawns as many threads a possible just to beat on the code. Usually, given the OS overhead, four threads would probably be faster than thirty. Experiment with the number of threads yourself.


******************************


# Ten-Cent Tour Of The API

Read the header files for the details. Fairly self-explainatory--more or less. Since real working code is a better explaination than a bunch of verbage, take a look at the frame rate and unit test projects to see the transforms in action.

### Header file: blipvert.h (Start Here)

#### IMPORTANT: For reasons related to the bitmap format definitions, all input parameters must follow these rules:

1. The width value must be >= 8, and an even multiple of 8.
2. The height value must be >= 16, and an even multiple of 4.
3. The stride value must be >= the minimum number of bytes-per-line needed for the width of the bitmap format.

The minimum granularity for the bitmap dimensions is 8 pixels for the width and 4 pixels for the height. Fourcc formats, like Y41P, horizontally represent the pixels in 8-pixel chunks. Other formats, like YUV9 or YVU9, use 4x4 chunks.  All the digital video bitmaps I have seen have their dimensions in pixels at this granularity anyway.

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
The function pointer definition used for calculating the size of a bitmap's buffer.
#
#### ```t_stagetransformfunc```
The function pointer definition used for initializing the ```Stage``` structure.
#
#### ```VideoFormatInfo```
Structure containing info for a particular video format.
#
#### Functions:
See the comments in the header files for details on parameters, etc.


#### ```void InitializeLibrary(void);```
*Always call this function first to initialize the library!* This sets up the lookup tables and maps that makes this code do it's thing.
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
#### ```t_stagetransformfunc FindTransformStage(const MediaFormatID& format);```
Returns a function pointer for a staging function for the given input media format. This functions will initialize the ```Stage``` struct for the specified media format.
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
#### ```bool IsPlanarYUV(const MediaFormatID& encoding);```
#### ```bool IsPlanarYUV(const Fourcc fourcc);```
Returns *true* if the input is what the function name states, and returns *false* otherwise.
#

#### ```bool IsGloballyValidBitmapDimension(int32_t width, int32_t height);```
Returns *true* if the given bitmap logical dimensions are within the constraints.

******************************

### Header file: Staging.h

These are the staging functions. They initialize the ```Stage``` struct that tells the transforms where to work their magic. The staging function will set up all the values for slicing and dicing the bitmaps for 1 to many worker threads. The signature for all of the staging functions follow this format:

#### t_stagetransformfunc(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette);

##### Parameters:


Stage - The pointer to the Stage struct to be initialized.

thread_index - 0 based thread index this stage is for. Use 0 if running a single thread.

thread_count - The total number of threads operating this bitmap. Use 1 running a single thread.

width , height - The dimensions of the source bitmap.

buf - pointer to the bitmap in memory.

stride - numebr of bytes between horizontal lines in tthe bitmap.

flipped - true if the bitmap is flipped.

palette - palette used to indexed RGB bitmaps.



#### Multi-threaded pseudo-code example.

### // Set up stuff

t_transformfunc encodeTransPtr = FindVideoTransform(inFormat, outFormat);

t_stagetransformfunc pstage_in = FindTransformStage(inFormat);

t_stagetransformfunc pstage_out = FindTransformStage(outFormat);


### // Slice bitmap up for two worker threads

int thread_count = 2;


### // In-out stages for the first slice.

Stage inStage0;

pstage_in(&inStage0, 0, thread_count, width, height, inBufPtr, in_stride, false, nullptr);

Stage outStage0;

pstage_out(&outStage0, 0, thread_count, width, height, outBufPtr, out_stride, false, nullptr);


### // In-out stages for the second slice.

Stage inStage1;

pstage_in(&inStage1, 1, thread_count, width, height, inBufPtr, in_stride, false, nullptr);

Stage outStage1;

pstage_out(&outStage1, 1, thread_count, width, height, outBufPtr, out_stride, false, nullptr);


### // Transform the bitmap one slice at a time.

### // This line of code goes into the executable for worker thread 1.
encodeTransPtr(&inStage0, &outStage0);

### // This line of code goes into the executable for worker thread 2.
encodeTransPtr(&inStage1, &outStage1);

