/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __STANDARTHEADERS_H
#define __STANDARTHEADERS_H

#include "Platform.h"

// standard includes
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
//#include <memory.h>
#include <string.h>
#include <stdlib.h>
#ifdef WIN32
#include <malloc.h>
#endif

/*
// Basic includes.
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <limits>

// C++ Stream stuff.
#include <fstream>
#include <iostream>
#include <iomanip>
#include <strstream>
*/


#if PLATFORM == PLATFORM_WINDOWS
	#include <windows.h>
#endif

#if PLATFORM == PLATFORM_LINUX
	#include <unistd.h>
	#include <dlfcn.h>
#endif


#endif