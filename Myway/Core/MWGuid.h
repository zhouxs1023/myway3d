#pragma once

#include "MWDefine.h"

namespace Myway
{

#define MAKE_DWORD(c1, c2, c3, c4) int(((c1) << 24) | ((c2) << 16) | ((c3) << 8) | (c4))
#define MAKE_WORD(c1, c2) short(((c1)<< 8) | (c2));

struct MW_ENTRY guid128
{
    int  key1;
    int  key2;
    int  key3;
    int  key4;

    inline guid128 & operator =(const guid128 & r)
    {
        key1 = r.key1;
        key2 = r.key2;
        key3 = r.key3;
        key4 = r.key4;
        return *this;
    }

    inline bool operator ==(const guid128 & r) const
    {
        return  key1 == r.key1 &&
                key2 == r.key2 &&
                key3 == r.key3 &&
                key4 == r.key4;
    }

    inline bool operator !=(const guid128 & r) const
    {
        return key1 != r.key1 ||
               key2 != r.key2 ||
               key3 != r.key3 ||
               key4 != r.key4;
    }
};
 
#define DECLARE_GUID128(id, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16)  \
    guid128 id = { MAKE_DWORD(c1, c2, c3, c4), MAKE_DWORD(c5, c6, c7, c8),                          \
                   MAKE_DWORD(c9, c10, c11, c12), MAKE_DWORD(c13, c14, c15, c16) }

#define DECLARE_GUID128_CONST(id, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16)  \
    const guid128 id = { MAKE_DWORD(c1, c2, c3, c4), MAKE_DWORD(c5, c6, c7, c8),                          \
                         MAKE_DWORD(c9, c10, c11, c12), MAKE_DWORD(c13, c14, c15, c16) }




struct MW_ENTRY guid96
{
    int  key1;
    int  key2;
    int  key3;

    inline guid96 & operator =(const guid96 & r)
    {
        key1 = r.key1;
        key2 = r.key2;
        key3 = r.key3;
        return *this;
    }

    inline bool operator ==(const guid96 & r) const
    {
        return key1 == r.key1 &&
               key2 == r.key2 &&
               key3 == r.key3;
    }

    inline bool operator !=(const guid96 & r) const
    {
        return key1 != r.key1 ||
               key2 != r.key2 ||
               key3 != r.key3;
    }
};

#define DECLARE_GUID96(id, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12)  \
    guid96 id = { MAKE_DWORD(c1, c2, c3, c4), MAKE_DWORD(c5, c6, c7, c8), MAKE_DWORD(c9, c10, c11, c12) }

#define DECLARE_GUID96_CONST(id, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12)  \
    const guid96 id = { MAKE_DWORD(c1, c2, c3, c4), MAKE_DWORD(c5, c6, c7, c8), MAKE_DWORD(c9, c10, c11, c12) }



struct MW_ENTRY guid64
{
    int  key1;
    int  key2;

    inline guid64 & operator =(const guid64 & r)
    {
        key1 = r.key1;
        key2 = r.key2;
        return *this;
    }

    inline bool operator ==(const guid64 & r) const
    {
        return key1 == r.key1 &&
               key2 == r.key2;
    }

    inline bool operator !=(const guid64 & r) const
    {
        return key1 != r.key1 ||
               key2 != r.key2;
    }
};

#define DECLARE_GUID64(id, c1, c2, c3, c4, c5, c6, c7, c8)  \
    guid64 id = { MAKE_DWORD(c1, c2, c3, c4), MAKE_DWORD(c5, c6, c7, c8) }

#define DECLARE_GUID64_CONST(id, c1, c2, c3, c4, c5, c6, c7, c8)  \
    const guid64 id = { MAKE_DWORD(c1, c2, c3, c4), MAKE_DWORD(c5, c6, c7, c8) }


struct MW_ENTRY guid32
{
    int  key;

    inline guid32 & operator =(const guid32 & r)
    {
        key = r.key;
        return *this;
    }

    inline bool operator ==(const guid32 & r) const
    {
        return key == r.key;
    }

    inline bool operator !=(const guid32 & r) const
    {
        return key != r.key;
    }
};

#define DECLARE_GUID32(id, c1, c2, c3, c4)  \
    guid64 id = { MAKE_DWORD(c1, c2, c3, c4) }

#define DECLARE_GUID32_CONST(id, c1, c2, c3, c4)  \
    const guid64 id = { MAKE_DWORD(c1, c2, c3, c4) }

}
