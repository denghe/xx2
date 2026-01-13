#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#	define NOMINMAX
#endif
//#	define NODRAWTEXT
//#	define NOGDI            // d3d9 need it
#	define NOBITMAP
#	define NOMCX
#	define NOSERVICE
#	define NOHELP
#	define WIN32_LEAN_AND_MEAN
#   include <WinSock2.h>
#   include <process.h>
#	include <Windows.h>
#   include <intrin.h>     // _BitScanReverseXXXX _byteswap_XXXX
#   include <ShlObj.h>
#   include <mmsystem.h>	// timeBeginPeriod
#else
#	include <unistd.h>    // for usleep
#   include <arpa/inet.h>  // __BYTE_ORDER __LITTLE_ENDIAN __BIG_ENDIAN
#endif

#include <GLFW/glfw3.h>
#define GLFW_MOUSE_BUTTON_WHEEL_UP (GLFW_MOUSE_BUTTON_LAST + 1)
#define GLFW_MOUSE_BUTTON_WHEEL_DOWN (GLFW_MOUSE_BUTTON_LAST + 2)
#define GLFW_MOUSE_BUTTON_WHEEL_LEFT (GLFW_MOUSE_BUTTON_LAST + 3)
#define GLFW_MOUSE_BUTTON_WHEEL_RIGHT (GLFW_MOUSE_BUTTON_LAST + 4)
#include <GLFW/glfw3native.h>
#ifdef WIN32
#pragma comment (lib ,"imm32.lib")
#endif
