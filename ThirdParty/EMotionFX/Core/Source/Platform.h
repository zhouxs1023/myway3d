/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __PLATFORM_H
#define __PLATFORM_H

// platform declarations
#define PLATFORM_WINDOWS	0x00
#define PLATFORM_LINUX		0x01
#define PLATFORM_MAC		0x02
#define PLATFORM_XBOX		0x03
#define PLATFORM_PS2		0x04
#define PLATFORM_GAMECUBE	0x05
#define PLATFORM_UNKNOWN	0xff


// autodetect the current platform (WINDOWS / MACOS / XBOX)
#ifdef _XBOX
	#define PLATFORM PLATFORM_XBOX
#else
	#ifdef MACOS
		#define PLATFORM PLATFORM_MAC
	#else
		#ifdef WIN32
			#define PLATFORM PLATFORM_WINDOWS
		#else
			#define PLATFORM PLATFORM_UNKNOWN
			#error "Unknown platform, not yet supported."
		#endif
	#endif
#endif


// override the platform (comment the autodetection part when using this to prevent compiler warnings)
//#define PLATFORM PLATFORM_XBOX

#endif