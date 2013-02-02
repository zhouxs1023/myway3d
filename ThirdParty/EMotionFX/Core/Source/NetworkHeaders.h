/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Macros.h"
#include "SourceDeclaration.h"
#include "NetworkSystem.h"

#if PLATFORM == PLATFORM_WINDOWS
	#define HAS_NETWORK_IMPLEMENTATION 1
	#include "WinSocket.h"
	#include "WinNetworkAddress.h"
	#include "WinNetwork.h"
	#pragma comment(lib, "WS2_32")	// automatically link winsock
#else
	//#error "Missing socket implementation for this platform!"
	#define HAS_NETWORK_IMPLEMENTATION 0
#endif