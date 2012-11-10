#pragma once

#include "MWCore.h"

namespace Myway
{

class Mat3;

class MW_ENTRY Vec2
{
    DECLARE_ALLOC();

public:
    static const Vec2 Zero;
    static const Vec2 Unit;
    static const Vec2 UnitX;
    static const Vec2 UnitY;
    static const Vec2 NegUnitX;
    static const Vec2 NegUnitY;

public:
    float x, y;

public:
    Vec2();
    ~Vec2();
    Vec2(float v);
    Vec2(float x, float y);
    Vec2(const Vec2 & v);

    Vec2& operator = (const Vec2 & v);
    operator float* ();
    operator const float* () const;

    bool operator ==(const Vec2 & v) const;
    bool operator !=(const Vec2 & v) const;
    Vec2 operator -() const;

    Vec2 operator + (const Vec2 & v) const;
    Vec2 operator - (const Vec2 & v) const;
    Vec2 operator * (const Vec2 & v) const;
    Vec2 operator / (const Vec2 & v) const;
    Vec2 operator * (float f) const;
    Vec2 operator / (float f) const;

    Vec2& operator += (const Vec2 & v);
    Vec2& operator -= (const Vec2 & v);
    Vec2& operator *= (float f);
    Vec2& operator /= (float f);

    friend MW_ENTRY Vec2 operator * (float f, const Vec2 & v);
    friend MW_ENTRY Vec2 operator / (float f, const Vec2 & v);
    friend MW_ENTRY std::ostream & operator << (std::ostream & os, const Vec2 & v);

    Vec2 operator * (const Mat3 & m) const;

public:
    Vec2 Minimum(const Vec2 & rk) const;
    Vec2 Normalize() const;
    float Dot(const Vec2 & rk) const;
    float Length() const;
    float LengthSq() const;
    float InvLength() const;
    float Distance(const Vec2 & rk) const;
    float DistanceSq(const Vec2 & rk) const;
    Vec2 Reflect(const Vec2 & n) const;
    Vec2 Refract(const Vec2 & n, float eta) const;
    Vec2 Transform(const Mat3 & m) const;
    Vec2 TransformN(const Mat3 & m) const;
    Vec2 Lerp(const Vec2 & rk, float t) const;
    Vec2 Perpendicular() const;
};

}