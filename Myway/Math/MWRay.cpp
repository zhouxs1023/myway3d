#include "MWRay.h"
#include "MWMath.h"

namespace Myway
{

Ray::Ray()
{
}

Ray::Ray(const Vec3 & vOrigin, const Vec3 & vDirection)
: origin(vOrigin),
  direction(vDirection)
{
    Math::VecNormalize(direction, direction);
}

Ray::~Ray()
{
}


RayIntersectionInfo Ray::Intersection(const Plane & rk) const
{
    RayIntersectionInfo info;
    Math::RayIntersection(info, *this, rk);
    return info;
}

RayIntersectionInfo Ray::Intersection(const Sphere & rk) const
{
    RayIntersectionInfo info;
    Math::RayIntersection(info, *this, rk);
    return info;
}

RayIntersectionInfo Ray::Intersection(const Aabb & rk) const
{
    RayIntersectionInfo info;
    Math::RayIntersection(info, *this, rk);
    return info;
}

RayIntersectionInfo Ray::Intersection(const Tri3 & rk) const
{
    RayIntersectionInfo info;
    Math::RayIntersection(info, *this, rk);
    return info;
}

}