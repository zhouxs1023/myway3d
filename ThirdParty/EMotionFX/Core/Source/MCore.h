/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MCORE_H
#define __MCORE_H

// automatically link the Core lib?
//#define AUTOLINK_CORE

// when automatically linking is enabled, always link the release version?
//#define CORE_ALWAYS_LINK_RELEASE

// config
#include "Config.h"
#include "Macros.h"
#include "Platform.h"
#include "SourceDeclaration.h"
#include "StandardHeaders.h"

// file system
#include "DiskFile.h"
#include "File.h"
#include "FileSystem.h"
#include "MemoryFile.h"
#include "Stream.h"

// template datastructures
#include "Array.h"
#include "Stack.h"
//#include "Iterator.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "HashFunctions.h"

// smart pointer
#include "SmartPointer.h"

// template patterns
#include "Observer.h"
#include "Pair.h"
#include "Singleton.h"

// math
#include "Color.h"
#include "Frustum.h"
#include "FastMath.h"
#include "Matrix4.h"
#include "NMatrix.h"
#include "PlaneEq.h"
#include "Quaternion.h"
#include "Random.h"
#include "Ray.h"
#include "Vector.h"
#include "OBB.h"
#include "AABB.h"
#include "BoundingSphere.h"
#include "Ray.h"
#include "CompressedFloat.h"
#include "CompressedVector.h"
#include "CompressedQuaternion.h"
#include "RootSolver.h"
#include "SwingAndTwist.h"
//#include "Spectrum.h"

// image system
//#include "Image.h"
//#include "ImageCodecs.h"
//#include "ImageCodecRegistry.h"

// native win32
#include "Timer.h"
#include "Directory.h"

// base
#include "Algorithms.h"
#include "Exception.h"
#include "Serializable.h"
#include "String.h"

// log system
#include "LogFile.h"
#include "LogManager.h"

// network
#include "NetworkHeaders.h"		// this auto-links the WinSock library from windows, if Windows is the current platform (and not XBOX)
#include "POP3Client.h"

// memory
#include "MemoryManager.h"


// autolink library
#ifdef AUTOLINK_CORE
	#if COMPILER == COMPILER_MSVC
		#ifdef CORE_ALWAYS_LINK_RELEASE
			// in case we are dealing with Visual C++ 6
			#if (COMPILERVERSION < 1300)
				#pragma comment(lib, "CoreReleaseVC6")
			#endif

			// in case we are dealing with Visual Studio .NET
			#if (COMPILERVERSION == 1300)
				#pragma comment(lib, "CoreReleaseVSNET")
			#endif

			// in case we are dealing with Visual Studio .NET 2003
			#if (COMPILERVERSION >= 1310)
				#pragma comment(lib, "CoreReleaseVSNET2003")
			#endif
		#else
			#ifdef _DEBUG
				// in case we are dealing with Visual C++ 6
				#if (COMPILERVERSION < 1300)
					#pragma comment(lib, "CoreDebugVC6")
				#endif

				// in case we are dealing with Visual Studio .NET
				#if (COMPILERVERSION == 1300)
					#pragma comment(lib, "CoreDebugVSNET")
				#endif

				// in case we are dealing with Visual Studio .NET 2003
				#if (COMPILERVERSION >= 1310)
					#pragma comment(lib, "CoreDebugVSNET2003")
				#endif
			#else	// we are in RELEASE mode
				// in case we are dealing with Visual C++ 6
				#if (COMPILERVERSION < 1300)
					#pragma comment(lib, "CoreReleaseVC6")
				#endif

				// in case we are dealing with Visual Studio .NET
				#if (COMPILERVERSION == 1300)
					#pragma comment(lib, "CoreReleaseVSNET")
				#endif

				// in case we are dealing with Visual Studio .NET 2003
				#if (COMPILERVERSION >= 1310)
					#pragma comment(lib, "CoreReleaseVSNET2003")
				#endif
			#endif
		#endif
	#elif COMPILER == COMPILER_CODEWARRIOR
		#ifdef CORE_ALWAYS_LINK_RELEASE
			#pragma comment(lib, "CoreCWRelease")
		#else
			#ifdef _DEBUG // we are in DEBUG mode
				#pragma comment(lib, "CoreCWDebug")
			#else // we are in RELEASE mode
				#pragma comment(lib, "CoreCWRelease")
			#endif
		#endif
	#else
		#error "Your compiler is currently not supported."
	#endif
#endif	// AUTOLINK_CORE

#endif	// __CORE_H