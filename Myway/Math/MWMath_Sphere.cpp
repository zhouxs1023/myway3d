#include "MWMath.h"
#include "MWMathSSE.h"

namespace Myway
{

float Math::SphereVolume(const Sphere& sph)
{
    return (4.0f / 3.0f) *  Math::PI_1 * sph.radius * sph.radius * sph.radius;
}

void Math::SphereMerge(Sphere& sphOut, const Sphere& sph1, const Sphere& sph2)
{
    sphOut.center = (sph1.center + sph2.center) * 0.5f;
    sphOut.radius = VecDistance(sphOut.center, sph1.center) + sph1.radius;
}

void Math::SphereMerge(Sphere& sphOut, const Sphere& sphIn, const Vec3 & vPoint)
{
    float fdist = VecDistance(vPoint, sphIn.center);
    sphOut.center = sphIn.center;
    sphOut.radius = (sphIn.radius < fdist) ?  fdist : sphIn.radius; 
}

void Math::SphereTransform(Sphere& sphOut, const Sphere& sphIn, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        mov esi, sphIn;
        mov edi, sphOut;
        mov edx, mat;

        movups xmm4, [edx];
        movups xmm5, 16[edx];
        movups xmm6, 32[edx];
        movups xmm7, 48[edx];

        /*
        transform center
        */
        movss xmm0, [esi];
        shufps xmm0, xmm0, 0;
        mulps xmm0, xmm4;

        movss xmm1, 4[esi];
        shufps xmm1, xmm1, 0;
        mulps xmm1, xmm5;

        movss xmm2, 8[esi];
        shufps xmm2, xmm2, 0;
        mulps xmm2, xmm6;

        addps xmm0, xmm1;
        addps xmm2, xmm7;
        addps xmm0, xmm2;

        /*
        copy radius
        */
        movss xmm0, 12[esi];

        /*
        copy data to out
        */
        movups [edi], xmm0;
    }
#else 
    sphOut.radius = sphIn.radius;
    VecTransform(sphOut.center, sphIn.center, mat);
#endif
}

int Math::SphereIntersection(const Sphere& sph1, const Sphere& sph2)
{
    float len = (sph1.radius + sph2.radius);
    len *= len;
    return (VecDistanceSq(sph1.center, sph2.center) <= len) ? 0 : -1;
}

int Math::SphereIntersection(const Sphere& sph, const Vec3 & vPoint)
{
    return (VecDistanceSq(sph.center, vPoint) >
            sph.radius * sph.radius) ? -1 : 0;
}

int Math::SphereIntersection(const Sphere& sph, const Aabb & aabb)
{
    return AABBIntersection(aabb, sph);
}

}