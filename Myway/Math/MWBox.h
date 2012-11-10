#pragma once

#include "MWVector2.h"
#include "MWVector3.h"
#include "MWVector4.h"

namespace Myway
{

template <class T>
class Box : public AllocObj
{
public:
    Box();
    Box(const T & left, const T & top, const T & right, const T & bottom, const T & front, const T & back);
    ~Box();

    Box operator *(float Scale) const;
    Box operator /(float Scale) const;
    Box & operator *=(float Scale);
    Box & operator /=(float Scale);

    T Width() const;
    T Height() const;
    T Depth() const;
    
    friend MW_ENTRY Box operator * (T Scale, const Box & bx);
    friend MW_ENTRY Box operator / (T Scale, const Box & bx);

public:
    T Left;
    T Top;
    T Right;
    T Bottom;
    T Front;
    T Back;
};

typedef Box<int>     Boxi;
typedef Box<float>     Boxf;
typedef Box<Vec2>      Box2;
typedef Box<Vec3>      Box3;
typedef Box<Vec4>      Box4;




template <class T>
inline Box<T>::Box()
{
}

template <class T>
Box<T>::Box(const T & left,
            const T & top,
            const T & right,
            const T & bottom,
            const T & front,
            const T & back)
            : Left(left),
            Top(top),
            Right(right),
            Bottom(bottom),
            Front(front),
            Back(back)
{
}

template <class T>
Box<T>::~Box()
{}

template <class T>
inline Box<T> Box<T>::operator *(float Scale) const
{
    Box bxRet;

    bxRet.Left      = Left * Scale;
    bxRet.Top       = Top * Scale;
    bxRet.Right     = Right * Scale;
    bxRet.Bottom    = Bottom * Scale;
    bxRet.Front     = Front * Scale;
    bxRet.Back      = Back * Scale;

    return bxRet;
}

template <class T>
inline Box<T> Box<T>::operator /(float Scale) const
{
    Box bxRet;
    float fInvScale = 1.0f / Scale;

    bxRet.Left      = Left * fInvScale;
    bxRet.Top       = Top * fInvScale;
    bxRet.Right     = Right * fInvScale;
    bxRet.Bottom    = Bottom * fInvScale;
    bxRet.Front     = Front * fInvScale;
    bxRet.Back      = Back * fInvScale;

    return bxRet;
}

template <class T>
inline Box<T> & Box<T>::operator *=(float Scale)
{
    Left    *= Scale;
    Top     *= Scale;
    Right   *= Scale;
    Bottom  *= Scale;
    Front   *= Scale;
    Back    *= Scale;

    return *this;
}

template <class T>
inline Box<T> & Box<T>::operator /=(float Scale)
{
    float fInvScale = 1.0f / Scale;

    Left    *= fInvScale;
    Top     *= fInvScale;
    Right   *= fInvScale;
    Bottom  *= fInvScale;
    Front   *= fInvScale;
    Back    *= fInvScale;

    return *this;
}

template <class T>
inline T Box<T>::Width() const
{
    return Right - Left;
}

template <class T>
inline T Box<T>::Height() const
{
    return Bottom - Top;
}

template <class T>
inline T Box<T>::Depth() const
{
    return Back - Front;
}

template <class T>
inline Box<T> operator * (T Scale, const Box<T> & bx)
{
    return bx * Scale;
}

template <class T>
inline Box<T> operator / (T Scale, const Box<T> & bx)
{
    return bx / Scale;
}

}