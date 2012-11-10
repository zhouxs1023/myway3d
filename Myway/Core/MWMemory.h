#ifndef DEV_MEMORY_H
#define DEV_MEMORY_H

/*
--------------------------------------------------------------------------
    Memory operator for Myway Engine. 
    
--------------------------------------------------------------------------
*/
#include <new.h>
#include "MWDebug.h"


#ifdef MW_DEBUG_MEMORY

#define _output_memory_status()         Myway::Memory::OutputMemoryStatus()
#define _output_memory_pool_status()    Myway::Memory::OutputMemoryPoolStatus()

#else

#define _output_memory_status()
#define _output_memory_pool_status()

#endif

namespace Myway
{

#define _alloc(size, align, file, line)          Myway::Memory::Alloc(size, align, file, line)
#define _free(p, file, line)                     Myway::Memory::Free(p, file, line)

#define _pool_alloc(size, align, file, line)     Myway::Memory::PoolAlloc(size, align, file, line)
#define _pool_free(p, file, line)                Myway::Memory::PoolFree(p, file, line)


#define alloc_mem(size)                         _alloc(size, MEMORY_ALIGN_NONE, __FILE__, __LINE__)
#define free_mem(p)                             _free(p, __FILE__, __LINE__)

#define alloc_m alloc_mem
#define free_m free_mem

#define pool_alloc(size)                        _pool_alloc(size, MEMORY_ALIGN_NONE, __FILE__, __LINE__)
#define pool_free(p)                            _pool_free(p, __FILE__, __LINE__)

#define alloc_mem_align(size, align)            _alloc(size, align, __FILE__, __LINE__)

#define mw_new                                  new(MEMORY_ALIGN_NONE, __FILE__, __LINE__)

#define _is_bad_mem(p)                          Myway::Memory::IsBadMemory(p)

#define safe_free(p)						    { if (p) { free_mem(p); (p) = 0;} }
#define safe_pool_free(p)					    { if (p) { pool_free(p); (p) = 0;} }
#define safe_delete(p)						    { if (p) { delete (p); (p) = 0; } }
#define safe_delete_array(p)				    { if (p) { delete[] (p); (p) = 0; } }

class MW_ENTRY Memory
{
public:
    static void Init(int _16BytePoolCount = 1000,
                     int _32BytePoolCount = 1000,
                     int _64BytePoolCount = 1000,
                     int _128BytePoolCount = 1000,
                     int _256BytePoolCount = 1000);
    static void DeInit();

    static void * Alloc(size_t size, int align, const char * file, int line);
    static void Free(void * p, const char * file, int line);

    static void * PoolAlloc(size_t size, int align, const char * file, int line);
    static void PoolFree(void * p, const char * file, int line);

    static bool IsBadMemory(void * pMem);

    static void OutputMemoryPoolStatus();
    static void OutputMemoryStatus();
};

void MW_ENTRY Strcat(char * dest, int destsize, const char * src1, const char * src2);
void MW_ENTRY Strcpy(char * dest, int destsize, const char * src);
void MW_ENTRY Strcpy(char * dest, int destsize, const char * src, int srcsize);
int MW_ENTRY Strcmp(const char * dest, const char * src);
int MW_ENTRY Strlen(const char * str);
void MW_ENTRY Memset8(void * mem, char val, int count);
void MW_ENTRY Memset16(void * mem, short val, int count);
void MW_ENTRY Memset32(void * mem, int val, int count);
bool MW_ENTRY Compare8(const void * mem, char val, int count);
bool MW_ENTRY Compare16(const void * mem, short val, int count);
bool MW_ENTRY Compare32(const void * mem, int val, int count);
bool MW_ENTRY CompareMemory(const void * mem1, const void * mem2, int size);
void MW_ENTRY Memzero(void * mem, int size);
void MW_ENTRY MemzeroSSE(void * mem, int size);
void MW_ENTRY MemzeroSSE2(void * mem, int size);
void MW_ENTRY Memcpy(void * dest, const void * src, int size);
void MW_ENTRY MemcpySSE(void * dest, const void * src, int size);
void MW_ENTRY MemcpySSE2(void * dest, const void * src, int size);


} // end of namespace Myway

// operator new, operator delete
static void * operator new(size_t size, int align, const char * file, int line);
static void operator delete(void * p, const char * file, int line);

static void * operator new[](size_t size, int align, const char * file, int line);
static void operator delete[](void * p, const char * file, int line);

#endif //end of file

//***************************************************************
//