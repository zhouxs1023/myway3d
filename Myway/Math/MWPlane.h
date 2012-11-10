#pragma once

#include "MWVector3.h"

namespace Myway
{

class Mat4;
class Aabb;
class Sphere;

class MW_ENTRY Plane
{
    DECLARE_ALLOC();

public:
    static const Plane Zero;

public:
    enum Side
    {
        NONE,
        POSITIVE,
        NEGATIVE,
        BOTH,

        MW_ALIGN_ENUM(Side)
    };

    union
    {
        struct {
            Vec3 n;
            float d;
        };

        struct {
            float a, b, c, d;
        };
    };

public:
    Plane();
    ~Plane();

    Plane(const Plane & p);
    Plane(const Vec3 & n, float d);
    Plane(const Vec3 & n, const Vec3 & p);
    Plane(float a, float b, float c, float d);
    Plane(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);

    Plane& operator =(const Plane & p);
    Plane operator -() const;

    bool operator ==(const Plane & p) const;
    bool operator !=(const Plane & p) const;

    friend MW_ENTRY std::ostream & operator << (std::ostream & os, const Plane & p);


public:
    Plane        Normalize() const;
    float        Distance(const Vec3 & v) const;
    Plane::Side  AtSide( const Vec3 & v) const;
    Plane::Side  AtSide(const Aabb & box) const;
    Plane::Side  AtSide(const Vec3 & v, const Vec3 & halfSize) const;
    Plane::Side  AtSide(const Sphere & box) const;
    Plane        Transform(const Mat4 & m);
};

}