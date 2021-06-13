# blipvert
C++ library for converting between fourcc video format bitmaps.

I created this library as part of an old Windows C++ video project (circa 2005). One objective of that project was the support of every possible webcam and video capture card. The biggest hurdle at that time was handling all the weird bitmap formats output from these devices. So, using the information at fourcc.org, I set out to write conversion functions so that even the one-of-a-kind bitmaps generated from obscure webcams could be supported by the project.

It will take me a bit of time to get all of the code posted since there's a lot that I'm updating to modern C++ and removing Visual Studio-specific definitions. This should make the source more portable.

I originally called the project "blipvert" because I was a fan of the Max Headroom series. I decided to keep the name because makes a catchy C++ namespace name, too.

******************

TESTING STATUS as of 13 June, 2021: The root transform functions were tested back in the day. HOWEVER, I need to spot test some of them to see if I broke anyting replacing the old Visual Studio defines with those for modern C++. I have started unit tests for the library, but mostly, project status is, "Hey man, it builds!"

Regards,

Don Jordan
