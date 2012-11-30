#pragma once

#ifdef X_EXPORT

#define X_ENTRY __declspec(dllexport)

#else

#define X_ENTRY __declspec(dllimport)

#endif