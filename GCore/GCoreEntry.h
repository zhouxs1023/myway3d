#pragma once

#ifdef GCORE_EXPORT

#define GCORE_ENTRY __declspec(dllexport)

#else

#define GCORE_ENTRY __declspec(dllimport)

#endif
