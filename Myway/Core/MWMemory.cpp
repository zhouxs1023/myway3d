#include "MWMutex.h"
#include "MWMemory.h"
#include "MWMemoryPool.h"

#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

using namespace Myway;

/*  Memory Allocate Data Format
-----------------------------------------------------------------------------------
    
    | align data | block info | block head | head | used data | end | 

    align data: 0 - 32 bytes
    block info: 8 bytes, 4 bytes actual size and 4 bytes request size 
    block head: 4 bytes,  2 bytes point offset and 2 bytes pool flag
    used data:  actual used data.
    head:       16 bytes, must be zero
    end:        16 bytes, must be zero

-----------------------------------------------------------------------------------
*/

#define BLOCK_BEG_DATA 16
#define BLOCK_END_DATA 16

#define ALLOC_FILE_SIZE 128

#define BLOCK_DATA     0

struct MemoryBlockHead
{
	short PointOffset;
    short PoolFlag;
};

struct MemoryBlockBeg
{
    char data[BLOCK_BEG_DATA];
};

struct MemoryBlockEnd
{
    char data[BLOCK_END_DATA];
};

struct MemoryBlockInfo
{
	int	ActualSize;
	int RequestSize;
    //char    AllocFile[ALLOC_FILE_SIZE];
    //int     AllocLine;
};

#ifdef MW_DEBUG_MEMORY

#define MemoryBlockBegPointer(p)            (MemoryBlockBeg*)(p - sizeof(MemoryBlockBeg))
#define MemoryBlockEndPointer(p, size)      (MemoryBlockEnd*)(p + size)
#define MemoryBlockHeadPointer(p)	        (MemoryBlockHead*)(p - sizeof(MemoryBlockBeg) - sizeof(MemoryBlockHead))
#define MemoryBlockInfoPointer(p)	        (MemoryBlockInfo*)(p - sizeof(MemoryBlockBeg) - sizeof(MemoryBlockHead) - sizeof(MemoryBlockInfo))

#else

#define MemoryBlockHeadPointer(p)	        (MemoryBlockHead*)(p - sizeof(MemoryBlockHead))

#endif

#ifdef MW_MULTI_THREAD

Mutex memMutex;

#define __lock_m()      memMutex.Lock()
#define __unlock_m()    memMutex.Unlock()

#else

#define __lock_m()
#define __unlock_m()

#endif

MemoryPool _mmpool;


int AllocCount			= 0;
int ReleaseCount			= 0;
int TotalRequested   	= 0;
int TotalUsed			= 0;
int TotalNeed			= 0;
int MaximumRequested		= 0;
int MaximumUsed			= 0;
int MaximumNeed			= 0;

void Memory::Init(int _16BytePoolCount,
                  int _32BytePoolCount,
                  int _64BytePoolCount,
                  int _128BytePoolCount,
                  int _256BytePoolCount)
{
    _mmpool.Initialize(_16BytePoolCount,
                      _32BytePoolCount,
                      _64BytePoolCount,
                      _128BytePoolCount,
                      _256BytePoolCount);
}

void Memory::DeInit()
{
    _mmpool.Shutdown();
}

void * Memory::Alloc(size_t iSize, int Align, const char * sFileName, int iLine)
{
    d_assert (iSize > 0);

    __lock_m();

	int				    iActualSize = 0;
    short               iMemoryPool = MEMORY_IN_SYSTEM;
	char *			    pActualMem = NULL;
	char *			    pUsedMem = NULL;
	MemoryBlockHead *   pHead = NULL;

	d_assert(iSize && "Invalidate Parameters");

#ifdef MW_DEBUG_MEMORY

    iActualSize = Align + sizeof(MemoryBlockHead) + sizeof(MemoryBlockInfo) + 
                  sizeof(MemoryBlockBeg) + iSize + sizeof(MemoryBlockEnd);

#else

    iActualSize = Align + sizeof(MemoryBlockHead) + iSize;

#endif
    
    iMemoryPool = MEMORY_IN_SYSTEM;
    pActualMem = (char*)::malloc(iActualSize);
    
	debug_assert(pActualMem, "Allocating Error");

#ifdef MW_DEBUG_MEMORY

    Memzero(pActualMem, iActualSize);

	pUsedMem = pActualMem + Align + sizeof(MemoryBlockInfo) + sizeof(MemoryBlockHead) + sizeof(MemoryBlockBeg);

#else

    pUsedMem = pActualMem + Align + sizeof(MemoryBlockHead);

#endif

	if (Align)
		pUsedMem = (char*)((int)(pUsedMem) & ~(Align - 1));

	pHead = MemoryBlockHeadPointer(pUsedMem);
	pHead->PointOffset = static_cast<short>((char*)pHead - (char*)pActualMem);
    pHead->PoolFlag = iMemoryPool;

#ifdef MW_DEBUG_MEMORY

    MemoryBlockInfo * pInfo = MemoryBlockInfoPointer(pUsedMem);
    MemoryBlockBeg * beg = MemoryBlockBegPointer(pUsedMem);
    MemoryBlockEnd * end = MemoryBlockEndPointer(pUsedMem, iSize);

    Memset8(beg, BLOCK_DATA, sizeof(MemoryBlockBeg));
    Memset8(end, BLOCK_DATA, sizeof(MemoryBlockEnd));
	
	pInfo->ActualSize = iActualSize;
	pInfo->RequestSize = iSize;

	++AllocCount;
	TotalRequested += iActualSize;
	TotalUsed += iSize;
	TotalNeed += iSize + Align + sizeof(MemoryBlockHead);
	MaximumRequested = (TotalRequested > MaximumRequested) ? TotalRequested : MaximumRequested;
	MaximumUsed = (TotalUsed > MaximumUsed) ? TotalUsed : MaximumUsed;
	MaximumNeed = (TotalNeed > MaximumNeed) ? TotalNeed : MaximumNeed;

#endif

    debug_assert(!IsBadMemory(pUsedMem), "Bad Memory Allocated.");

    __unlock_m();

	return pUsedMem;
}

void Memory::Free(void * pMem, const char * sFileName, int iLine)
{
    __lock_m();

    debug_assert(!IsBadMemory(pMem), "Bad Memory Used.");

    char *				pActualMem;
	MemoryBlockHead *	pHead;

	pHead = MemoryBlockHeadPointer((char*)pMem);
	pActualMem = (char*)pHead - pHead->PointOffset;

#ifdef MW_DEBUG_MEMORY

	MemoryBlockInfo * pInfo = MemoryBlockInfoPointer((char*)pMem);
	ReleaseCount++;
	TotalRequested -= pInfo->ActualSize;
	TotalUsed -= pInfo->RequestSize;
	TotalNeed -= pInfo->ActualSize - sizeof(MemoryBlockInfo) - sizeof(MemoryBlockBeg) - sizeof(MemoryBlockEnd);

#endif
    assert (pHead->PoolFlag == MEMORY_IN_SYSTEM)

    ::free(pActualMem);

     __unlock_m();
}

void * Memory::PoolAlloc(size_t iSize, int Align, const char * sFileName, int iLine)
{
    d_assert (iSize > 0);

    __lock_m();

    int				    iActualSize = 0;
    short               iMemoryPool = MEMORY_IN_SYSTEM;
    char *				pActualMem = NULL;
    char *				pUsedMem = NULL;
    MemoryBlockHead *	pHead = NULL;

    debug_assert(iSize, "Invalidate Parameters");

#ifdef MW_DEBUG_MEMORY

    iActualSize = Align + sizeof(MemoryBlockHead) + sizeof(MemoryBlockInfo) + 
                  sizeof(MemoryBlockBeg) + iSize + sizeof(MemoryBlockEnd);

#else

    iActualSize = Align + sizeof(MemoryBlockHead) + iSize;

#endif

    pActualMem = (char*)_mmpool.Alloc(iActualSize, iMemoryPool);

    if (!pActualMem)
    {
        iMemoryPool = MEMORY_IN_SYSTEM;
        pActualMem = (char*)::malloc(iActualSize);
    }

    debug_assert(pActualMem, "Allocating Error");

#ifdef MW_DEBUG_MEMORY

    Memzero(pActualMem, iActualSize);

    pUsedMem = pActualMem + Align + sizeof(MemoryBlockInfo) + sizeof(MemoryBlockHead) + sizeof(MemoryBlockBeg);

#else

    pUsedMem = pActualMem + Align + sizeof(MemoryBlockHead);

#endif

    if (Align)
        pUsedMem = (char*)((int)(pUsedMem) & ~(Align - 1));

    pHead = MemoryBlockHeadPointer(pUsedMem);
    pHead->PointOffset = static_cast<short>((char*)pHead - (char*)pActualMem);
    pHead->PoolFlag = iMemoryPool;

#ifdef MW_DEBUG_MEMORY

    MemoryBlockInfo * pInfo = MemoryBlockInfoPointer(pUsedMem);
    MemoryBlockBeg * beg = MemoryBlockBegPointer(pUsedMem);
    MemoryBlockEnd * end = MemoryBlockEndPointer(pUsedMem, iSize);

    Memset8(beg, BLOCK_DATA, sizeof(MemoryBlockBeg));
    Memset8(end, BLOCK_DATA, sizeof(MemoryBlockEnd));

    pInfo->ActualSize = iActualSize;
    pInfo->RequestSize = iSize;

    ++AllocCount;
    TotalRequested += iActualSize;
    TotalUsed += iSize;
    TotalNeed += iSize + Align + sizeof(MemoryBlockHead);
    MaximumRequested = (TotalRequested > MaximumRequested) ? TotalRequested : MaximumRequested;
    MaximumUsed = (TotalUsed > MaximumUsed) ? TotalUsed : MaximumUsed;
    MaximumNeed = (TotalNeed > MaximumNeed) ? TotalNeed : MaximumNeed;

#endif

    debug_assert(!_is_bad_mem(pUsedMem), "Bad Memory Allocated.");

    __unlock_m();

    return pUsedMem;
}

void Memory::PoolFree(void * pMem, const char * sFileName, int iLine)
{
    __lock_m();

    debug_assert(!_is_bad_mem(pMem), "Bad Memory Used.");

    char *				pActualMem;
    MemoryBlockHead *	pHead;

    pHead = MemoryBlockHeadPointer((char*)pMem);
    pActualMem = (char*)pHead - pHead->PointOffset;

#ifdef MW_DEBUG_MEMORY

    MemoryBlockInfo * pInfo = MemoryBlockInfoPointer((char*)pMem);
    ReleaseCount++;
    TotalRequested -= pInfo->ActualSize;
    TotalUsed -= pInfo->RequestSize;
    TotalNeed -= pInfo->ActualSize - sizeof(MemoryBlockInfo) - sizeof(MemoryBlockBeg) - sizeof(MemoryBlockEnd);

    if (ReleaseCount == AllocCount)
        debug_printf("Memory OK.\n");

#endif

    if (pHead->PoolFlag != MEMORY_IN_SYSTEM)
        _mmpool.Free(pActualMem, pHead->PoolFlag);
    else
        ::free(pActualMem);

    __unlock_m();
}

bool Memory::IsBadMemory(void * pMem)
{
    MemoryBlockHead * pHead = MemoryBlockHeadPointer((char*)pMem);

    // check head
    if (pHead->PointOffset < 0 || pHead->PointOffset >= 32 ||
        !(pHead->PoolFlag == MEMORY_IN_SYSTEM || 
          pHead->PoolFlag == MEMORY_IN_POOL16 ||
          pHead->PoolFlag == MEMORY_IN_POOL32 ||
          pHead->PoolFlag == MEMORY_IN_POOL64 ||
          pHead->PoolFlag == MEMORY_IN_POOL128 ||
          pHead->PoolFlag == MEMORY_IN_POOL256))
        return TRUE;

#ifdef MW_DEBUG_MEMORY

    //chek beg data
    MemoryBlockInfo * pInfo = MemoryBlockInfoPointer((char*)pMem);
    MemoryBlockBeg * beg = MemoryBlockBegPointer((char*)pMem);
    MemoryBlockEnd * end = MemoryBlockEndPointer((char*)pMem, pInfo->RequestSize);

    for (int i = 0; i < BLOCK_BEG_DATA; ++i)
    {
        if (beg->data[i] != BLOCK_DATA)
            return TRUE;
    }

    for (int i = 0; i < BLOCK_END_DATA; ++i)
    {
        if (end->data[i] != BLOCK_DATA)
            return TRUE;
    }

#endif

   return FALSE;
}

void Memory::OutputMemoryPoolStatus()
{
    debug_printf("\n****** Memory Pool Status ******\n" \
                 "  16 bytes pool:            \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  32 bytes pool:            \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  64 bytes pool:            \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  128 bytes pool:           \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n" \
                 "                            \n" \
                 "  256 bytes pool:           \n" \
                 "          total:          %d\n" \
                 "          max requset:    %d\n" \
                 "          active:         %d\n" \
                 "          free:           %d\n\n",

                 _mmpool.mAlloc16.GetTotalCount(),
                 _mmpool.mAlloc16.GetMaxRequest(),
                 _mmpool.mAlloc16.GetActiveCount(),
                 _mmpool.mAlloc16.GetFreeCount(),

                 _mmpool.mAlloc32.GetTotalCount(),
                 _mmpool.mAlloc32.GetMaxRequest(),
                 _mmpool.mAlloc32.GetActiveCount(),
                 _mmpool.mAlloc32.GetFreeCount(),

                 _mmpool.mAlloc64.GetTotalCount(),
                 _mmpool.mAlloc64.GetMaxRequest(),
                 _mmpool.mAlloc64.GetActiveCount(),
                 _mmpool.mAlloc64.GetFreeCount(),

                 _mmpool.mAlloc128.GetTotalCount(),
                 _mmpool.mAlloc128.GetMaxRequest(),
                 _mmpool.mAlloc128.GetActiveCount(),
                 _mmpool.mAlloc128.GetFreeCount(),

                 _mmpool.mAlloc256.GetTotalCount(),
                 _mmpool.mAlloc256.GetMaxRequest(),
                 _mmpool.mAlloc256.GetActiveCount(),
                 _mmpool.mAlloc256.GetFreeCount());
}

void Memory::OutputMemoryStatus()
{
	debug_printf("\n---------- Memory Use Status --------------\n"   \
		         "          Allocation Count:      %d\n"     \
				 "          Deallocation Count:    %d\n"     \
				 "          Current Request Size:  %d\n"     \
				 "          Current Used Size:     %d\n"     \
				 "          Current Need Size:     %d\n"     \
				 "          Maximum Request Size:  %d\n"     \
				 "          Maximum Used Size:     %d\n"     \
				 "          Maximum Need Size:     %d\n\n",
				 AllocCount,
                 ReleaseCount,
				 TotalRequested,
                 TotalUsed,
                 TotalNeed,
				 MaximumRequested,
                 MaximumUsed,
                 MaximumNeed);
}

void Myway::Strcat(char * dest, int destsize, const char * src1, const char * src2)
{
    int len1 = 0, len2 = 0;
    int max_len = destsize - 1;

    while (len1 < max_len && src1[len1])
    {
        dest[len1] = src1[len1];
        ++len1;
    }

    d_assert(src1[len1] == 0 && "dest size overflow");

    while (len1 < max_len && src2[len2])
    {
        dest[len1++] = src2[len2++];
    }

    d_assert(src2[len2] == 0 && "dest size overflow");

    dest[len1] = 0;
}

void Myway::Strcpy(char * dest, int destsize, const char * src)
{
    int len = 0;
    int max_len = destsize - 1;

    while (len < max_len && src[len])
    {
        dest[len] = src[len];
        ++len;
    }

    d_assert(src[len] == 0 && "dest size overflow");

    dest[len] = 0;
}

void Myway::Strcpy(char * dest, int destsize, const char * src, int srcsize)
{
    assert (destsize >= srcsize);

#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, dest;
        mov     esi, src;
        mov     eax, srcsize;

        mov     ecx, eax;
        shr     ecx, 2;
        rep     movsd;

        mov     ecx, eax;
        and     ecx, 3;
        rep     movsb;
    }

#else

    memcpy(dest, src, srcsize);

#endif 
}

int Myway::Strcmp(const char * dest, const char * src)
{
    const char * left = dest;
    const char * right = src;
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret;
}

int Myway::Strlen(const char * str)
{
    int len = 0;

    while (str[len])
    {
        ++len;
    }

    return len;
}

void Myway::Memset8(void * mem, char val, int count)
{
#ifdef MW_PLATFORM_WIN32

    __asm 
    {
        mov     edi, mem;

        mov     al, val;
        mov     ah, val;
        shl     eax, 16;
        mov     al, val;
        mov     ah, val;

        mov     ecx, count;
        shr     ecx, 2;
        rep     stosd;

        mov     ecx, count;
        and     ecx, 3;
        rep     stosb;
    }

#else

    memset(mem, val, count);

#endif 
}

void Myway::Memset16(void * mem, short val, int count)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, mem;
        mov     ax, val;
        shl     eax, 16;
        mov     ax, val;
             
        mov     ecx, count;
        shr     ecx, 1;
        rep     stosd;

        mov     ecx, count;
        and     ecx, 1;
        rep     stosw;
    }

#else

    memset(mem, val, count * 2);

#endif 
}

void Myway::Memset32(void * mem, int val, int count)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, mem;
        mov     eax, val;
        mov     ecx, count;
        rep     stosd;
    }

#else

    memset(mem, val, count * 4);

#endif
}

bool Myway::Compare8(const void * mem, char val, int count)
{
    assert (count > 0);

#ifdef MW_PLATFORM_WIN32

    __asm 
    {
        mov     edi, mem;

        mov     al, val;
        mov     ah, val;
        shl     eax, 16;
        mov     al, val;
        mov     ah, val;

        mov     ecx, count;

        shr     ecx, 2;
        repne   scasd;
        jne      __ne;
        
        mov     ecx, count;
        and     ecx, 3;
        repne   scasb;
        jg      __ne;

        mov     eax, 1;
        jmp     __done;

__ne:
        mov     eax, 0;

__done:

    }

#else

    int v = (val << 24) | (val << 16) | (val << 8) | val;
    int c = count >> 2;
    const int * m = (const int *)mem;

    while (c && *m++ == v)
        --c;

    if (c)
        return FALSE;

    c = count & 3;
    const int8 * n = (const int8 *)m;

    c = count & 3;

    while (c && *n++ == val)
        --c;

    return c ? FALSE : TRUE;

#endif
}

bool Myway::Compare16(const void * mem, short val, int count)
{
    assert (count > 0);

#ifdef MW_PLATFORM_WIN32

    __asm 
    {
        mov     edi, mem;

        mov     ax, val;
        shl     eax, 16;
        mov     ax, val;

        mov     ecx, count;

        shr     ecx, 1;
        repne   scasd;
        jne      __ne;

        mov     ecx, count;
        and     ecx, 1;
        repne   scasw;
        jne      __ne;

        mov     eax, 1;
        jmp     __done;

__ne:
        mov     eax, 0;

__done:

    }

#else

    int v = (val << 16) | val;
    int c = count >> 1;
    const int * m = (const int *)mem;

    while (c-- && *m++ == v) ;

    if (c)
        return FALSE;

    c = count & 1;

    const int16 * n = (const int16 *)m;

    if (c && *n++ == val)
    {
        --c;
    }

    return c ? FALSE : TRUE;

#endif
}

bool Myway::Compare32(const void * mem, int val, int count)
{
#ifdef MW_PLATFORM_WIN32

    __asm 
    {
        mov     edi, mem;
        mov     eax, val;
        mov     ecx, count;
        repne   scasd;
        jne     __ne;

        mov     eax, 1;
        jmp     __done;

__ne:
        mov     eax, 0;

__done:

    }

#else

    const int * m = (const int *)mem;

    while (count && *m++ == val)
        --count;

    return count ? FALSE : TRUE;

#endif
}

bool Myway::CompareMemory(const void * mem1, const void * mem2, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm 
    {
        mov     edi, mem1;
        mov     esi, mem2;
        mov     ecx, bytes;

        shr     ecx, 2;
        repne   cmpsd;
        jne     __ne;

        mov     ecx, bytes;
        and     ecx, 3;
        repne   cmpsb;
        jne     __ne;

        mov     eax, 1;
        jmp     __done;

__ne:
        mov     eax, 0;

__done:

    }

#else

    const int * m = (const int *)mem1;
    const int * n = (const int *)mem2;
    int c = bytes >> 2;

    while (c && *m++ == *n++)
        --c;

    if (c)
        return FALSE;

    c = bytes & 3;

    const int8 * i = (const int *)m;
    const int8 * j = (const int *)n;

    while (c && *i++ == *j++)
        --c;

    return count ? FALSE : TRUE;

#endif

}


void Myway::Memzero(void * mem, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, mem;
        mov     edx, bytes;
        mov     eax, 0;

        mov     ecx, edx;
        shr     ecx, 2;
        rep     stosd;

        mov     ecx, edx;
        and     ecx, 3;
        rep     stosb;
    }

#else

    memset(mem, 0, bytes);

#endif
}

void Myway::MemzeroSSE(void * mem, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, mem;
        mov     ecx, bytes;
        xorps   xmm0, xmm0;
        mov     eax, 0;
        cmp     ecx, 256;
        jb      __L_8;

__L_16:
        movntps [edi], xmm0;
        movntps 16[edi], xmm0;
        movntps 32[edi], xmm0;
        movntps 48[edi], xmm0;
        movntps 64[edi], xmm0;
        movntps 80[edi], xmm0;
        movntps 96[edi], xmm0;
        movntps 112[edi], xmm0;
        movntps 128[edi], xmm0;
        movntps 144[edi], xmm0;
        movntps 160[edi], xmm0;
        movntps 176[edi], xmm0;
        movntps 192[edi], xmm0;
        movntps 208[edi], xmm0;
        movntps 224[edi], xmm0;
        movntps 240[edi], xmm0;
        sub     ecx, 256;
        add     edi, 256;
        cmp     ecx, 256;
        jae     __L_16;

__L_8:
        cmp     ecx, 128;
        jb      __L_4;
        movntps [edi], xmm0;
        movntps 16[edi], xmm0;
        movntps 32[edi], xmm0;
        movntps 48[edi], xmm0;
        movntps 64[edi], xmm0;
        movntps 80[edi], xmm0;
        movntps 96[edi], xmm0;
        movntps 112[edi], xmm0;
        sub     ecx, 128;
        add     edi, 128;

__L_4:
        cmp     ecx, 64;
        jb      __L_2;
        movntps [edi], xmm0;
        movntps 16[edi], xmm0;
        movntps 32[edi], xmm0;
        movntps 48[edi], xmm0;
        sub     ecx, 64;
        add     edi, 64;


__L_2:
        cmp     ecx, 32;
        jb      __L_1;
        movntps [edi], xmm0;
        movntps 16[edi], xmm0;
        sub     ecx, 32;
        add     edi, 32;

__L_1:
        cmp     ecx, 16;
        jb      __dword;
        movntps [edi], xmm0;
        sub     ecx, 16;
        add     edi, 16;

__dword:
        sfence;
        cmp    ecx, 4;
        jb     __word;
        stosd;
        cmp    ecx, 4;
        jb     __word;
        stosd;
        cmp    ecx, 4;
        jb     __word;
        stosd;

__word:
        cmp    ecx, 2;
        jb     __byte;
        sub    ecx, 2;
        stosw;
__byte:
        cmp    ecx, 1;
        jb     __done;
        stosb;
__done:
    }

#else

    memset(mem, 0, bytes);

#endif
}

void Myway::MemzeroSSE2(void * mem, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, mem;
        mov     ecx, bytes;
        xorps   xmm0, xmm0;
        mov     eax, 0;
        cmp     ecx, 256;
        jb      __L_8;

__L_16:
        movntdq [edi], xmm0;
        movntdq 16[edi], xmm0;
        movntdq 32[edi], xmm0;
        movntdq 48[edi], xmm0;
        movntdq 64[edi], xmm0;
        movntdq 80[edi], xmm0;
        movntdq 96[edi], xmm0;
        movntdq 112[edi], xmm0;
        movntdq 128[edi], xmm0;
        movntdq 144[edi], xmm0;
        movntdq 160[edi], xmm0;
        movntdq 176[edi], xmm0;
        movntdq 192[edi], xmm0;
        movntdq 208[edi], xmm0;
        movntdq 224[edi], xmm0;
        movntdq 240[edi], xmm0;
        sub     ecx, 256;
        add     edi, 256;
        cmp     ecx, 256;
        jae     __L_16;

__L_8:
        cmp     ecx, 128;
        jb      __L_4;
        movntdq [edi], xmm0;
        movntdq 16[edi], xmm0;
        movntdq 32[edi], xmm0;
        movntdq 48[edi], xmm0;
        movntdq 64[edi], xmm0;
        movntdq 80[edi], xmm0;
        movntdq 96[edi], xmm0;
        movntdq 112[edi], xmm0;
        sub     ecx, 128;
        add     edi, 128;

__L_4:
        cmp     ecx, 64;
        jb      __L_2;
        movntdq [edi], xmm0;
        movntdq 16[edi], xmm0;
        movntdq 32[edi], xmm0;
        movntdq 48[edi], xmm0;
        sub     ecx, 64;
        add     edi, 64;


__L_2:
        cmp     ecx, 32;
        jb      __L_1;
        movntdq [edi], xmm0;
        movntdq 16[edi], xmm0;
        sub     ecx, 32;
        add     edi, 32;

__L_1:
        cmp     ecx, 16;
        jb      __dword;
        movntdq [edi], xmm0;
        sub     ecx, 16;
        add     edi, 16;

__dword:
        sfence;
        cmp    ecx, 4;
        jb     __word;
        stosd;
        cmp    ecx, 4;
        jb     __word;
        stosd;
        cmp    ecx, 4;
        jb     __word;
        stosd;

__word:
        cmp    ecx, 2;
        jb     __byte;
        sub    ecx, 2;
        stosw;
__byte:
        cmp    ecx, 1;
        jb     __done;
        stosb;
__done:
    }

#else

    memset(mem, 0, bytes);

#endif
}

void Myway::Memcpy(void * dest, const void * src, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, dest;
        mov     esi, src;
        mov     eax, bytes;

        mov     ecx, eax;
        shr     ecx, 2;
        rep     movsd;

        mov     ecx, eax;
        and     ecx, 3;
        rep     movsb;
    }

#else

    memcpy(dest, src, bytes);

#endif
}

void Myway::MemcpySSE(void * dest, const void * src, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, dest;
        mov     esi, src;
        mov     ecx, bytes;
        cmp     ecx, 128;
        jb      __L_4;

__L_8:
        prefetchnta [esi + 64];
        movaps  xmm0, [esi];
        movntps [edi], xmm0;
        movaps  xmm1, 16[esi];
        movntps 16[edi], xmm1;
        movaps  xmm2, 32[esi];
        movntps 32[edi], xmm2;
        movaps  xmm3, 48[esi];
        movntps 48[edi], xmm3;

        prefetchnta [esi + 128];
        movaps  xmm4, 64[esi];
        movntps 64[edi], xmm4;
        movaps  xmm5, 80[esi];
        movntps 80[edi], xmm5;
        movaps  xmm6, 96[esi];
        movntps 96[edi], xmm6;
        movaps  xmm7, 112[esi];
        movntps 112[edi], xmm7;

        add     esi, 128;
        add     edi, 128;
        sub     ecx, 128;
        cmp     ecx, 128;
        jae     __L_8;

__L_4:
        cmp     ecx, 64;
        jb      __L_2;

        prefetchnta [esi + 64];
        movaps  xmm0, [esi];
        movntps [edi], xmm0;
        movaps  xmm1, 16[esi];
        movntps 16[edi], xmm1;
        movaps  xmm2, 32[esi];
        movntps 32[edi], xmm2;
        movaps  xmm3, 48[esi];
        movntps 48[edi], xmm3;
        add     esi, 64;
        add     edi, 64;
        sub     ecx, 64;

__L_2:
        cmp     ecx, 32;
        jb      __L_1;
        movaps  xmm4, [esi];
        movntps [edi], xmm4;
        movaps  xmm5, 16[esi];
        movntps 16[edi], xmm5;
        add     esi, 32;
        add     edi, 32;
        sub     ecx, 32;

__L_1:
        cmp     ecx, 16;
        jb      __dword;
        movaps  xmm4, [esi];
        movntps [edi], xmm4;
        add     esi, 16;
        add     edi, 16;
        sub     ecx, 16;

__dword:
        sfence;

        cmp     ecx, 4;
        jb      __word;
        movsd;
        cmp     ecx, 4;
        jb      __word;
        movsd;
        cmp     ecx, 4;
        jb      __word;
        movsd;

__word:
        cmp     ecx, 2;
        jb      __byte;
        movsw;

__byte:
        cmp     ecx, 1;
        jb      __done;
        movsb;
__done:

    }

#else

    memcpy(dest, src, bytes);

#endif

}


void Myway::MemcpySSE2(void * dest, const void * src, int bytes)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     edi, dest;
        mov     esi, src;
        mov     ecx, bytes;
        cmp     ecx, 128;
        jb      __L_4;

__L_8:
        prefetchnta [esi + 64];
        movdqa  xmm0, [esi];
        movntdq [edi], xmm0;
        movdqa  xmm1, 16[esi];
        movntdq 16[edi], xmm1;
        movdqa  xmm2, 32[esi];
        movntdq 32[edi], xmm2;
        movdqa  xmm3, 48[esi];
        movntdq 48[edi], xmm3;

        prefetchnta [esi + 128];
        movdqa  xmm4, 64[esi];
        movntdq 64[edi], xmm4;
        movdqa  xmm5, 80[esi];
        movntdq 80[edi], xmm5;
        movdqa  xmm6, 96[esi];
        movntdq 96[edi], xmm6;
        movdqa  xmm7, 112[esi];
        movntdq 112[edi], xmm7;

        add     esi, 128;
        add     edi, 128;
        sub     ecx, 128;
        cmp     ecx, 128;
        jae     __L_8;

__L_4:
        cmp     ecx, 64;
        jb      __L_2;

        prefetchnta [esi + 64];
        movdqa  xmm0, [esi];
        movntdq [edi], xmm0;
        movdqa  xmm1, 16[esi];
        movntdq 16[edi], xmm1;
        movdqa  xmm2, 32[esi];
        movntdq 32[edi], xmm2;
        movdqa  xmm3, 48[esi];
        movntdq 48[edi], xmm3;
        add     esi, 64;
        add     edi, 64;
        sub     ecx, 64;

__L_2:
        cmp     ecx, 32;
        jb      __L_1;
        movdqa  xmm4, [esi];
        movntdq [edi], xmm4;
        movdqa  xmm5, 16[esi];
        movntdq 16[edi], xmm5;
        add     esi, 32;
        add     edi, 32;
        sub     ecx, 32;

__L_1:
        cmp     ecx, 16;
        jb      __dword;
        movdqa  xmm4, [esi];
        movntdq [edi], xmm4;
        add     esi, 16;
        add     edi, 16;
        sub     ecx, 16;

__dword:
        sfence;

        cmp     ecx, 4;
        jb      __word;
        movsd;
        cmp     ecx, 4;
        jb      __word;
        movsd;
        cmp     ecx, 4;
        jb      __word;
        movsd;

__word:
        cmp     ecx, 2;
        jb      __byte;
        movsw;

__byte:
        cmp     ecx, 1;
        jb      __done;
        movsb;
__done:
    }

#else

    memcpy(dest, src, bytes);

#endif
}


/*
void * operator new(size_t size)
{
   return _alloc(size, MEMORY_ALIGN_NONE, "unknow", 0);
}

void operator delete(void * p)
{
    _free(p, "unknow", 0);
}
*/

/*
void * operator new[](size_t size)
{
    return _alloc(size, MEMORY_ALIGN_NONE, "unknow", 0);
}

void operator delete[](void * p)
{
    _free(p, "unknow", 0);
}
*/

void * operator new(size_t size, int align, const char * file, int line)
{
    return _alloc(size, align, file, line);
}

void operator delete(void * p, const char * file, int line)
{
    _free(p, file, line);
}

void * operator new[](size_t size, int align, const char * file, int line)
{
    return _alloc(size, align, file, line);
}

void operator delete[](void * p, const char * file, int line)
{
    _free(p, file, line);
}

#pragma warning(pop)