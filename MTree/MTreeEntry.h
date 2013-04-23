//////////////////////////////////////////////////////////////////////////
//
// SpeedTreeRT Integration.
//   SpeedTreeRT engine: http://www.idvinc.com
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#ifdef MTREE_EXPORT

#define MTREE_ENTRY __declspec(dllexport)

#else

#define MTREE_ENTRY __declspec(dllimport)

#endif