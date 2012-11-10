#pragma once

#include "MWVector3.h"

namespace Myway
{

class Aabb;

class MW_ENTRY Sphere
{
    DECLARE_ALLOC();

public:
    static const Sphere Zero;
    static const Sphere Infinite;

public:
    Sphere();
    Sphere(const Vec3 & vCenter, float fRadius);
    Sphere(float x, float y, float z, float fRadius);
    ~Sphere();

    Sphere & operator =(const Sphere & sph);

    Sphere   operator *(float x) const;
    Sphere   operator /(float x) const;

    Sphere & operator *=(float x);
    Sphere & operator /=(float x);

    friend Sphere operator *(float x, const Sphere & sph);
    friend Sphere operator /(float x, const Sphere & sph);

public:
    float   Volume() const;
    Sphere  Merge(const Sphere & rk) const;
    Sphere  Merge(const Vec3 & rk) const;
    Sphere  Transform(const Mat4 & mat) const;
    int     Intersection(const Sphere & rk) const;
    int     Intersection(const Vec3 & rk) const;
    int     Intersection(const Aabb & rk) const;


public:
    Vec3    center;
    float   radius;
};

}