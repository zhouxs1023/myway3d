#pragma once

#include "MWAllocObj.h"

namespace Myway
{

class MW_ENTRY RefObj
{
public:
    RefObj();
    virtual ~RefObj();

    int IncRef();
    int DecRef();

    int RefCount();

public:
    virtual void DeleteSelf() = 0;

protected:
    int       mReferenceCount;
};

#include "MWRefObj.inl"

#define DeclareRefObj() \
public: \
    virtual void DeleteSelf();

/*
public: \
    template <class T> \
    friend class SmartPtr;
*/

}