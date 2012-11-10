#include "MWMath.h"

namespace Myway
{

void Math::PlaneNormalize(Plane & pOut, const Plane & p)
{
    float inv_len = 1.0f / Math::Sqrt(p.a * p.a + p.b * p.b + p.c * p.c);
    pOut.a = p.a * inv_len;
    pOut.b = p.b * inv_len;
    pOut.c = p.c * inv_len;
    pOut.d = p.d * inv_len;
}

float Math::PlaneDistance(const Plane & p, const Vec3 & v)
{
    return v.x * p.n.x + v.y * p.n.y + v.z * p.n.z + p.d;
}

Plane::Side Math::PlaneSide(const Plane & p, const Vec3 & v)
{
    float d = PlaneDistance(p, v);

    if (d > 0)
        return Plane::POSITIVE;
    else if (d < 0)
        return Plane::NEGATIVE;
    else
        return Plane::NONE;
}

Plane::Side Math::PlaneSide(const Plane & p, const Aabb & box)
{
    Vec3 center = box.GetCenter();
    Vec3 half = box.GetHalfSize();

    return PlaneSide(p, center, half);
}

Plane::Side Math::PlaneSide(const Plane & p, const Vec3 & v, const Vec3 & half)
{
    float d = PlaneDistance(p, v);
    float dist = Math::Abs(half.x * p.n.x) + Math::Abs(half.y * p.n.y) + Math::Abs(half.z * p.n.z);

    if (d > dist)
        return Plane::POSITIVE;
    else if (d < -dist)
        return Plane::NEGATIVE;
    else
        return Plane::BOTH;
}

Plane::Side Math::PlaneSide(const Plane & p, const Sphere& sph)
{
    float d = PlaneDistance(p, sph.center);

    if (d > sph.radius)
        return Plane::POSITIVE;
    else if (d < -sph.radius)
        return Plane::NEGATIVE;
    else
        return Plane::BOTH;
}


void Math::PlaneTransform(Plane & pOut, const Plane & p, const Mat4 & m)
{
    /*
    position = plane.n * -d;
    normal = plane.normal;

    transform position & normal

    it's ok.
    */

    Vec3 pos;

    VecTransform(pos, p.n * -p.d, m);
    VecTransformN(pOut.n, p.n, m);

    pOut.d = -VecDot(pOut.n, pos);
}


}