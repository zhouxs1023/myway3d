
#pragma once

#include "MWAllocObj.h"

namespace Myway
{

template <class T> 
class Visitor : public AllocObj
{
public:
    Visitor(T begin, T end) 
    : mBegin(begin), 
      mEnd(end),
      mIter(begin)
    {
    }

    ~Visitor()
    {
    }

    inline Visitor & operator ++()
    {
        ++mIter;
        return *this;
    }

    inline Visitor operator ++(int)
    {
        Visitor vistor = *this;
        ++(*this);
        return vistor;
    }

    inline Visitor & operator --()
    {
        --mIter;
        return *this;
    }

    inline Visitor operator --(int)
    {
        Visitor vistor = *this;
        --(*this);
        return vistor;
    }

    inline Visitor & operator =(const Visitor & r)
    {
        mIter = r.mIter;
        mBegin = r.mBegin;
        mEnd = r.mEnd;
        return *this;
    }

    inline Visitor(const Visitor & r)
    {
        mIter = r.mIter;
        mBegin = r.mBegin;
        mEnd = r.mEnd;
    }

    inline T & Begin()
    {
        return mBegin;
    }

    inline T & End()
    {
        return mEnd;
    }

    inline T & Cursor()
    {
        return mIter;
    }

	inline bool Beginof() const
	{
		return mIter == mBegin;
	}

	inline bool bof() const
	{
		return mIter == mBegin;
	}

	inline bool Endof() const
	{
		return mIter == mEnd;
	}

	inline bool eof() const
	{
		return mIter == mEnd;
	}

protected:
    T           mIter;
    T           mBegin;
    T           mEnd;
};

}