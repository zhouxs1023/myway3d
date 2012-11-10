#include "MWMath.h"
#include "MWMathSSE.h"

namespace Myway
{

/* :) AABB
-------------------------------------------------------------
-------------------------------------------------------------
*/
float Math::AABBVolume(const Aabb & aabb)
{
    return aabb.GetWidth() * aabb.GetHeight() * aabb.GetDepth();
}

bool Math::AABBInside(const Aabb & dest, const Aabb & src)
{
    return (src.minimum.x >= dest.minimum.x && src.maximum.x <= dest.maximum.x) &&
           (src.minimum.y >= dest.minimum.y && src.maximum.y <= dest.maximum.y) &&
           (src.minimum.z >= dest.minimum.z && src.maximum.z <= dest.maximum.z);
}

void Math::AABBMerge(Aabb & aabbOut, const Aabb & aabb1, const Aabb & aabb2)
{
    aabbOut.minimum.x = Math::Minimum(aabb1.minimum.x, aabb2.minimum.x);
    aabbOut.minimum.y = Math::Minimum(aabb1.minimum.y, aabb2.minimum.y);
    aabbOut.minimum.z = Math::Minimum(aabb1.minimum.z, aabb2.minimum.z);
    aabbOut.maximum.x = Math::Maximum(aabb1.maximum.x, aabb2.maximum.x);
    aabbOut.maximum.y = Math::Maximum(aabb1.maximum.y, aabb2.maximum.y);
    aabbOut.maximum.z = Math::Maximum(aabb1.maximum.z, aabb2.maximum.z);
}

void Math::AABBMerge(Aabb & aabbOut, const Aabb & aabb1, const Vec3 & vPoint)
{
    aabbOut.minimum.x = Math::Minimum(aabb1.minimum.x, vPoint.x);
    aabbOut.minimum.y = Math::Minimum(aabb1.minimum.y, vPoint.y);
    aabbOut.minimum.z = Math::Minimum(aabb1.minimum.z, vPoint.z);
    aabbOut.maximum.x = Math::Maximum(aabb1.maximum.x, vPoint.x);
    aabbOut.maximum.y = Math::Maximum(aabb1.maximum.y, vPoint.y);
    aabbOut.maximum.z = Math::Maximum(aabb1.maximum.z, vPoint.z);
}

#pragma warning(push)
#pragma warning(disable : 4731)

void Math::AABBTransform(Aabb & aabbOut, const Aabb & aabbIn, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        push ebp;

        mov edi, aabbOut;
        mov esi, aabbIn;
        mov edx, mat;

        /*
        declaration variables
        116 bytes: 8 points(4floats), 4 floats, 16 bytes aligned 
        */
        sub esp, 160;
        mov ebp, esp;

        /*
        16 bytes align
        */
        add ebp, 16;
        and ebp, 0xFFFFFFF0;

        mov 128[ebp], edi;
        mov 132[ebp], edx;

        mov eax, [esi];    //min.x
        mov ebx, 4[esi];   //min.y
        mov ecx, 8[esi];   //min.z
        mov edx, 12[esi];  //max.x
        mov edi, 20[esi];  //max.z
        mov esi, 16[esi];  //max.y

        /*
        left bottom fornt point
        min.x, min.y, min.z
        */
        mov [ebp],  eax;
        mov 4[ebp], ebx;
        mov 8[ebp], ecx;

        /*
        left bottom back point
        min.x, min.y, max.z
        */
        mov 16[ebp], eax;
        mov 20[ebp], ebx;
        mov 24[ebp], edi;

        /*
        left top front point.
        min.x, max.y, min.z
        */
        mov 32[ebp], eax;
        mov 36[ebp], esi;
        mov 40[ebp], ecx;

        /*
        left top back point.
        min.x, max.y, max.z
        */
        mov 48[ebp], eax;
        mov 52[ebp], esi;
        mov 56[ebp], edi;

        /*
        right bottom fornt point
        max.x, min.y, min.z
        */
        mov 64[ebp], edx;
        mov 68[ebp], ebx;
        mov 72[ebp], ecx;


        /*
        right bottom fornt point
        max.x, min.y, max.z
        */
        mov 80[ebp], edx;
        mov 84[ebp], ebx;
        mov 88[ebp], edi;


        /*
        right top front point.
        max.x, max.y, min.z
        */
        mov 96[ebp], edx;
        mov 100[ebp], esi;
        mov 104[ebp], ecx;


        /*
        right top back point.
        max.x, max.y, max.z
        */
        mov 112[ebp], edx;
        mov 116[ebp], esi;
        mov 120[ebp], edi;

        mov edi, 128[ebp];      //out
        mov edx, 132[ebp];      //mat

        mov esi, ebp;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];
        movups	xmm7,	[edx + 48];

        //transfrom point
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;

        add     esi,    16;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movaps	[esi], xmm0;


        // recalc aabb
        mov     esi,    ebp;

        movaps  xmm0,   [esi];      //min
        movaps  xmm1,   [esi];      //max

        movaps  xmm2,   [esi + 16];
        movaps  xmm3,   [esi + 32];

        minps   xmm0,   xmm2;
        maxps   xmm1,   xmm2;
        minps   xmm0,   xmm3;
        maxps   xmm1,   xmm3;

        movaps  xmm4,   [esi + 48];
        movaps  xmm5,   [esi + 64];

        minps   xmm0,   xmm4;
        maxps   xmm1,   xmm4;
        minps   xmm0,   xmm5;
        maxps   xmm1,   xmm5;

        movaps  xmm2,   [esi + 80];
        movaps  xmm3,   [esi + 96];
        minps   xmm4,   [esi + 112];

        minps   xmm0,   xmm2;
        maxps   xmm1,   xmm2;
        minps   xmm0,   xmm3;
        maxps   xmm1,   xmm3;
        minps   xmm0,   xmm4;
        maxps   xmm1,   xmm4;

        movlps	[edi],	xmm0;
        movlps	[edi + 12],	xmm1;

        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        shufps	xmm1,	xmm1,	SHUFFLE_PS(2, 2, 2, 2);

        movss	[edi + 8], xmm0;
        movss	[edi + 20], xmm1;

        add esp, 160;
        pop ebp;
    }
#else
    Vec3 p[8];

    p[0] = aabbOut.GetLeftBottomFrontPoint();
    p[1] = aabbOut.GetLeftTopFrontPoint();
    p[2] = aabbOut.GetRightBottomFrontPoint();
    p[3] = aabbOut.GetRightTopFrontPoint();
    p[4] = aabbOut.GetLeftBottomBackPoint();
    p[5] = aabbOut.GetLeftTopBackPoint();
    p[6] = aabbOut.GetRightBottomBackPoint();
    p[7] = aabbOut.GetRightTopBackPoint();

    Math::VecTransform(p, p, mat, 8);

    aabbOut.minimum = p[0];
    aabbOut.maximum = p[0];

    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[1]);
    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[2]);
    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[3]);
    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[4]);
    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[5]);
    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[6]);
    Math::VecMinimum(aabbOut.mi, aabbOut.mi, p[7]);

    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[1]);
    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[2]);
    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[3]);
    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[4]);
    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[5]);
    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[6]);
    Math::VecMaximum(aabbOut.ma, aabbOut.ma, p[7]);
#endif
}

#pragma warning(pop)


int Math::AABBIntersection(Aabb & aabbOut, const Aabb & aabb1, const Aabb & aabb2)
{
    aabbOut.minimum.x = Math::Maximum(aabb1.minimum.x, aabb2.minimum.x);
    aabbOut.minimum.y = Math::Maximum(aabb1.minimum.y, aabb2.minimum.y);
    aabbOut.minimum.z = Math::Maximum(aabb1.minimum.z, aabb2.minimum.z);

    aabbOut.maximum.x = Math::Minimum(aabb1.maximum.x, aabb2.maximum.x);
    aabbOut.maximum.x = Math::Minimum(aabb1.maximum.y, aabb2.maximum.y);
    aabbOut.maximum.x = Math::Minimum(aabb1.maximum.z, aabb2.maximum.z);

    if (aabbOut.minimum.x >= aabbOut.maximum.x ||
        aabbOut.minimum.y >= aabbOut.maximum.y ||
        aabbOut.minimum.z >= aabbOut.maximum.z)
        return -1;

    return 0;
}

float Math::AABBIntersectionVolume(const Aabb & aabb1, const Aabb & aabb2)
{
    Aabb aabb;

    aabb.minimum.x = Math::Maximum(aabb1.minimum.x, aabb2.minimum.x);
    aabb.minimum.y = Math::Maximum(aabb1.minimum.y, aabb2.minimum.y);
    aabb.minimum.z = Math::Maximum(aabb1.minimum.z, aabb2.minimum.z);

    aabb.maximum.x = Math::Minimum(aabb1.maximum.x, aabb2.maximum.x);
    aabb.maximum.y = Math::Minimum(aabb1.maximum.y, aabb2.maximum.y);
    aabb.maximum.z = Math::Minimum(aabb1.maximum.z, aabb2.maximum.z);

    return aabb.GetWidth() * aabb.GetHeight() * aabb.GetDepth();
}

int Math::AABBIntersection(const Aabb & aabb1, const Aabb & aabb2)
{
    Aabb aabb;

    aabb.minimum.x = Math::Maximum(aabb1.minimum.x, aabb2.minimum.x);
    aabb.minimum.y = Math::Maximum(aabb1.minimum.y, aabb2.minimum.y);
    aabb.minimum.z = Math::Maximum(aabb1.minimum.z, aabb2.minimum.z);

    aabb.maximum.x = Math::Minimum(aabb1.maximum.x, aabb2.maximum.x);
    aabb.maximum.y = Math::Minimum(aabb1.maximum.y, aabb2.maximum.y);
    aabb.maximum.z = Math::Minimum(aabb1.maximum.z, aabb2.maximum.z);

    return aabb.maximum > aabb.minimum ? 0 : -1;
}

int Math::AABBIntersection(const Aabb & aabb, const Vec3 & vPoint)
{
    if (vPoint.x >= aabb.minimum.x &&
        vPoint.x <= aabb.maximum.x &&

        vPoint.y >= aabb.minimum.y &&
        vPoint.y <= aabb.maximum.y &&

        vPoint.z >= aabb.minimum.z && 
        vPoint.z <= aabb.maximum.z)
        return 0;

    return -1;
}

int Math::AABBIntersection(const Aabb & aabb, const Sphere& sph)
{
    float radius_sq  = sph.radius * sph.radius;
    if (VecDistanceSq(sph.center, aabb.GetLeftBottomFrontPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetLeftBottomBackPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetLeftTopFrontPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetLeftTopBackPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetRightBottomFrontPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetRightBottomBackPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetRightTopFrontPoint()) < radius_sq ||
        VecDistanceSq(sph.center, aabb.GetRightTopBackPoint()) < radius_sq)
        return 0;

    return -1;
}


}