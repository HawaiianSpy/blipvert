# blipvert
C++ library for converting between fourcc video format bitmaps.

I created this library as part of an old Windows C++ video project (circa 2005). One objective of that project was the support of every possible webcam and video capture card. The biggest hurdle at that time was handling all the weird bitmap formats output from these devices. So, using the information at fourcc.org, I set out to write conversion functions so that even the one-of-a-kind bitmaps generated from obscure webcams could be supported by the project.

I originally called the project "blipvert" because I was a fan of the Max Headroom series. I decided to keep the name because makes a catchy C++ namespace name, too.

After looking at the code, you may be asking yourself, "Why the heck would I *ever* want to convert a Y41P bitmap to IYU2?" Probably never, nowadays. Yet, I would suggest you view this act of video programming obsessive-compulsive disorder like achieveing a gold badge on HackerRank before it was cool. Instead of whiling away the hours moving nonsensecal numbers around in random arrays, as one does on HackerRank, at least I had some, more or less, useful code to show for it afterwards. =:D

Regarding testing: The transform functions were tested back in the day using this porocess:

1. Run a webcam through a DirectShow filter graph.
2. Insert one of the transform functions into the graph.
3. Transform whatever the output bitmap was into a format that can be displayed by a video renderer.
4. Does it look like the same video stream that went in? If so, "pass." If not, "fail." 

I suppose one could implement an automated testing regimen to compare the outputs, but Since the conversion back and forth between RGB and YUV bitmaps is lossy, it would probably be more trouble than it's worth. However, I will spot test some of the transforms to see if I broke anyting replacing the old Visual Studio defines with those for modern C++. I have started unit tests for the library, but mostly, the project testing status is, "Hey man, it builds!"

Regards,

Don Jordan

June 13, 2021

******************************


# The Ten-Cent Tour Of The API

### Header file: blipvert.h (Start Here)

#### Data Types:

#### MediaFormatID
A string value representing a video bitmap format. Since there aren't fourcc codes for *all* the formats (RGB bitmaps for example), these strings can be converted to fourcc codes it happens to match one.


#### Fourcc
A 32-bit unsigned integer containing a real fourcc code.


#### t_transformfunc
The function pointer definition used for all of the transform functions.


#### Functions:
See the comments in the header files for details on parameters, etc.


#### void InitializeLibrary(void);
*Always call this function first to initialize the library!* This sets up the lookup tables and maps that makes this code do it's thing.


#### t_transformfunc FindVideoTransform(const MediaFormatID& inFormat, const MediaFormatID& outFormat);
Returns a function pointer that will convert the requested input format to the requested output format.


#### bool GetVideoFormatInfo(const MediaFormatID& inFormat, Fourcc& fourcc, Fourcc& xRefFourcc, int16_t& effctiveBitsPerPixel);
Returns useful information about the media type including the fourcc code (if it exists), main fourcc code (if the matching fourcc code is a duplicate), and the bits / pixel (good for calculating the buffer size).

#### bool GetVideoFormatID(Fourcc fourcc, MediaFormatID& outFormat);
Returns the MediaFormatID for the given fourcc code.

*More to come...*
