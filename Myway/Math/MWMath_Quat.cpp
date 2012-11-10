#include "MWMath.h"
#include "MWMathSSE.h"

namespace Myway
{

/* : QuatDot
------------------------------------------------------------
    @Remark:
        四元数点积.
------------------------------------------------------------
*/
float Math::QuatDot(const Quat & q1, const Quat & q2)
{
    return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}


/* : QuatLength
------------------------------------------------------------
    @Remark:
        四元数长度.
------------------------------------------------------------
*/
float Math::QuatLength(const Quat & q)
{
    return Math::Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}


/* : QuatNormalize
------------------------------------------------------------
    @Remark:
        四元数单位化.
------------------------------------------------------------
*/
void Math::QuatNormalize(Quat & qOut, const Quat & q)
{
    float len = Math::Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    assert(Math::Abs(len) > DEFAULT_EPSILON);
    len = 1.0f / len;
    qOut.x = q.x * len;
    qOut.y = q.y * len;
    qOut.z = q.z * len;
    qOut.w = q.w * len;
}


/* : QuatConjugate
------------------------------------------------------------
    @Remark:
        求共轭四元数.
------------------------------------------------------------
*/
void Math::QuatConjugate(Quat & qOut, const Quat & q)
{
    qOut.x = -q.x;
    qOut.y = -q.y;
    qOut.z = -q.z;
    qOut.w = q.w;
}


/* : QuatInverse
------------------------------------------------------------
    @Remark:
        求共轭四元数.
------------------------------------------------------------
*/
void Math::QuatInverse(Quat & qOut, const Quat & q)
{
    /*
        q- = q* / |len|^2
    */
    float len2 = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

    d_assert (len2 > 0 && "quaternion can't inverse.");

    len2 = 1.0f / len2;

    qOut.x = -q.x * len2;
    qOut.y = -q.y * len2;
    qOut.z = -q.z * len2;
    qOut.w = q.w * len2;
}


/* : QuatFromAxis
------------------------------------------------------------
    @Remark:
        轴角转化成四元数.
------------------------------------------------------------
*/
void Math::QuatFromAxis(Quat & qOut, const Vec3 & vAxis, float rads)
{
    float half_rads = rads / 2.0f;
    float cosine = Math::Cos(half_rads);
    float sine = Math::Sin(half_rads);
    
    qOut.x = vAxis.x * sine;
    qOut.y = vAxis.y * sine;
    qOut.z = vAxis.z * sine;
    qOut.w = cosine;
}

void Math::QuatFromAxis(Quat & q, const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis)
{
    Mat4 m;

    m._11 = xAxis.x, m._12 = xAxis.y, m._13 = xAxis.z;
    m._21 = yAxis.x, m._22 = yAxis.y, m._23 = yAxis.z;
    m._31 = zAxis.x, m._32 = zAxis.y, m._33 = zAxis.z;

    QuatFromMatrix(q, m);
}

void Math::QuatFromDir(Quat & qOut, const Vec3 & dir1, const Vec3 & dir2, const Vec3 & fallbackAxis, bool normalize)
{
    Vec3 d1 = dir1;
    Vec3 d2 = dir2;

    if (normalize)
    {
        VecNormalize(d1, d1);
        VecNormalize(d2, d2);
    }

    float d = VecDot(d1, d2);

    if (d >= 1.0f)
    {
        qOut = Quat::Identity;
    }
    else if (d < (1e-6f - 1.0f))
    {
        if (fallbackAxis != Vec3::Zero)
        {
            // rotate 180 degrees about the fallback axis
            Math::QuatFromAxis(qOut, fallbackAxis, Math::PI_1);
        }
        else
        {
            // Generate an axis
            Vec3 axis;
            Math::VecCross(axis, Vec3::UnitX, d1);

            if (Math::VecLengthSq(axis) < DEFAULT_EPSILON) // pick another if colinear
                Math::VecCross(axis, Vec3::UnitY, d1);

            VecNormalize(axis, axis);
            Math::QuatFromAxis(qOut, axis, Math::PI_1);
        }
    }
    else
    {
        float s = Math::Sqrt((1 + d) * 2);
        float invs = 1 / s;

        Vec3 c;
        Math::VecCross(c, d1, d2);

        qOut.x = c.x * invs;
        qOut.y = c.y * invs;
        qOut.z = c.z * invs;
        qOut.w = s * 0.5f;

        Math::QuatNormalize(qOut, qOut);
    }
}

/* : QuatFromPitchYawRoll
------------------------------------------------------------
    @Remark:
------------------------------------------------------------
*/
void Math::QuatFromPitchYawRoll(Quat & qOut, float pitch, float yaw, float roll)
{
#ifdef WIN32
    const float half_1 = 0.5f;
    float val[6];

    __asm
    {
        mov     edi,    qOut;

        fld     half_1;
        fmul    pitch;
        fstp    pitch;

        fld     half_1;
        fmul    yaw;
        fstp    yaw;

        fld     half_1;
        fmul    roll;
        fstp    roll;

        fld     roll;
        fsincos;
        fstp    [val];
        fstp    12[val];

        fld     yaw;
        fsincos;
        fstp    4[val];
        fstp    16[val];

        fld     roll;
        fsincos;
        fstp    8[val];
        fstp    20[val];

        movss   xmm0,   [val];
        movss   xmm1,   4[val];
        movss   xmm2,   8[val];
        movss   xmm3,   12[val];
        movss   xmm4,   16[val];
        movss   xmm5,   20[val];

        shufps  xmm0,   xmm0,   0;
        shufps  xmm1,   xmm1,   0;
        shufps  xmm2,   xmm2,   0;
        shufps  xmm3,   xmm3,   0;
        shufps  xmm4,   xmm5,   0;
        shufps  xmm5,   xmm6,   0;

        movss   xmm0,   12[val];
        movss   xmm1,   16[val];
        movss   xmm2,   20[val];
        movss   xmm3,   [val];
        movss   xmm4,   4[val];
        movss   xmm5,   8[val];

        shufps  xmm0,   xmm0,   SHUFFLE_PS(1, 1, 0, 1);
        shufps  xmm1,   xmm1,   SHUFFLE_PS(1, 0, 1, 1);
        //shufps  xmm2,   xmm2,   SHUFFLE_PS(0, 1, 1, 1);
        shufps  xmm3,   xmm3,   SHUFFLE_PS(1, 1, 0, 1);
        shufps  xmm4,   xmm4,   SHUFFLE_PS(1, 0, 1, 1);
        //shufps  xmm5,   xmm5,   SHUFFLE_PS(0, 1, 1, 1);

        mulps   xmm0,   xmm1;
        mulps   xmm3,   xmm4;

        mulps   xmm1,   xmm2;
        mulps   xmm3,   xmm5;

        movaps  xmm6,   xmm0;
        movaps  xmm7,   xmm3;

        subss   xmm6,   xmm7;

        addps   xmm0,   xmm3;
        movss   xmm0,   xmm6;

        movups  [edi], xmm0;
    }
#else
    float cos_z_2 = cosf(0.5f * roll);
    float cos_y_2 = cosf(0.5f * yaw);
    float cos_x_2 = cosf(0.5f * pitch);

    float sin_z_2 = sinf(0.5f * roll);
    float sin_y_2 = sinf(0.5f * yaw);
    float sin_x_2 = sinf(0.5f * pitch);

    qOut.w = cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2;
    qOut.x = cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2 * sin_y_2 * cos_x_2;
    qOut.y = cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2;
    qOut.z = sin_z_2 * cos_y_2 * cos_x_2 + cos_z_2 * sin_y_2 * sin_x_2;
#endif
}


void Math::QuatFromMatrix(Quat & q, const Mat4 & m)
{
    float s = 0.0f;
    float t = 0.0f;
    
    // check the diagonal
    if(m._11 + m._22 + m._33 > 0.0)
    {
        t = 1.0f + m._11 + m._22 + m._33;
        s = 0.5f / Math::Sqrt(t);

        q.w = t * s;
        q.x = (m._23 - m._32) * s;
        q.y = (m._31 - m._13) * s;
        q.z = (m._12 - m._21) * s;
    }

    // column 1
    else if(m._11 > m._22 && m._11 > m._33)
    {
        t = 1.0f + m._11 - m._22 - m._33;
        s = 0.5f / Math::Sqrt(t);

        q.x = t * s;
        q.y = (m._12 + m._21) * s;
        q.z = (m._13 + m._31) * s;
        q.w = (m._23 - m._32) * s;
    }

    // column 2
    else if( m._22 > m._33)
    {
        t = 1.0f + m._22 - m._11 - m._33;
        s = 0.5f / Math::Sqrt(t);

        q.y = t * s;
        q.x = (m._21 + m._12) * s;
        q.z = (m._32 + m._23) * s;
        q.w = (m._31 - m._13) * s;
    }

    // column 3
    else
    {
        t = 1.0f + m._33 - m._11 - m._22;
        s = 0.5f / Math::Sqrt(t);

        q.z = t * s;
        q.x = (m._31 + m._13) * s;
        q.y = (m._32 + m._23) * s;
        q.w = (m._21 - m._12) * s;
    }
}


/* : QuatSlerp
------------------------------------------------------------
    @Remark:
        四元数球形插值.
------------------------------------------------------------
*/
void Math::QuatSlerp(Quat & qOut, const Quat & q1, const Quat & q2, float t)
{
    /*
	*     Lerp(q1, q2, t) = (q1 * sin((1-t) * theta) + q2 * sin(t * theta)) /sin(theta)
	*/
	float cosine, t0, t1;
	Quat q2_t = q2;

	cosine = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	if (cosine < 0.0f)
	{
		cosine = -cosine;
		q2_t = -q2;
	}

    if (1.0f - cosine > Math::EPSILON_E4)
	{
        float theta = Math::ACos(cosine);
        float inv_sine = 1.0f / Math::Sin(theta);
		t0 = Math::Sin((1.0f - t) * theta) * inv_sine;
		t1 = Math::Sin(t * theta) * inv_sine;
	}
	else
	{
		t0 = 1.0f - t;
		t1 = t;
	}

	qOut = q1 * t0 + q2_t * t1;
}


/* : QuatMultiply
------------------------------------------------------------
    @Remark:
        四元数相乘.
------------------------------------------------------------
*/
void Math::QuatMultiply(Quat & quat, const Quat & q1, const Quat & q2)
{
#ifdef WIN32
    static MW_ALIGN16 Quat t1(1.0f, -1.0f, 1.0f, -1.0f);
    static MW_ALIGN16 Quat t2(1.0f, 1.0f, -1.0f, -1.0f);
    static MW_ALIGN16 Quat t3(-1.0f, 1.0f, 1.0f, -1.0f);

    __asm
    {
        mov esi, q1;
        mov edx, q2;
        mov edi, quat;

        movss   xmm0, 12[esi];      //w
        movss   xmm1, [esi];        //x
        movss   xmm2, 4[esi];       //y
        movss   xmm3, 8[esi];       //z

        movups  xmm4, [edx];
        movaps  xmm5, xmm4;
        movaps  xmm6, xmm4;
        movaps  xmm7, xmm4;

        shufps  xmm0, xmm0, SHUFFLE_PS(0, 0, 0, 0);
        shufps  xmm1, xmm1, SHUFFLE_PS(0, 0, 0, 0);
        shufps  xmm2, xmm2, SHUFFLE_PS(0, 0, 0, 0);
        shufps  xmm3, xmm3, SHUFFLE_PS(0, 0, 0, 0);

        shufps  xmm4, xmm4, SHUFFLE_PS(0, 1, 2, 3);
        shufps  xmm5, xmm5, SHUFFLE_PS(3, 2, 1, 0);
        shufps  xmm6, xmm6, SHUFFLE_PS(2, 3, 0, 1);
        shufps  xmm7, xmm7, SHUFFLE_PS(1, 0, 3, 2);

        mulps   xmm0, xmm4;
        mulps   xmm1, xmm5;
        mulps   xmm2, xmm6;
        mulps   xmm3, xmm7;

        movaps  xmm4, t1;
        movaps  xmm5, t2;
        movaps  xmm6, t3;

        mulps   xmm1, xmm4;  
        mulps   xmm2, xmm5;  
        mulps   xmm3, xmm6;  

        addps   xmm0, xmm1;
        addps   xmm2, xmm3;
        addps   xmm0, xmm2;

        movups [edi], xmm0; 
    }
#else
    Quat q;
    q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    quat = q;
#endif
}

/* : QuatToAxis
------------------------------------------------------------
    @Remark:
------------------------------------------------------------
*/
void Math::QuatToAxis(Vec3 & axis, float & rads, const Quat & q)
{
    rads = Math::ACos(q.w);

    float sin_theta_inv = 1.0f / Math::Sin(rads);

    axis.x = q.x * sin_theta_inv;
    axis.y = q.y * sin_theta_inv;
    axis.z = q.z * sin_theta_inv;

    rads *= 2;
}

/* : QuatToMatrix
------------------------------------------------------------
    @Remark:
        四元数转化成矩阵.
------------------------------------------------------------
*/
void Math::QuatToMatrix(Mat4 & qOut, const Quat & q)
{
    /*
	*     x'         1 - 2 * (q.y * q.y + q.z * q.z)  2 * (q.x * q.y - q.w * q.z)       2 * (q.w * q.y + q.x * q.z)         |       x
	*     y'    =    2 * (q.x * q.y + q.w * q.z)      1 - 2 * (q.x * q.x + q.z * q.z)   2 * (q.y * q.z - q.w * q.x)         |       y
	*     z'         2 * (q.x * q.z - q.w * q.y)      2 * (q.y * q.z + q.w * q.x)       1 - 2 * (q.x * q.x + q.y * q.y)     |       z
	*/

    /* 
        //Right Hand
        qOut._11 = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
        qOut._12 = 2.0f * (q.x * q.y - q.w * q.z);
        qOut._13 = 2.0f * (q.w * q.y + q.x * q.z);
        qOut._14 = 0.0f;

        qOut._21 = 2.0f * (q.x * q.y + q.w * q.z);
        qOut._22 = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
        qOut._23 = 2.0f * (q.y * q.z - q.w * q.x);
        qOut._24 = 0.0f;

        qOut._31 = 2.0f * (q.x * q.z - q.w * q.y);
        qOut._32 = 2.0f * (q.y * q.z + q.w * q.x);
        qOut._33 = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
        qOut._34 = 0.0f;

        qOut._41 = 0.0f;
        qOut._42 = 0.0f;
        qOut._43 = 0.0f;
        qOut._44 = 1.0f;
    */

    /*
        Left Hand
    */
    qOut._11 = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    qOut._12 = 2.0f * (q.x * q.y + q.w * q.z);
    qOut._13 = 2.0f * (q.x * q.z - q.w * q.y);
    qOut._14 = 0.0f;

    qOut._21 = 2.0f * (q.x * q.y - q.w * q.z); 
    qOut._22 = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    qOut._23 = 2.0f * (q.y * q.z + q.w * q.x);
    qOut._24 = 0.0f;

    qOut._31 = 2.0f * (q.w * q.y + q.x * q.z);
    qOut._32 = 2.0f * (q.y * q.z - q.w * q.x);
    qOut._33 = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    qOut._34 = 0.0f;

    qOut._41 = 0.0f;
    qOut._42 = 0.0f;
    qOut._43 = 0.0f;
    qOut._44 = 1.0f;
}

void Math::QuatTranslate(Vec3 & vOut, const Quat & q, const Vec3 & v)
{
    //NVIDIA SDK implementation
    Vec3 uv, uuv;

    Vec3 qv(q.x, q.y, q.z);

    VecCross(uv, qv, v);
    VecCross(uuv, qv, uv);

    uv *= (2.0f * q.w);
    uuv *= 2.0f;

    vOut = v + uv + uuv;
}

void Math::QuatRotation(Vec3 & vOut, const Quat & q, const Vec3 & v)
{
    //NVIDIA SDK implementation
    Vec3 uv, uuv;

    Vec3 qv(q.x, q.y, q.z);

    VecCross(uv, qv, v);
    VecCross(uuv, qv, uv);

    uv *= (2.0f * q.w);
    uuv *= 2.0f;

    vOut = v + uv + uuv;
}

void Math::QuatAxisX(Vec3 & vOut, const Quat & q)
{
    vOut.x = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    vOut.y = 2.0f * (q.x * q.y + q.w * q.z);
    vOut.z = 2.0f * (q.x * q.z - q.w * q.y);
}

void Math::QuatAxisY(Vec3 & vOut, const Quat & q)
{
    vOut.x = 2.0f * (q.x * q.y - q.w * q.z);
    vOut.y = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    vOut.z = 2.0f * (q.y * q.z + q.w * q.x);
}

void Math::QuatAxisZ(Vec3 & vOut, const Quat & q)
{
    vOut.x = 2.0f * (q.w * q.y + q.x * q.z);
    vOut.y = 2.0f * (q.y * q.z - q.w * q.x);
    vOut.z = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
}

void Math::QuatAxisXYZ(Vec3 & xAxis, Vec3 & yAxis, Vec3 & zAxis, const Quat & q)
{
    xAxis.x = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    xAxis.y = 2.0f * (q.x * q.y + q.w * q.z);
    xAxis.z = 2.0f * (q.x * q.z - q.w * q.y);

    yAxis.x = 2.0f * (q.x * q.y - q.w * q.z); 
    yAxis.y = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    yAxis.z = 2.0f * (q.y * q.z + q.w * q.x);

    zAxis.x = 2.0f * (q.w * q.y + q.x * q.z);
    zAxis.y = 2.0f * (q.y * q.z - q.w * q.x);
    zAxis.z = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
}

}