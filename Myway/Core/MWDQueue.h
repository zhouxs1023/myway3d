#pragma once

#include "MWAllocObj.h"

namespace Myway
{

template <class T>
class DQueue : public AllocObj
{
public:
    DQueue();
    DQueue(int iAllcateSize);
    ~DQueue();

    T &             Front();
    const T &       Front() const;
    T &             Back();
    const T &       Back() const;

    void            PushFront(const T & Val);
    void            PopFront();
    void            PushBack(const T & Val);
    void            PopBack();
    int             Size() const;
    bool            Empty() const;
    void            Clear();

private:
    explicit        DQueue(const DQueue & q);
    DQueue &        operator=(const DQueue & q);

protected:
    int       mSize;
    int       mIncreaseSize;
    int       mMaxSize;
    T *       mMember;
};

}