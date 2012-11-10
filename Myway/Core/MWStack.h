#pragma once

#include "MWAllocObj.h"
#include "MWAllocator.h"


namespace Myway
{

template <class T>
class Stack : public AllocObj
{
    struct _member
    {
        _member *  next;
        T           mem;
    };

public:
                Stack();
                ~Stack();

    void        Clear();
    bool        Empty() const;
    void        Push(const T & Val);
    void        Pop();
    int         Size() const;
    T &         Top();
    const T &   Top() const;

private:
    explicit    Stack(const Stack & s);
    Stack &     operator =(const Stack & s);

protected:
    Allocator<_member, 128>   mAlloc;
    _member *                 mTop;
};

#include "MWStack.inl"

}