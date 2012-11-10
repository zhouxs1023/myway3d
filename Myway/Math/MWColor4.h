#pragma once

#include "MWCore.h"

namespace Myway
{

class MW_ENTRY Color4
{
    DECLARE_ALLOC();

public:
    static const Color4 White;
    static const Color4 Red;
    static const Color4 Green;
    static const Color4 Blue;
    static const Color4 Black;
    static const Color4 Gray;

public:
    struct
    {
        float r;
        float g;
        float b;
        float a;
    };

    Color4() {}
    Color4(float v);
    Color4(float r, float g, float b, float a = 1.0f);
    Color4(const Color4 & c);

    Color4& operator =(const Color4 & c);
    Color4 operator +(const Color4 & c) const;
    Color4 operator -(const Color4 & c) const;
    Color4 operator *(const Color4 & c) const;
    Color4 operator *(float v) const;
    Color4 operator /(float v) const;

    Color4& operator +=(const Color4 & c);
    Color4& operator -=(const Color4 & c);
    Color4& operator *=(const Color4 & c);
    Color4& operator *=(float v);
    Color4& operator /=(float v);

    bool operator ==(const Color4 & c) const;
    bool operator !=(const Color4 & c) const;

    friend MW_ENTRY Color4 operator *(float v, const Color4 & c);
    friend MW_ENTRY std::ostream & operator <<(std::ostream & os, const Color4 & c);

public:
    Color4 Normalize() const;
    Color4 Saturate() const;
    Color4 Lerp(const Color4 & rk, float t) const;
};

}
