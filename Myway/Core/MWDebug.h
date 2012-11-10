#pragma once

#include "MWDefine.h"

namespace Myway
{

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef assert
#undef assert
#endif

#ifdef VERIFY
#undef VERIFY
#endif

#ifdef verify
#undef verify
#endif

#ifdef TRACE
#undef TRACE
#endif

#ifdef memory_check
#undef memory_check
#endif

#define DEBUG_BACKPOINT			0
#define DEBUG_IGNORE			1
#define DEBUG_CONTINUE			2

#define OPEN_APPEND				1
#define OPEN_OUT				2

#define STRINGIZING(x)          #x

#ifdef _DEBUG

#define debug_assert(x, comment)	\
{									\
	static bool bIgnore = false;	\
	if (!bIgnore && !(x))			\
	{								\
		switch (Myway::DisplayError("ASSERT FAILED!", #x, comment, __FILE__, __LINE__)) \
		{							\
		case DEBUG_IGNORE:			\
			bIgnore = true;         \
			break;					\
		case DEBUG_BACKPOINT:		\
			__asm {int 3};          \
			break;					\
		}							\
	}								\
}

#define debug_printf        Myway::Output
#define debug_memory_check  Myway::EnableMemoryLeakCheck
#define debug_verify(x)     debug_assert(x, "")


#else

#define debug_assert(x, comment)
#define debug_printf
#define debug_memory_check
#define debug_verify(x) x

#endif

#define d_printf                debug_printf
#define assert(x)	            debug_assert(x, "")
#define d_assert(x)	            debug_assert(x, "")
#define assert(x)	            debug_assert(x, "")
#define ASSERT(x)	            debug_assert(x, "")
#define verify(x)               debug_verify(x)
#define VERIFY(x)               debug_verify(x)
#define TRACE		            debug_printf
#define memory_check            debug_memory_check

int MW_ENTRY DisplayError(const char * title, 
					      const char * text, 
					      const char * description, 
					      const char * file, 
					      int line);

void MW_ENTRY Output(const char * fmt, ...);

void MW_ENTRY EnableMemoryLeakCheck();
void MW_ENTRY BreakAllock(int time);



/*
    while assert
*/
#ifdef _DEBUG

    inline bool __while_assert(bool a)
    {
        debug_assert(a, "while limit.");
        return a;
    }

    #define __UNIQUE_VAR(x)  __unique##x
    #define __while_limit(a, b, c) \
        debug_assert(b > 0, "while limit, limit must be greater 0."); \
        int __UNIQUE_VAR(c) = b; \
        while ((a) && __while_assert(--__UNIQUE_VAR(c) >= 0))

    #define _while_limit(a, b)  __while_limit(a, b, __COUNTER__)

#else
    #define _while_limit(a, b)   while (a)

#endif

/*
    compile assert
        @example:
            compile_assert(sizeof(MyEnum) == sizeof(unsigned int));
*/
#define compile_assert(expn) typedef char __C_ASSERT__[(expn) ? 1 : -1]

/*
    array_count
        @example:
            int count = array_count(a);
*/
#define array_count(a) sizeof(a) / sizeof((a)[0])


/*
    while limit
*/
#define while_limit(a, b) _while_limit(a, b)


/* :) float exception
-------------------------------------------------------------------
    @Remark:
        _EM_INVALID:        非法异常.
        _EM_ZERODIVIDE:     除0异常.
        _EM_OVERFLOW:       溢出异常.
        _EM_INEXACT:        不精确异常.(应该关闭)
        _EM_UNDERFLOW:      下溢. (应该关闭)
        _EM_DENORMAL:       非正常浮点数异常. (测试时用)
-------------------------------------------------------------------
*/
#ifdef _DEBUG

#pragma warning(push)
#pragma warning(disable : 4996)

    inline void __enable_fp_exception()
    {
        int i = _controlfp(0, 0);
        i &= ~(EM_INVALID | EM_ZERODIVIDE | EM_OVERFLOW);
        _controlfp(i, MCW_EM);
    }

    inline void __disable_fp_exception()
    {
        int i = _controlfp(0, 0);
        i &= EM_INVALID | EM_ZERODIVIDE | EM_OVERFLOW;
        _controlfp(i, MCW_EM);
    }

    #define _ENABLE_FP_EXCEPTION    __enable_fp_exception()
    #define _DISABLE_FP_EXCEPTION   __disable_fp_exception()

#pragma warning(pop)

#else

    #define _ENABLE_FP_EXCEPTION
    #define _DISABLE_FP_EXCEPTION

#endif
    
#define ENABLE_FP_EXCEPTION     _ENABLE_FP_EXCEPTION
#define DISABLE_FP_EXCEPTION    _DISABLE_FP_EXCEPTION

}



