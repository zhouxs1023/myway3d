//////////////////////////////////////////////////////////////////////////
//
// MyGui Integration.
//   MyGui engine: http://mygui.info
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#ifdef MGUI_EXPORT
	#define MGUI_ENTRY __declspec(dllexport)
#else
	#define MGUI_ENTRY __declspec(dllimport)
#endif

#include "Myway.h"

#include "MyGUI.h"

#define MYGUI_PLATFORM_LOG_SECTION "Platform"
#define MYGUI_PLATFORM_LOG(level, text) MYGUI_LOGGING(MYGUI_PLATFORM_LOG_SECTION, level, text)

