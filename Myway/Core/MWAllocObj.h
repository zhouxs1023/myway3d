#pragma once

#include "MWMemory.h"

namespace Myway
{

struct MW_ENTRY AllocObj
{
public:
    AllocObj();
    ~AllocObj();

    void * operator new(size_t size, int align, const char * file, int line);
    void * operator new[](size_t size, int align, const char * file, int line);
    void * operator new(size_t size);
    void * operator new[](size_t size);

    void operator delete(void * p, const char * file, int line);
    void operator delete[](void * p, const char * file, int line);

    void operator delete(void * p);
    void operator delete[](void * p);
};

struct PoolAllocObj
{
public:
    PoolAllocObj();
    ~PoolAllocObj();

    void * operator new(size_t size, int align, const char * file, int line);
    void * operator new[](size_t size, int align, const char * file, int line);
    void * operator new(size_t size);
    void * operator new[](size_t size);

    void operator delete(void * p, const char * file, int line);
    void operator delete[](void * p, const char * file, int line);

    void operator delete(void * p);
    void operator delete[](void * p);
};

#include "MWAllocObj.inl"



#define DECLARE_ALLOC() \
public: \
    void * operator new(size_t size, int align, const char * file, int line) \
    { \
        return _alloc(size, align,  file, line); \
    } \
      \
    void * operator new[](size_t size, int align, const char * file, int line) \
    { \
        return _alloc(size, align, file, line); \
    } \
      \
    void * operator new(size_t size) \
    { \
        return _alloc(size, MEMORY_ALIGN_NONE, "unknow", 0); \
    } \
      \
    void * operator new[](size_t size) \
    { \
        return _alloc(size, MEMORY_ALIGN_NONE, "unknow", 0); \
    } \
      \
    void operator delete(void * p, const char * file, int line) \
    { \
        _free(p, file, line); \
    } \
      \
    void operator delete[](void * p, const char * file, int line) \
    { \
        _free(p, file, line); \
    } \
      \
    void operator delete(void * p) \
    { \
        _free(p, "unknow", 0); \
    } \
      \
    void operator delete[](void * p) \
    { \
        _free(p, "unknow", 0); \
    }


#define DECLARE_POOL_ALLOC() \
public: \
   void * operator new(size_t size, int align, const char * file, int line) \
    { \
        return _pool_alloc(size, align,  file, line); \
    } \
      \
    void * operator new[](size_t size, int align, const char * file, int line) \
    { \
        return _pool_alloc(size, align, file, line); \
    } \
      \
    void * operator new(size_t size) \
    { \
        return _pool_alloc(size, MEMORY_ALIGN_NONE, "unknow", 0); \
    } \
      \
    void * operator new[](size_t size) \
    { \
        return _pool_alloc(size, MEMORY_ALIGN_NONE, "unknow", 0); \
    } \
      \
    void operator delete(void * p, const char * file, int line) \
    { \
        _pool_free(p, file, line); \
    } \
      \
    void operator delete[](void * p, const char * file, int line) \
    { \
        _pool_free(p, file, line); \
    } \
      \
    void operator delete(void * p) \
    { \
        _pool_free(p, "unknow", 0); \
    } \
      \
    void operator delete[](void * p) \
    { \
        _pool_free(p, "unknow", 0); \
    }

}