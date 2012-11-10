#include "MWMath.h"

namespace Myway
{

void Math::RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Plane & plane)
{
    float ddn = VecDot(ray.direction, plane.n);

    if (Math::Abs(ddn) < Math::EPSILON_E5)
    {
        info.iterscetion = FALSE;
        info.distance = 0;
    }
    else
    {
        float dt = PlaneDistance(plane, ray.origin);
        float t = -(dt / ddn);
        info.iterscetion = (t >= 0);
        info.distance = t;
    }

}

void Math::RayIntersection(RayIntersectionInfo & info, const Vec3 & orig, const Vec3 & target, const Plane & plane)
{
    Ray ray(orig, target - orig);
    
    RayIntersection(info, ray, plane);

    if (info.iterscetion)
    {
        info.iterscetion = info.distance * info.distance <= VecDistanceSq(target, orig);
    }
}

void Math::RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Sphere& sph)
{
    const Vec3 po = ray.origin - sph.center;
    float radiusSq = sph.radius * sph.radius;
    float pol = VecLengthSq(po);

    if (pol < radiusSq)
    {
        info.iterscetion = TRUE;
        info.distance = 0;
    }
    else
    {
        // In 2 dimension
        // solve this equations set
        //
        // (x - o)^2 = r * r
        // (p + dt) = x;
        //
        // ==>
        //
        // (dt + po)^2 = r * r
        //
        float a = VecDot(ray.direction, ray.direction);
        float b = 2.0f * VecDot(ray.direction, po);
        float c = pol - radiusSq;

        //b^2 - 4ac
        float delta = b * b - 4.0f * a * c;
        if (delta < 0)
        {
            info.iterscetion = FALSE;
            info.distance = MAX_FLOAT;
        }
        else
        {
            // t = (-b +/- sqrt(detla) / 2a
            float sqrtd = Math::Sqrt(delta);
            float inv2a = 0.5f / a;
            float t1 = (-b - sqrtd) * inv2a;
            float t2 = (-b + sqrtd) * inv2a;

            info.iterscetion = TRUE;
            info.distance = Math::Minimum(t1, t2);
        }
    }
}

void Math::RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Aabb & aabb)
{
    //inside
    if (ray.origin > aabb.minimum && ray.origin < aabb.maximum)
    {
        info.iterscetion = TRUE;
        info.distance = 0;
        return;
    }

    //outside
    float t;
    float lowt = 0.0f;
    bool hit = FALSE;
    Vec3 hitpos;

    //check each face
    
    //minimum x
    if (ray.origin.x <= aabb.minimum.x && ray.direction.x > 0.0f)
    {
        t = (aabb.minimum.x - ray.origin.x) / ray.direction.x;
        if (t >= 0)
        {
            hitpos = ray.origin + ray.direction * t;

            if (hitpos.y >= aabb.minimum.y && hitpos.y <= aabb.maximum.y &&
                hitpos.z >= aabb.minimum.z && hitpos.z <= aabb.maximum.z &&
                (!hit || t < lowt))
            {
                hit = TRUE;
                lowt = t;
            }
        }
    }

    //maximum x
    if (ray.origin.x >= aabb.maximum.x && ray.direction.x < 0.0f)
    {
        t = (aabb.maximum.x - ray.origin.x) / ray.direction.x;
        if (t >= 0)
        {
            hitpos = ray.origin + ray.direction * t;

            if (hitpos.y >= aabb.minimum.y && hitpos.y <= aabb.maximum.y &&
                hitpos.z >= aabb.minimum.z && hitpos.z <= aabb.maximum.z &&
                (!hit || t < lowt))
            {
                hit = TRUE;
                lowt = t;
            }
        }
    }

    //minimum y
    if (ray.origin.y <= aabb.minimum.y && ray.direction.y > 0.0f)
    {
        t = (aabb.minimum.y - ray.origin.y) / ray.direction.y;
        if (t >= 0)
        {
            hitpos = ray.origin + ray.direction * t;

            if (hitpos.x >= aabb.minimum.x && hitpos.x <= aabb.maximum.x &&
                hitpos.z >= aabb.minimum.z && hitpos.z <= aabb.maximum.z &&
                (!hit || t < lowt))
            {
                hit = TRUE;
                lowt = t;
            }
        }
    }

    //maximum y
    if (ray.origin.y >= aabb.maximum.y && ray.direction.y < 0.0f)
    {
        t = (aabb.maximum.y - ray.origin.y) / ray.direction.y;
        if (t >= 0)
        {
            hitpos = ray.origin + ray.direction * t;

            if (hitpos.x >= aabb.minimum.x && hitpos.x <= aabb.maximum.x &&
                hitpos.z >= aabb.minimum.z && hitpos.z <= aabb.maximum.z &&
                (!hit || t < lowt))
            {
                hit = TRUE;
                lowt = t;
            }
        }
    }

    //minimum z
    if (ray.origin.z <= aabb.minimum.z && ray.direction.z > 0.0f)
    {
        t = (aabb.minimum.z - ray.origin.z) / ray.direction.z;
        if (t >= 0)
        {
            hitpos = ray.origin + ray.direction * t;

            if (hitpos.x >= aabb.minimum.x && hitpos.x <= aabb.maximum.x &&
                hitpos.y >= aabb.minimum.y && hitpos.y <= aabb.maximum.y &&
                (!hit || t < lowt))
            {
                hit = TRUE;
                lowt = t;
            }
        }
    }

    //maximum z
    if (ray.origin.z >= aabb.maximum.z && ray.direction.z < 0.0f)
    {
        t = (aabb.maximum.z - ray.origin.z) / ray.direction.z;
        if (t >= 0)
        {
            hitpos = ray.origin + ray.direction * t;

            if (hitpos.x >= aabb.minimum.x && hitpos.x <= aabb.maximum.x &&
                hitpos.y >= aabb.minimum.y && hitpos.y <= aabb.maximum.y &&
                (!hit || t < lowt))
            {
                hit = TRUE;
                lowt = t;
            }
        }
    }

    info.iterscetion = hit;
    info.distance = lowt;
}

void Math::RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Tri3 & tri)
{
    /*
        p = o + dt;
        p = sa + qb + rc;
        s + q + r = 1;

        ==>

        a + (b - a)q + (c - a)r = o + dt

        ==>

        -dt + (b-a)q + (c-a) r = o - a

        ==>
        
                            | t |   
        |-d, b - a, c - a|  | q | = | o - a |
                            | r |

        ==>

               |o - a, b - a, c - a|
        t = --------------------------
                |-d, b - a, c - a|

                |-d, o - a, c - a|
        q = --------------------------
                |-d, b - a, c - a|

                |-d, b - a, o - a|
        r = --------------------------
                |-d, b - a, c - a|
    */

    Vec3 ab = tri.b - tri.a;
    Vec3 ac = tri.c - tri.a;

    Vec3 cross;
    VecCross(cross, ray.direction, ac);
    float det = VecDot(ab, cross);

    //parallel
    if (det < Math::EPSILON_E4)
    {
        info.iterscetion = FALSE;
        info.distance = MAX_FLOAT;
        return;
    }

    Vec3 ray2a = ray.origin - tri.a;

    //q
    float q = VecDot(ray2a, cross);

    if (q < 0.0f || q > det)
    {
        info.iterscetion = FALSE;
        info.distance = MAX_FLOAT;
        return;
    }

    //r
    VecCross(cross, ray2a, ab);
    float r = VecDot(ray.direction, cross);

    if (r < 0.0f || r + q > det)
    {
        info.iterscetion = FALSE;
        info.distance = MAX_FLOAT;
        return;
    }

    //t
    float t = VecDot(ac, cross);

    info.iterscetion = TRUE;
    info.distance = t / det;
}


}