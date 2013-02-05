#pragma once

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <float.h>
#include <stdarg.h>
#include <cmath>

#pragma warning (disable : 4251)

namespace Myway
{

#if defined _WIN32
#define MW_PLATFORM_WIN32
#elif defined _LINUX
#define MW_PLATFORM_LINUX
#elif defined _UNIX
#define MW_PLATFORM_UNIX
#endif


#ifndef NULL
#define NULL				0
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#define MW_ALIGN16            _declspec(align(16))

#ifdef MW_EXPORT

#define MW_ENTRY __declspec(dllexport)

#else

#define MW_ENTRY __declspec(dllimport)

#endif
    
#define DEFAULT_INCREASE_SIZE 10

//command script
#define MAX_LINE_SIZE       1024
#define MAX_CMD_SIZE        128
#define MAX_PARAM_SIZE      128

#ifndef NPOS
#define NPOS                -1
#endif

#define M_ARGB(a, r, g, b)    ((int)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define M_XRGB(r, g, b)       M_ARGB(0xFF, r, g, b)
#define M_RGBA(r, g, b, a)    M_ARGB(a, r, g, b)

#define M_ARGB_F(a, r, g, b)  M_ARGB((int)(a * 255), (int)(r * 255), (int)(g * 255), (int)(b * 255))

#define M_ARGB_A(c)           ((unsigned int(c) & 0xFF000000) >> 24)
#define M_ARGB_R(c)           ((unsigned int(c) & 0x00FF0000) >> 16)
#define M_ARGB_G(c)           ((unsigned int(c) & 0x0000FF00) >> 8)
#define M_ARGB_B(c)           ((unsigned int(c) & 0x000000FF) >> 0)

#define MEMORY_IN_SYSTEM    0
#define MEMORY_IN_POOL16    16
#define MEMORY_IN_POOL32    32
#define MEMORY_IN_POOL64    64
#define MEMORY_IN_POOL128   128
#define MEMORY_IN_POOL256   256

#define MEMORY_ALIGN_NONE	0
#define MEMORY_ALIGN_4		4
#define MEMORY_ALIGN_8		8
#define MEMORY_ALIGN_16		16
#define MEMORY_ALIGN_32		32

#define MW_ALIGN_ENUM(enumerate) __force_dword_##enumerate = 0x7FFFFFFF

typedef __int64 int64;
typedef unsigned __int64 uint64;

/*
#ifdef _UNICODE

	typedef wchar_t		tchar;

	#define _text(a)	(L##a);

	#define _tstrlen	wcslen
	#define _tstrcpy	wcscpy
	#define _tstrncpy	wcsncpy
	#define _strcmp		wcscmp
	#define _tstrncmp	wcsncmp
	#define _tstrcat	wcscat

	#define _tsprintf	swprintf
	#define _tsnprintf  _snwprintf
	#define _tvsprintf	vswprintf

    #define _tuitostr   _ultow
    #define _titostr    _ltow
    #define _ti64tostr  _i64tow
    #define _tui64tostr _ui64tow
    #define _tftostr

    #define _tstrtoi    _wtoi
    #define _tstrtod    wcstod

#else

	typedef char		tchar;

	#define _text

	#define _tstrlen	strlen
	#define _tstrcpy	strcpy
	#define _tstrncpy	strncpy
	#define _tstrcmp	strcmp
	#define _tstrncmp	strncmp
	#define _tstrcat	strcat

	#define _tsprintf	sprintf
	#define _tsnprintf  _snprintf
	#define _tvsprintf	vsprintf


    #define _tuitostr   _ultoa
    #define _titostr    _ltoa
    #define _ti64tostr  _i64toa
    #define _tui64tostr _ui64toa
    #define _tftostr    _gcvt

    #define _tstrtoi    atoi
    #define _tstrtod    strtod

#endif
*/

}
