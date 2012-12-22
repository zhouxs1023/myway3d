#pragma once

#ifdef MTREE_EXPORT

#define MTREE_ENTRY __declspec(dllexport)

#else

#define MTREE_ENTRY __declspec(dllimport)

#endif