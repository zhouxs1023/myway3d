#pragma once

#include "MWVector2.h"

namespace Myway
{

class Mat4;

class MW_ENTRY Vec3
{
    DECLARE_ALLOC();

public:
    static const Vec3 Zero;
    static const Vec3 Unit;
    static const Vec3 UnitX;
    static const Vec3 UnitY;
    static const Vec3 UnitZ;
    static const Vec3 NegUnitX;
    static const Vec3 NegUnitY;
    static const Vec3 NegUnitZ;

public:
    float x, y, z;

public:
    Vec3();
    ~Vec3();
    Vec3(float v);
    Vec3(float x, float y, float z);
    Vec3(const Vec3 & v);

    Vec3& operator = (const Vec3 & v);
    operator float* ();
    operator const float* () const;

    Vec3 operator -() const;

    bool operator ==(const Vec3 & v) const;
    bool operator ==(float v) const;
    bool operator !=(const Vec3 & v) const;
    bool operator !=(float v) const;
    bool operator > (const Vec3 & v) const;
    bool operator >=(const Vec3 & v) const;
    bool operator < (const Vec3 & v) const;
    bool operator <=(const Vec3 & v) const;
    bool operator > (float v) const;
    bool operator >=(float v) const;
    bool operator < (float v) const;
    bool operator <=(float v) const;

    Vec3 operator + (const Vec3 & v) const;
    Vec3 operator - (const Vec3 & v) const;
    Vec3 operator * (const Vec3 & v) const;
    Vec3 operator / (const Vec3 & v) const;
    Vec3 operator * (float f) const;
    Vec3 operator / (float f) const;

    Vec3& operator += (const Vec3 & v);
    Vec3& operator -= (const Vec3 & v);
    Vec3& operator *= (float f);
    Vec3& operator /= (float f);

    friend MW_ENTRY Vec3 operator * (float f, const Vec3 & v);
    friend MW_ENTRY Vec3 operator / (float f, const Vec3 & v);
    friend MW_ENTRY std::ostream & operator << (std::ostream & os, const Vec3 & v);

    Vec3 operator * (const Mat4 & m) const;
    Vec3 & operator *= (const Mat4 & m);

public:
	Vec3 Minimum(const Vec3 & rk) const;
    Vec3 Maximum(const Vec3 & rk) const;
    float Dot(const Vec3 & rk) const;
    float Length() const;
    float LengthSq() const;
    float InvLength() const;
    float Distance(const Vec3 & rk) const;
    float DistanceSq(const Vec3 & rk) const;
    Vec3 Reflect(const Vec3 & n) const;
    Vec3 Refract(const Vec3 & n, float eta) const;
    Vec3 Cross(const Vec3 & rk) const;
    Vec3 CrossN(const Vec3 & rk) const;
    Vec3 Transform(const Mat4 & m) const;
    Vec3 TransformN(const Mat4 & m) const;
    Vec3 TransformA(const Mat4 & m) const;
    Vec3 Lerp(const Vec3 & rk, float t) const;
    Vec3 Normalize() const;
    float NormalizeL();
};



}