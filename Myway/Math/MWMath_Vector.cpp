#include "MWMath.h"
#include "MWMathSSE.h"

namespace Myway
{

/*
---------------------------------------------------------------------------------------
    minimum, maximum
---------------------------------------------------------------------------------------
*/
void Math::VecMinimum(Vec2 & v, const Vec2 & v1, const Vec2 & v2)
{
    v.x = Minimum(v1.x, v2.x);
    v.y = Minimum(v1.y, v2.y);
}

void Math::VecMinimum(Vec3 & v, const Vec3 & v1, const Vec3 & v2)
{
    v.x = Minimum(v1.x, v2.x);
    v.y = Minimum(v1.y, v2.y);
    v.z = Minimum(v1.z, v2.z);
}

void Math::VecMinimum(Vec4 & v, const Vec4 & v1, const Vec4 & v2)
{
    v.x = Minimum(v1.x, v2.x);
    v.y = Minimum(v1.y, v2.y);
    v.z = Minimum(v1.z, v2.z);
    v.w = Minimum(v1.w, v2.w);
}

void Math::VecMaximum(Vec2 & v, const Vec2 & v1, const Vec2 & v2)
{
    v.x = Maximum(v1.x, v2.x);
    v.y = Maximum(v1.y, v2.y);
}

void Math::VecMaximum(Vec3 & v, const Vec3 & v1, const Vec3 & v2)
{
    v.x = Maximum(v1.x, v2.x);
    v.y = Maximum(v1.y, v2.y);
    v.z = Maximum(v1.z, v2.z);
}

void Math::VecMaximum(Vec4 & v, const Vec4 & v1, const Vec4 & v2)
{
    v.x = Maximum(v1.x, v2.x);
    v.y = Maximum(v1.y, v2.y);
    v.z = Maximum(v1.z, v2.z);
    v.w = Maximum(v1.w, v2.w);
}


/*
------------------------------------------------------------------------------
    dot
------------------------------------------------------------------------------
*/
float Math::VecDot(const Vec2 & v1, const Vec2 & v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Math::VecDot(const Vec3 & v1, const Vec3 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Math::VecDot(const Vec4 & v1, const Vec4 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



/*
------------------------------------------------------------------------------
    cross
------------------------------------------------------------------------------
*/

void Math::VecCross(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2)
{
#ifdef WIN32
    /*
    *     x = y1 * z2 - z1 * y2
    *     y = z1 * x2 - x1 * z2
    *     z = x1 * y2 - y1 * x2
    */
    __asm
    {
        mov		esi,	v1;
        mov		edi,	v2;
        mov     edx,    vOut;
        movups	xmm0,	[esi];
        movups	xmm1,	[edi];
        movaps	xmm2,	xmm0;
        movaps	xmm3,	xmm1;
        shufps	xmm0,	xmm0,	11001001b;
        shufps	xmm1,	xmm1,	11010010b;
        mulps	xmm0,	xmm1;
        shufps	xmm2,	xmm2,	11010010b;
        shufps	xmm3,	xmm3,	11001001b;
        mulps	xmm2,	xmm3;
        subps	xmm0,	xmm2;

        movlps	[edx],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	8[edx], xmm0;
    }
#else
	Vec3 vTmp;
	vTmp.x = v1.y * v2.z - v1.z * v2.y;
	vTmp.y = v1.z * v2.x - v1.x * v2.z;
	vTmp.z = v1.x * v2.y - v1.y * v2.x;
	vOut = vTmp;
#endif
}

void Math::VecCross(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2)
{
#ifdef WIN32
    /*
    *     x = y1 * z2 - z1 * y2
    *     y = z1 * x2 - x1 * z2
    *     z = x1 * y2 - y1 * x2
    */
    __asm
    {
        mov		esi,	v1;
        mov		edi,	v2;
        mov     edx,    vOut;
        movups	xmm0,	[esi];
        movups	xmm1,	[edi];
        movaps	xmm2,	xmm0;
        movaps	xmm3,	xmm1;
        shufps	xmm0,	xmm0,	11001001b;
        shufps	xmm1,	xmm1,	11010010b;
        mulps	xmm0,	xmm1;
        shufps	xmm2,	xmm2,	11010010b;
        shufps	xmm3,	xmm3,	11001001b;
        mulps	xmm2,	xmm3;
        subps	xmm0,	xmm2;
        movss	xmm0,	Math::FLOAT_1;
        movups	[edx],	xmm0;
    }
#else
	/*
	*     x = y1 * z2 - z1 * y2
	*     y = z1 * x2 - x1 * z2
	*     z = x1 * y2 - y1 * x2
	*/
	Vec4 vTmp;
	vTmp.x = v1.y * v2.z - v1.z * v2.y;
	vTmp.y = v1.z * v2.x - v1.x * v2.z;
	vTmp.z = v1.x * v2.y - v1.y * v2.x;
	vTmp.w = 1.0f;
	vOut = vTmp;
#endif
}


/*
------------------------------------------------------------------------------
    length
------------------------------------------------------------------------------
*/
float Math::VecLength(const Vec2 & v)
{
	return Math::Sqrt(v.x * v.x + v.y * v.y);
}

float Math::VecLength(const Vec3 & v)
{
	return Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Math::VecLength(const Vec4 & v)
{
	return Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
------------------------------------------------------------------------------
    inverse length
------------------------------------------------------------------------------
*/
float Math::VecInvLength(const Vec2 & v)
{
	return 1.0f / Math::Sqrt(v.x * v.x + v.y * v.y);
}

float Math::VecInvLength(const Vec3 & v)
{
	return 1.0f / Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Math::VecInvLength(const Vec4 & v)
{
	return 1.0f / Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}



/*
------------------------------------------------------------------------------
    length square
------------------------------------------------------------------------------
*/
float Math::VecLengthSq(const Vec2 & v)
{
	return v.x * v.x + v.y * v.y;
}

float Math::VecLengthSq(const Vec3 & v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Math::VecLengthSq(const Vec4 & v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}




/*
------------------------------------------------------------------------------
    distance
------------------------------------------------------------------------------
*/
float Math::VecDistance(const Vec2 & v1, const Vec2 & v2)
{
    Vec2 v = v2 - v1;
    return Math::Sqrt(v.x * v.x + v.y * v.y);
}

float Math::VecDistance(const Vec3 & v1, const Vec3 & v2)
{
    Vec3 v = v2 - v1;
    return Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Math::VecDistance(const Vec4 & v1, const Vec4 & v2)
{
    Vec4 v = v2 - v1;
    return Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}



/*
------------------------------------------------------------------------------
    distance square
------------------------------------------------------------------------------
*/
float Math::VecDistanceSq(const Vec2 & v1, const Vec2 & v2)
{
    Vec2 v = v2 - v1;
    return v.x * v.x + v.y * v.y;
}

float Math::VecDistanceSq(const Vec3 & v1, const Vec3 & v2)
{
    Vec3 v = v2 - v1;
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Math::VecDistanceSq(const Vec4 & v1, const Vec4 & v2)
{
    Vec4 v = v2 - v1;
    return v.x * v.x + v.y * v.y + v.z * v.z;
}



/*
-------------------------------------------------------------------------
    vector reflect
-------------------------------------------------------------------------
*/
void Math::VecReflect(Vec2 & v, const Vec2 & i, const Vec2 & n)
{
    v = i - 2.0 * n * VecDot(n, i);
}

void Math::VecReflect(Vec3 & v, const Vec3 & i, const Vec3 & n)
{
    v = i - 2.0 * n * VecDot(n, i);
}

void Math::VecReflect(Vec4 & v, const Vec4 & i, const Vec4 & n)
{
    v = i - 2.0 * n * VecDot(n, i);
    v.w = 1.0f;
}

void Math::VecReflect(Vec3 & po, const Vec3 & pi, const Plane & p)
{
    float d = VecDot(p.n, pi) + p.d;
    po = pi - 2 * d * p.n;
}

void Math::VecReflect(Vec4 & po, const Vec4 & pi, const Plane & p)
{
    Vec3 i(pi.x, pi.y, pi.z);
    float d = VecDot(p.n, i) + p.d;
    i = i - 2 * d * p.n;

    po.x = i.x;
    po.x = i.y;
    po.x = i.z;
    po.w = 1.0f;
}

/*
-------------------------------------------------------------------------
    vector refract
-------------------------------------------------------------------------
*/
void Math::VecRefract(Vec2 & v, const Vec2 & i, const Vec2 & n, float eta)
{
    float cosi = VecDot(-i, n);
    float cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
    Vec2 t = eta * i + ((eta * cosi - Math::Sqrt(Math::Abs(cost2))) * n);
    v = cost2 > 0 ? t : Vec2::Zero;
}

void Math::VecRefract(Vec3 & v, const Vec3 & i, const Vec3 & n, float eta)
{
    float cosi = VecDot(-i, n);
    float cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
    Vec3 t = eta * i + ((eta * cosi - Math::Sqrt(Math::Abs(cost2))) * n);
    v = cost2 > 0 ? t : Vec3::Zero;
}

void Math::VecRefract(Vec4 & v, const Vec4 & i, const Vec4 & n, float eta)
{
    float cosi = VecDot(-i, n);
    float cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
    Vec4 t = eta * i + ((eta * cosi - Math::Sqrt(Math::Abs(cost2))) * n);
    v = cost2 > 0 ? t : Vec4::Zero;
}





/*
----------------------------------------------------------------------------------------
   cross and normalize
----------------------------------------------------------------------------------------
*/

void Math::VecCrossN(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2)
{
#if 0
    /*
    *     x = y1 * z2 - z1 * y2
    *     y = z1 * x2 - x1 * z2
    *     z = x1 * y2 - y1 * x2
    */
    __asm
    {
        mov		esi,	v1;
        mov		edi,	v2;
        mov     edx,    vOut;
        movups	xmm0,	[esi];
        movups	xmm1,	[edi];
        movaps	xmm2,	xmm0;
        movaps	xmm3,	xmm1;
        shufps	xmm0,	xmm0,	11001001b;
        shufps	xmm1,	xmm1,	11010010b;
        mulps	xmm0,	xmm1;
        shufps	xmm2,	xmm2,	11010010b;
        shufps	xmm3,	xmm3,	11001001b;
        mulps	xmm2,	xmm3;
        subps	xmm0,	xmm2;

        movaps  xmm1,   xmm0;
        mulps   xmm1,   xmm0;
        movaps  xmm3,   xmm1;
        movaps  xmm2,   xmm1;
        shufps  xmm3,   xmm3,   SHUFFLE_PS(2, 2, 2, 2);
        shufps  xmm2,   xmm2,   SHUFFLE_PS(1, 1, 1, 1);
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);
        addps   xmm2,   xmm3;
        addps   xmm1,   xmm2;
        rsqrtps xmm1,   xmm1;
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);
        mulps   xmm0,   xmm1;

        movlps	[edx],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	8[edx], xmm0;
    }
#else
    Vec3 v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;

    float len = Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    len = 1.0f / len;

    vOut.x = v.x * len;
    vOut.y = v.y * len;
    vOut.z = v.z * len;
#endif
}

void Math::VecCrossN(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2)
{
#ifdef WIN32
    /*
    *     x = y1 * z2 - z1 * y2
    *     y = z1 * x2 - x1 * z2
    *     z = x1 * y2 - y1 * x2
    */
    __asm
    {
        mov		esi,	v1;
        mov		edi,	v2;
        mov     edx,    vOut;
        movups	xmm0,	[esi];
        movups	xmm1,	[edi];
        movaps	xmm2,	xmm0;
        movaps	xmm3,	xmm1;
        shufps	xmm0,	xmm0,	11001001b;
        shufps	xmm1,	xmm1,	11010010b;
        mulps	xmm0,	xmm1;
        shufps	xmm2,	xmm2,	11010010b;
        shufps	xmm3,	xmm3,	11001001b;
        mulps	xmm2,	xmm3;
        subps	xmm0,	xmm2;

        movaps  xmm1,   xmm0;
        mulps   xmm1,   xmm0;
        movaps  xmm3,   xmm1;
        movaps  xmm2,   xmm1;
        shufps  xmm3,   xmm3,   SHUFFLE_PS(2, 2, 2, 2);
        shufps  xmm2,   xmm2,   SHUFFLE_PS(1, 1, 1, 1);
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);
        addps   xmm2,   xmm3;
        addps   xmm1,   xmm2;
        rsqrtps xmm1,   xmm1;
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);
        mulps   xmm0,   xmm1;

        movss	xmm0,	Math::FLOAT_1;
        movups	[edx],	xmm0;
    }
#else
    Vec4 v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;

    float len = Math::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    len = 1.0f / len;

    vOut.x = v.x * len;
    vOut.y = v.y * len;
    vOut.z = v.z * len;
    vOut.w = 1.0f;
#endif
}




/*
----------------------------------------------------------------------------------------
    normalize
----------------------------------------------------------------------------------------
*/
void Math::VecNormalize(Vec2 & vOut, const Vec2 & v)
{
    float inv_len = VecInvLength(v);
    vOut.x = v.x * inv_len;
    vOut.y = v.y * inv_len;
}

void Math::VecNormalize(Vec3 & vOut, const Vec3 & v)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov     edx,    vOut;

        movups  xmm0,   [esi];
        movaps  xmm1,   xmm0;
        mulps   xmm1,   xmm0;
        movaps  xmm3,   xmm1;
        movaps  xmm2,   xmm1;

        shufps  xmm3,   xmm3,   SHUFFLE_PS(2, 2, 2, 2);
        shufps  xmm2,   xmm2,   SHUFFLE_PS(1, 1, 1, 1);
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);

        addps   xmm2,   xmm3;
        addps   xmm1,   xmm2;

        rsqrtps xmm1,   xmm1;
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);
        mulps   xmm0,   xmm1;

        movlps	[edx],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	8[edx], xmm0;
    }
#else
	float inv_len = VecInvLength(v);
	vOut.x = v.x * inv_len;
	vOut.y = v.y * inv_len;
	vOut.z = v.z * inv_len;
#endif
}

void Math::VecNormalize(Vec4 & vOut, const Vec4 & v)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov     edx,    vOut;

        movups  xmm0,   [esi];
        movaps  xmm1,   xmm0;
        mulps   xmm1,   xmm0;

        movaps  xmm3,   xmm1;
        movaps  xmm2,   xmm1;

        shufps  xmm3,   xmm3,   SHUFFLE_PS(2, 2, 2, 2);
        shufps  xmm2,   xmm2,   SHUFFLE_PS(1, 1, 1, 1);
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);

        addps   xmm2,   xmm3;
        addps   xmm1,   xmm2;

        rsqrtps xmm1,   xmm1;
        shufps  xmm1,   xmm1,   SHUFFLE_PS(0, 0, 0, 0);
        mulps   xmm0,   xmm1;

        movss	xmm0,	Math::FLOAT_1;
        movups	[edx],	xmm0;
    }
#else
	float inv_len = VecInvLength(v);
	vOut.x = v.x * inv_len;
	vOut.y = v.y * inv_len;
	vOut.z = v.z * inv_len;
	vOut.w = 1.0f;
#endif
}



/*
----------------------------------------------------------------------------------------
    transform normal
----------------------------------------------------------------------------------------
*/
void Math::VecTransformN(Vec2 & vOut, const Vec2 & v, const Mat3 & mat)
{
	Vec2 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21;
	vTmp.y = v.x * mat._21 + v.y * mat._22;
	vOut = vTmp;
}

void Math::VecTransformN(Vec3 & vOut, const Vec3 & v, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm0,	xmm2;

        movlps	[edx],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edx + 8], xmm0;
    }
#else
	Vec3 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31;
	vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32;
	vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33;
	vOut = vTmp;
#endif
}

void Math::VecTransformN(Vec4 & vOut, const Vec3 & v, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,   0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm0,	xmm2;

        movss	xmm0,	Math::FLOAT_1;
        movups	[edx], xmm0;
    }
#else
	Vec4 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31;
	vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32;
	vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33;
	vTmp.w = 1.0f;
	vOut = vTmp;
#endif
}

void Math::VecTransformN(Vec4 &vOut, const Vec4 & v, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];

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
        addps	xmm0,	xmm2;

        movss	xmm0,	Math::FLOAT_1;
        movups	[edx], xmm0;
    }
#else
	Vec4 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31;
	vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32;
	vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33;
	vTmp.w = v.w;
	vOut = vTmp;
#endif
}





/*
----------------------------------------------------------------------------------------
    transform
----------------------------------------------------------------------------------------
*/
void Math::VecTransform(Vec2 & vOut, const Vec2 & v, const Mat3 & mat)
{
	Vec2 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21 + mat._31;
    vTmp.y = v.x * mat._12 + v.y * mat._22 + mat._32;
    vTmp  /= v.x * mat._13 + v.y * mat._23 + mat._33;
	vOut = vTmp;
}

void Math::VecTransformX(float & x, const Vec3 & v, const Mat4 & mat)
{
    x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + mat._41;
    x /= v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44;
}

void Math::VecTransformY(float & y, const Vec3 & v, const Mat4 & mat)
{
    y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + mat._42;
    y /= v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44;
}

void Math::VecTransformZ(float & z, const Vec3 & v, const Mat4 & mat)
{
    z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + mat._43;
    z /= v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44;
}

void Math::VecTransform(Vec3 & vOut, const Vec3 & v, const Mat4 & mat)
{
#ifdef WIN32
    /*
    x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + v.w * mat._41;
    y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + v.w * mat._42;
    z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + v.w * mat._43;
    w = v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + v.w * mat._44;
    */
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];
        movups	xmm7,	[edi + 48];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        //v.xyz = v.xyz / v.w
        movaps  xmm1,   xmm0;
        shufps  xmm1,   xmm1,   SHUFFLE_PS(3, 3, 3, 3);

        divps   xmm0,   xmm1;

        //copy xy
        movlps	[edx],	xmm0;
        //copy z
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edx + 8], xmm0;
    }

#else
    Vec3 vTmp;
    vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + mat._41;
    vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + mat._42;
    vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + mat._43;
    vTmp /=  v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44;
    vOut = vTmp;
#endif
}

void Math::VecTransform(Vec4 & vOut, const Vec3 & v, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];
        movups	xmm7,	[edi + 48];

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

        movups	[edx], xmm0;
    }
#else
	Vec4 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + mat._41;
	vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + mat._42;
	vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + mat._43;
	vTmp.w = v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44;
	vOut = vTmp;
#endif
}

void Math::VecTransform(Vec4 & vOut, const Vec4 & v, const Mat4 & mat)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];
        movups	xmm7,	[edi + 48];

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];
        movss	xmm3,	[esi + 12];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;
        shufps	xmm3,	xmm3,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;
        mulps	xmm3,	xmm7;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm3;
        addps	xmm0,	xmm2;

        movups	[edx], xmm0;
    }
#else
	Vec4 vTmp;
	vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + v.w * mat._41;
	vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + v.w * mat._42;
	vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + v.w * mat._43;
	vTmp.w = v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + v.w * mat._44;
	vOut = vTmp;
#endif
}



void Math::VecTransformN(Vec3 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	pIn;
        mov		edi,	pOut;
        mov     edx,    mat;
        mov     ecx,    iCount;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];

__again:
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm0,	xmm2;

        movlps	[edi],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edi + 8], xmm0;

        add     esi, 12;
        add     edi, 12;

        dec     ecx;
        jnz     __again;
    }
#else
    for (int i = 0; i < iCount; ++i)
    {
        VecTransformN(pOut[i], pIn[i], mat);
    }
#endif
}

void Math::VecTransformN(Vec4 * pOut, const Vec4 * pIn, const Mat4 & mat, int iCount)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	pIn;
        mov		edi,	pOut;
        mov     edx,    mat;
        mov     ecx,    iCount;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];

__again:
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm0,	xmm2;

        movss   xmm0,   Math::FLOAT_1;
        movups	[edi],  xmm0;

        add     esi, 12;
        add     edi, 12;

        dec     ecx;
        jnz     __again;
    }
#else
    for (int i = 0; i < iCount; ++i)
    {
        VecTransformN(pOut[i], pIn[i], mat);
    }
#endif
}

void Math::VecTransform(Vec3 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	pIn;
        mov		edi,	pOut;
        mov     edx,    mat;
        mov     ecx,    iCount;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];
        movups  xmm7,   [edx + 48]

__again:
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        //v.xyz = v.xyz / v.w
        movaps  xmm1,   xmm0;
        shufps  xmm1,   xmm1,   SHUFFLE_PS(3, 3, 3, 3);

        divps   xmm0,   xmm1;

        movlps	[edi],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edi + 8], xmm0;

        add     esi, 12;
        add     edi, 12;

        dec     ecx;
        jnz     __again;
    }
#else
    for (int i = 0; i < iCount; ++i)
    {
        VecTransform(pOut[i], pIn[i], mat);
    }
#endif
}

void Math::VecTransform(Vec4 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	pIn;
        mov		edi,	pOut;
        mov     edx,    mat;
        mov     ecx,    iCount;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];
        movups  xmm7,   [edx + 48]

__again:
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movups	[edi],  xmm0;

        add     esi, 12;
        add     edi, 16;

        dec     ecx;
        jnz     __again;
    }
#else
    for (int i = 0; i < iCount; ++i)
    {
        VecTransform(pOut[i], pIn[i], mat);
    }
#endif
}

void Math::VecTransform(Vec4 * pOut, const Vec4 * pIn, const Mat4 & mat, int iCount)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	pIn;
        mov		edi,	pOut;
        mov     edx,    mat;
        mov     ecx,    iCount;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];
        movups  xmm7,   [edx + 48]

__again:
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];
        movss	xmm3,	[esi + 12];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;
        shufps	xmm3,	xmm3,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;
        mulps	xmm3,	xmm7;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm3;
        addps	xmm0,	xmm2;

        movups	[edi],  xmm0;

        add     esi, 16;
        add     edi, 16;

        dec     ecx;
        jnz     __again;
    }
#else
    for (int i = 0; i < iCount; ++i)
    {
        VecTransform(pOut[i], pIn[i], mat);
    }
#endif
}



/*
--------------------------------------------------------------------------------------
    transform affine
--------------------------------------------------------------------------------------
*/
void Math::VecTransformAffine(Vec3 & vOut, const Vec3 & v, const Mat4 & mat)
{
#ifdef WIN32
    /*
    x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + v.w * mat._41;
    y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + v.w * mat._42;
    z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + v.w * mat._43;
    */
    __asm
    {
        mov		esi,	v;
        mov		edi,	mat;
        mov     edx,    vOut;

        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        movups	xmm4,	[edi];
        movups	xmm5,	[edi + 16];
        movups	xmm6,	[edi + 32];
        movups	xmm7,	[edi + 48];

        shufps	xmm0,	xmm0,	0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        //copy xy
        movlps	[edx],	xmm0;
        //copy z
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edx + 8], xmm0;
    }
#else
    Vec3 vTmp;
    vTmp.x = v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + mat._41;
    vTmp.y = v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + mat._42;
    vTmp.z = v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + mat._43;
    vOut = vTmp;
#endif
}

void Math::VecTransformAffine(Vec3 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	pIn;
        mov		edi,	pOut;
        mov     edx,    mat;
        mov     ecx,    iCount;

        movups	xmm4,	[edx];
        movups	xmm5,	[edx + 16];
        movups	xmm6,	[edx + 32];
        movups  xmm7,   [edx + 48]

__again:
        movss	xmm0,	[esi];
        movss	xmm1,	[esi + 4];
        movss	xmm2,	[esi + 8];

        shufps	xmm0,	xmm0,   0;
        shufps	xmm1,	xmm1,	0;
        shufps	xmm2,	xmm2,	0;

        mulps	xmm0,	xmm4;
        mulps	xmm1,	xmm5;
        mulps	xmm2,	xmm6;

        addps	xmm0,	xmm1;
        addps	xmm2,	xmm7;
        addps	xmm0,	xmm2;

        movlps	[edi],	xmm0;
        shufps	xmm0,	xmm0,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edi + 8], xmm0;

        add     esi, 12;
        add     edi, 12;

        dec     ecx;
        jnz     __again;
    }
#else
    for (int i = 0; i < iCount; ++i)
    {
        VecTransformAffine(pOut[i], pIn[i], mat);
    }
#endif
}




/*
------------------------------------------------------------------------
    lerp
------------------------------------------------------------------------
*/
void Math::VecLerp(Vec2 & vOut, const Vec2 & v1, const Vec2 & v2, float t)
{
    float _1_t = 1.0f - t;
    vOut.x = v1.x * _1_t + v2.x * t;
    vOut.y = v1.y * _1_t + v2.y * t;
}

void Math::VecLerp(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2, float t)
{
    vOut.x = v1.x + (v2.x - v1.x) * t;
    vOut.y = v1.y + (v2.y - v1.y) * t;
    vOut.z = v1.z + (v2.z - v1.z) * t;
}

void Math::VecLerp(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2, float t)
{
#ifdef WIN32
    __asm 
    {
        mov		esi,	v1;
        mov		edi,	v2;
        mov     edx,    vOut;

        movups  xmm0,   [esi];
        movups  xmm1,   [edi];
        movss   xmm2,   t;
        subps   xmm1,   xmm0;
        shufps  xmm2,   xmm2,   0;
        mulps   xmm1,   xmm2;
        addps   xmm0,   xmm1;

        movups [edx],	xmm0;
    }
#else 
    vOut.x = v1.x + (v2.x - v1.x) * t;
    vOut.y = v1.y + (v2.y - v1.y) * t;
    vOut.z = v1.z + (v2.z - v1.z) * t;
    vOut.w = v1.w + (v2.w - v1.w) * t;
#endif 
}



}