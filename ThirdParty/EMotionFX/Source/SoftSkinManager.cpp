/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include EMotion FX related headers
#include "SoftSkinManager.h"
#include "SoftSkinDeformer.h"

#if COMPILER == COMPILER_MSVC
	#include "SoftSkinDeformerSSE.h"
#endif


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{


// constructor
SoftSkinManager::SoftSkinManager()
{
	bool hasSSE = false;

#if COMPILER == COMPILER_MSVC
	__asm 
	{
		// Move the number 1 into eax - this will move the
		// feature bits into EDX when a CPUID is issued, that
		// is, EDX will then hold the key to the cpuid
		mov eax, 1

		// Does this processor have SSE support?
		cpuid

		// Perform CPUID (puts processor feature info in EDX)
		// Shift the bits in edx to the right by 26, thus bit 25
		// (SSE bit) is now in CF bit in EFLAGS register.
		shr edx,0x1A

		// If CF is not set, jump over next instruction
		jnc nocarryflag

		// set the return value to 1 if the CF flag is set
		mov [hasSSE], 1

		nocarryflag:
	}
#endif

	// set the value
	mDetectedSSE = hasSSE;

	MCore::LOG("SoftSkinManager : SSE detected = %s", mDetectedSSE ? "Yes" : "No");
}




SoftSkinDeformer* SoftSkinManager::CreateDeformer(Mesh* mesh)
{
	// TODO: fix the SSE deformer so that its fast again
/*	if (mDetectedSSE)
	{
		#if COMPILER == COMPILER_MSVC
			return new SoftSkinDeformerSSE(mesh);	
		#else
			return new SoftSkinDeformer(mesh);
		#endif	
	}
	else*/
		return new SoftSkinDeformer(mesh);
}



} // namespace EMotionFX