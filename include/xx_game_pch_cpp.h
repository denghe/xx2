// use pch.cpp include this

#ifndef __EMSCRIPTEN__

#define GLAD_MALLOC(sz)       (new char[sz])
#define GLAD_FREE(ptr)        delete[](ptr)
#define GLAD_GL_IMPLEMENTATION
#ifndef __EMSCRIPTEN__
#include <gl33.h>
#endif


//#define STBI_NO_JPEG
//#define STBI_NO_PNG
#define STBI_NO_GIF
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_HDR
#define STBI_NO_TGA
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#endif
