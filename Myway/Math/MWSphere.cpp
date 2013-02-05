#include "MWSphere.h"
#include "MWMath.h"

namespace Myway
{

const Sphere Sphere::Zero     = Sphere(0, 0, 0, 0);
const Sphere Sphere::Infinite = Sphere(0, 0, 0, FLT_MAX);

/* :) Sphere
---------------------------------------------------------------------------------------
    @Remark:
        3d Sphere.
---------------------------------------------------------------------------------------
*/
inline Sphere::Sphere()
{
}

inline Sphere::Sphere(const Vec3 & _vCenter, float _fRadius)
: center(_vCenter.x, _vCenter.y, _vCenter.z),
  radius(_fRadius)
{
}

inline Sphere::Sphere(float x, float y, float z, float _fRadius)
: center(x, y, z),
  radius(_fRadius)
{
}

inline Sphere::~Sphere()
{
}

inline Sphere & Sphere::operator =(const Sphere & sph)
{
    center = sph.center;
    radius = sph.radius;
    return *this;
}

inline Sphere Sphere::operator *(float x) const
{
    return Sphere(center * x, radius * x);
}

inline Sphere Sphere::operator /(float x) const
{
    x = 1.0f / x;
    return Sphere(center * x, radius * x);
}

inline Sphere & Sphere::operator *=(float x)
{
    center *= x;
    radius *= x;
    return *this;
}

inline Sphere & Sphere::operator /=(float x)
{
    x = 1.0f / x;
    center *= x;
    radius *= x;
    return *this;
}




float Sphere::Volume() const
{
    return Math::SphereVolume(*this);
}

Sphere Sphere::Merge(const Sphere & rk) const
{
    Sphere sph;
    Math::SphereMerge(sph, *this, rk);
    return sph;
}

Sphere Sphere::Merge(const Vec3 & rk) const
{
    Sphere sph;
    Math::SphereMerge(sph, *this, rk);
    return sph;
}

Sphere Sphere::Transform(const Mat4 & mat) const
{
    Sphere sph;
    Math::SphereTransform(sph, *this, mat);
    return sph;
}

int Sphere::Intersection(const Sphere & rk) const
{
    return Math::SphereIntersection(*this, rk);
}

int Sphere::Intersection(const Vec3 & rk) const
{
    return Math::SphereIntersection(*this, rk);
}

int Sphere::Intersection(const Aabb & rk) const
{
    return Math::SphereIntersection(*this, rk);
}

}
