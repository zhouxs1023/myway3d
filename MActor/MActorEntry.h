//////////////////////////////////////////////////////////////////////////
//
// EMotionFX Integration.
//   EMotionFX engine : http://www.mysticgd.com
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#ifdef MACTOR_EXPORT

#define MACTOR_ENTRY __declspec(dllexport)

#else

#define MACTOR_ENTRY __declspec(dllimport)

#endif