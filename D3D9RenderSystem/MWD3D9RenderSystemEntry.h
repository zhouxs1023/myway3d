#pragma once

#ifdef RS_EXPORT

#define RS_ENTRY _declspec(dllexport)

#else

#define RS_ENTRY _declspec(dllimport)

#endif