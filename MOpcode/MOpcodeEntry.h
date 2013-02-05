#pragma once

#ifdef MOPCODE_EXPORT

#define MOPCODE_ENTRY __declspec(dllexport)

#else

#define MOPCODE_ENTRY __declspec(dllimport)

#endif