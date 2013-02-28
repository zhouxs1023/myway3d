//////////////////////////////////////////////////////////////////////////
//
// Newton Integration.
//   newton physics engine: http://newtondynamics.com/forum/newton.php
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#ifdef MNEWTON_EXPORT

#define MNEWTON_ENTRY __declspec(dllexport)

#else

#define MNEWTON_ENTRY __declspec(dllimport)

#endif

#include "Newton.h"
#include "JointLibrary.h"
#include "Myway.h"