#pragma once

#ifdef MNEWTON_EXPORT

#define MNEWTON_ENTRY __declspec(dllexport)

#else

#define MNEWTON_ENTRY __declspec(dllimport)

#endif