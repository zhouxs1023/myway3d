#pragma once

#ifdef MRENDER_EXPORT

#define MRENDER_ENTRY __declspec(dllexport)

#else

#define MRENDER_ENTRY __declspec(dllimport)

#endif