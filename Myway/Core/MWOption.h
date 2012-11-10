#pragma once

#include "MWDebug.h"
#include "MWAllocObj.h"

namespace Myway
{

class MW_ENTRY Option : public AllocObj
{
    typedef void * ptr;

    struct Op
    {
        static const int len = 64;

        char   name[len];
        ptr    val;
    };

    static const int MAX_OPTIONS = 32;

public:
    Option();
    ~Option();

    void Clear();

    ptr & operator [] (const char * name);

    const ptr & operator [] (const char * name) const;

protected:
    Op  mItems[MAX_OPTIONS];
};

}