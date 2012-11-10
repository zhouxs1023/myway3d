#pragma once

#include "MWAllocObj.h"
#include "MWAllocator.h"

namespace Myway
{

template <class T>
class Queue : public AllocObj
{
protected:
    struct _member
    {
        _member *  next;
        T           mem;
    };

public:
    Queue();
    ~Queue();

    T &             Front();
    const T &       Front() const;
    T &             Back();
    const T &       Back() const;

    void            Push(const T & Val);
    void            Pop();
    int          Size() const;
    bool            Empty() const;
    void            Clear();

private:
    explicit        Queue(const Queue & q);
    Queue &         operator=(const Queue & q);

protected:
    Allocator<_member, 128>   mAlloc;
    _member *                 mFront;
    _member *                 mBack;
};

}