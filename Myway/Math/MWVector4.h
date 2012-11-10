#pragma once

#include "MWCore.h"
#include "MWVector3.h"

namespace Myway
{

class Mat4;

class MW_ENTRY Vec4
{
    DECLARE_ALLOC();

public:
    static const Vec4 Zero;
    static const Vec4 Unit;
    static const Vec4 UnitX;
    static const Vec4 UnitY;
    static const Vec4 UnitZ;
    static const Vec4 NegUnitX;
    static const Vec4 NegUnitY;
    static const Vec4 NegUnitZ;

public:
    float x, y, z, w;

public:
    Vec4();
    ~Vec4();
    Vec4(float v);
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3 & xyz, float w);
    Vec4(const Vec4 & v);

    Vec4& operator = (const Vec4 & v);
    operator float* ();
    operator const float* () const;

    bool operator ==(const Vec4 & v) const;
    bool operator ==(float v) const;
    bool operator !=(const Vec4 & v) const;
    bool operator !=(float v) const;
    bool operator > (const Vec4 & v) const;
    bool operator >=(const Vec4 & v) const;
    bool operator < (const Vec4 & v) const;
    bool operator <=(const Vec4 & v) const;
    bool operator > (float v) const;
    bool operator >=(float v) const;
    bool operator < (float v) const;
    bool operator <=(float v) const;

    Vec4 operator -() const;

    Vec4 operator + (const Vec4 & v) const;
    Vec4 operator - (const Vec4 & v) const;
    Vec4 operator * (const Vec4 & v) const;
    Vec4 operator / (const Vec4 & v) const;
    Vec4 operator * (float f) const;
    Vec4 operator / (float f) const;

    Vec4& operator += (const Vec4 & v);
    Vec4& operator -= (const Vec4 & v);
    Vec4& operator *= (float f);
    Vec4& operator /= (float f);

    Vec4& operator *= (const Vec4 & v);
    Vec4& operator /= (const Vec4 & v);

    friend MW_ENTRY Vec4 operator * (float f, const Vec4 & v);
    friend MW_ENTRY Vec4 operator / (float f, const Vec4 & v);
    friend MW_ENTRY std::ostream & operator << (std::ostream & os, const Vec4 & v);

    Vec4 operator * (const Mat4 & m) const;

public:
    Vec4 Minimum(const Vec4 & rk) const;
    float Dot(const Vec4 & rk) const;
    float Length() const;
    float LengthSq() const;
    float InvLength() const;
    float Distance(const Vec4 & rk) const;
    float DistanceSq(const Vec4 & rk) const;
    Vec4 Reflect(const Vec4 & n) const;
    Vec4 Refract(const Vec4 & n, float eta) const;
    Vec4 Cross(const Vec4 & rk) const;
    Vec4 CrossN(const Vec4 & rk) const;
    Vec4 Transform(const Mat4 & m) const;
    Vec4 TransformN(const Mat4 & m) const;
    Vec4 Lerp(const Vec4 & rk, float t) const;
    Vec4 Normalize() const;
};

}