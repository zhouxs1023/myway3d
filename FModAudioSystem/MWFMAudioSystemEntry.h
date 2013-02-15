#pragma once

#ifdef FM_EXPORT

#define FM_ENTRY __declspec(dllexport)

#else

#define FM_ENTRY __declspec(dllimport)

#endif

#include <fmod.h>