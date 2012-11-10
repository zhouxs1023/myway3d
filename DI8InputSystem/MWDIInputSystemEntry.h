#pragma once

#ifdef DI_EXPORT

#define DI_ENTRY __declspec(dllexport)

#else

#define DI_ENTRY __declspec(dllimport)

#endif

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>