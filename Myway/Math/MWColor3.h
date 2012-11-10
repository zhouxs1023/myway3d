#pragma once

#include "MWCore.h"

namespace Myway
{

class MW_ENTRY Color3 : public AllocObj
{
public:
    float r;
    float g;
    float b;

    Color3() {}
    Color3(float v);
    Color3(float r, float g, float b);
    Color3(const Color3 & c);

    Color3& operator =(const Color3 & c);
    Color3 operator +(const Color3 & c) const;
    Color3 operator -(const Color3 & c) const;
    Color3 operator *(const Color3 & c) const;
    Color3 operator *(float v) const;

    Color3& operator +=(const Color3 & c);
    Color3& operator -=(const Color3 & c);
    Color3& operator *=(const Color3 & c);
    Color3& operator *=(float v);

    bool operator ==(const Color3 & c) const;
    bool operator !=(const Color3 & c) const;

    friend Color3 operator *(float v, const Color3 & c);
    friend std::ostream & operator <<(std::ostream & os, const Color3 & c);
};

}