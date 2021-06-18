# blipvert
C++ library for converting between fourcc video format bitmaps.

I created this library as part of an old Windows C++ video project (circa 2005). One objective of that project was the support of every possible webcam and video capture card. The biggest hurdle at that time was handling all the weird bitmap formats output from these devices. So, using the information at fourcc.org, I set out to write conversion functions so that even the one-of-a-kind bitmaps generated from obscure webcams could be supported by the project.

I originally called the project "blipvert" because I was a fan of the Max Headroom series. I decided to keep the name because makes a catchy C++ namespace name, too.

After looking at the code, you may be asking yourself, "Why the heck would I *ever* want to convert a Y41P bitmap to IYU2?" Probably never, nowadays. Yet, I would suggest you view this act of video programming obsessive-compulsive disorder like achieveing a gold badge on HackerRank before it was cool. Instead of whiling away the hours moving nonsensecal numbers around in random arrays, as one does on HackerRank, at least I had some, more or less, useful code to show for it afterwards. =:D

Regarding testing: The transform functions were tested back in the day using this process:

1. Run a webcam through a DirectShow filter graph.
2. Insert one of the transform functions into the graph.
3. Transform whatever the output bitmap was into a format that can be displayed by a video renderer.
4. Does it look like the same video stream that went in? If so, "pass." If not, "fail." 

I added some spot unit tests for the transforms converting RGB32 to YUV formats and back to RGB32. Unit tests for the YUV formats that average the color channel values (U & V) for a lossy type of compression are just too messy to implement in a practical manner for this project. I also added some unit tests for the library functions and for the RGB to YUV / YUV to RGB lookup tables.

Regards,

Don Jordan

San Diego, California, USA

P.S. If you found this library helpful and have improvements/bug fixes to contribute, I would appreciate it if you would submit them back here for review. Thanks.



******************************


# The Ten-Cent Tour Of The API
*This will have to do until I can get the wiki going!*

### Header file: blipvert.h (Start Here)

#### IMPORTANT: All pixel bitmap dimensions (width & height) should  be multiples of 4 to accomodate the packed YUV formats and general algorithmic speed improvements. You can get away with non-modulo-four values for many of the transforms, but following that rule will insure maximum compatibilty across the library. The consequences for living on the edge? Some of the transforms may try coloring outside the lines. Of course you can experiment with the buffer sizes to work around that.

#### Data Types:

#### MediaFormatID
A string value representing a video bitmap format. These strings can be converted to fourcc codes if it happens to match one. But there are some formats (e.g. RGBs) that don't have corresponding fourcc codes.


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

#### t_greyscalefunc FindGreyscaleTransform(const MediaFormatID& inFormat);
Returns a function pointer that will perform an in-place conversion of the bitmap to greyscale.


#### bool GetVideoFormatInfo(const MediaFormatID& inFormat, Fourcc& fourcc, Fourcc& xRefFourcc, int16_t& effectiveBitsPerPixel);
Returns useful information about the media type including the fourcc code (if it exists), main fourcc code (if the matching fourcc code is a duplicate), and the bits / pixel (good for calculating the buffer size).

#### bool GetVideoFormatID(Fourcc fourcc, MediaFormatID& outFormat);
Returns the MediaFormatID for the given fourcc code.


