/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "StandardHeaders.h"
#include "SourceDeclaration.h"
#include "Platform.h"


/// Compiler types.
#define COMPILER_MSVC			0x00
#define COMPILER_INTELC			0x01
#define COMPILER_CODEWARRIOR	0x02
#define COMPILER_GNUC			0x03
#define COMPILER_BORL			0x04


/// Finds the compiler type and version.
#if defined _MSC_VER
	#define COMPILER COMPILER_MSVC
	#define COMPILERVERSION _MSC_VER
#elif defined __MWERKS__
	#define COMPILER COMPILER_CODEWARRIOR
	#define COMPILERVERSION __MWERKS__
#elif defined __ICL
	#define COMPILER COMPILER_INTELC
	#define COMPILER_VERSION 0
#elif defined __GNUC__
	#define COMPILER COMPILER_GNUC
	#define COMPILERVERSION __VERSION__
#elif defined __BORLANDC__
	#define COMPILER COMPILER_BORL
	#define COMPILERVERSION __BCPLUSPLUS__
#else
	#pragma error "Unknown compiler used, cannot continue."
#endif


/// Disable conversion compile warning
#if COMPILER == COMPILER_MSVC
	#pragma warning (disable : 4244)	// conversion from 'double' to 'float', possible loss of data
	#pragma warning (disable : 4800)	// 'int' : forcing value to bool 'true' or 'false' (performance warning)
	#pragma warning (disable : 4786)	// identifier was truncated to '255' characters in the debug information
	#pragma warning (disable : 4100)	// unreferenced formal parameter
	#pragma warning (disable : 4324)	// structure was padded due to __declspec(align())
	#pragma warning (disable : 4127)	// conditional expression is constant
	#pragma warning (disable : 4238)	// problem with template type
/*	#pragma warning(disable : 4786) // identifier truncated to 255 characters
	#pragma warning(disable : 4244) // implicit conversion: possible loss of data
	#pragma warning(disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
	#pragma warning(disable : 4189) // local variable is initialized but not referenced
	#pragma warning(disable : 4100) // unreferenced formal parameter
	#pragma warning(disable : 4245) // conversion from 'const int' to 'unsigned long', signed/unsigned mismatch
	#pragma warning(disable : 4660) // template-class specialization 'basic_filebuf<char,class _STLD::char_traits<char> >' is already instantiated
	#pragma warning(disable : 4701) // local variable 'base' may be used without having been initialized
	#pragma warning(disable : 4075) // initializers put in unrecognized initialization area
	#pragma warning(disable : 4673) // throwing class with private base class
	#pragma warning(disable : 4670) // throwing class with private base class
	#pragma warning(disable : 4018) // signed/unsigned mismatch
	#pragma warning(disable : 4505) // unreferenced local function has been removed
	#pragma warning(disable : 4146) // unary minus applied to unsigned type
	#pragma warning(disable : 4244) // arithmetic conversion - possible loss of data
	#pragma warning(disable : 4290) // c++ exception specification ignored
	#pragma warning(disable : 4018) // signed/unsigned mismatch
	#pragma warning(disable : 4251) // DLL interface needed
	#pragma warning(disable : 4284) // for -> operator
	#pragma warning(disable : 4244) // conversion to float, possible loss of data							
	#pragma warning(disable : 4699) // creating precompiled header											
	#pragma warning(disable : 4200) // Zero-length array item at end of structure, a VC-specific extension	
	#pragma warning(disable : 4100) // unreferenced formal parameter										
	#pragma warning(disable : 4514) // unreferenced inline function has been removed						
	#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union					
	#pragma warning(disable : 4710) // inline function not expanded											
	#pragma warning(disable : 4714) // __forceinline function not expanded									  
	#pragma warning(disable : 4702) // unreachable code in inline expanded function							
	#pragma warning(disable : 4711) // function selected for autmatic inlining								
	#pragma warning(disable : 4725) // Pentium fdiv bug														
	#pragma warning(disable : 4127) // Conditional expression is constant									
	#pragma warning(disable : 4512) // assignment operator could not be generated                           
	#pragma warning(disable : 4530) // C++ exception handler used, but unwind semantics are not enabled     
	#pragma warning(disable : 4245) // conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch 
	#pragma warning(disable : 4238) // nonstandard extension used : class rvalue used as lvalue             
	#pragma warning(disable : 4251) // needs to have dll-interface to be used by clients of class 'ULinker' 
	#pragma warning(disable : 4275) // non dll-interface class used as base for dll-interface class         
	#pragma warning(disable : 4511) // copy constructor could not be generated                              
	#pragma warning(disable : 4284) // return type is not a UDT or reference to a UDT                       
	#pragma warning(disable : 4355) // this used in base initializer list                                   
	#pragma warning(disable : 4097) // typedef-name '' used as synonym for class-name ''                    
	#pragma warning(disable : 4291) // typedef-name '' used as synonym for class-name ''                    
	#pragma warning(disable : 4731) // frame pointer register 'ebp' modified by inline assembly code        
*/
#endif


// some special types that are missing inside Visual C++ 6
#if COMPILER == COMPILER_MSVC
	#if (COMPILERVERSION < 1300)
		typedef unsigned long uintPointer;
	#else
		typedef uintptr_t uintPointer;
	#endif
#endif


// a define to disable the ImageFile class (to eliminate OpenIL requirement)
#define NO_IMAGEFILE


// use doubles everywhere instead of floats
//#define HIGH_PRECISION

#ifdef HIGH_PRECISION
	typedef double MReal;
#else
	typedef float MReal;
#endif



/// Disable conversion compile warning
#if COMPILER == COMPILER_INTELC
	#pragma warning(disable : 873)
	#pragma warning(disable : 981)
	#pragma warning(disable : 522)
	#pragma warning(disable : 271)
	#pragma warning(disable : 424)
	#pragma warning(disable : 193)
	#pragma warning(disable : 444)
	#pragma warning(disable : 440)
	#pragma warning(disable : 171)
	#pragma warning(disable : 1125)
	#pragma warning(disable : 488)
	#pragma warning(disable : 858)
	#pragma warning(disable : 82)
	#pragma warning(disable : 1)
	#pragma warning(disable : 177)
	#pragma warning(disable : 279)
#endif


#if COMPILER == COMPILER_MSVC
	/// Import function from DLL.
	#define DLL_IMPORT	__declspec(dllimport)
	/// Export function to DLL.
	#define DLL_EXPORT  __declspec(dllexport)
	/// Functions with variable arguments.
	#define VARARGS __cdecl
	/// Standard C function.
	#undef CDECL
	#define CDECL __cdecl
	/// Standard calling convention.
	#define STDCALL __stdcall
	/// Force code to be inline.
	//#define FORCEINLINE __forceinline
	//#define f_inline __forceinline
#elif COMPILER == COMPILER_INTELC
	/// Force code to be inline.
	//#define f_inline inline
#elif COMPILER == COMPILER_GNUC
	/// Force code to be inline.
	//#define f_inline inline
#elif COMPILER == COMPILER_BORL
	/// Force code to be inline.
	//#define f_inline inline
#elif COMPILER == COMPILER_CODEWARRIOR
	//
#else
	#pragma error "No known compiler."
#endif


// concrete debug implementation.
#ifdef _DEBUG
	#define DEBUG
#endif


// if C++ exception handling is disabled, force guarding to be off.
#if COMPILER == COMPILER_MSVC
	#ifndef _CPPUNWIND
		#error "Bad VCC option: C++ exception handling must be enabled"
	#endif
#endif //WIN32


#ifdef WIN32
	//#define MAC_ONLY(a)
	//#define SWAP32(a)
	//#define SWAP16(a)
#else
/*
	//#define MAC_ONLY(a) a

	template<typename T> 
	inline void ::SWAP32(T &x)
	{
		unsigned int * swapped = (unsigned int *)&x;
		*swapped = (*swapped<<24 | ((*swapped<<8)&0x00FF0000) | ((*swapped>>8)&0xFF00) | ((*swapped>>24)&0xFF));
	}

	#define SWAP16(a) a = (a<<8) | (a>>8)&0xFF

	template<typename T> 
	inline const T &::max(const T &x, const T &y)
	{
		return x < y ? y : x;
	}

	#define BOOL unsigned char
*/
#endif


// define NULL, in case it hasn't been done yet
#ifndef NULL
	#define NULL 0
#endif


#endif