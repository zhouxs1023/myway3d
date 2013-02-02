#pragma once

#ifdef MACTOR_EXPORT

#define MACTOR_ENTRY __declspec(dllexport)

#else

#define MACTOR_ENTRY __declspec(dllimport)

#endif