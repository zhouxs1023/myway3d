#include "MWMath.h"
#include "MWMathSSE.h"

namespace Myway
{

float Math::MatMinor(const Mat3 & m, int r, int c)
{
    int i, j;
    float n[2][2];

    for (i = 0; i < r; ++i)
    {
        for (j = 0; j < c; ++j)
        {
            n[i][j] = m[i][j];
        }

        for (j = c + 1; j < 3; ++j)
        {
            n[i][j - 1] = m[i][j];
        }
    }

    for (i = r + 1; i < 3; ++i)
    {
        for (j = 0; j < c; ++j)
        {
            n[i - 1][j] = m[i][j];
        }

        for (j = c + 1; j < 3; ++j)
        {
            n[i - 1][j - 1] = m[i][j];
        }
    }

    return n[0][0] * n[1][1] - n[0][1] * n[1][0];
}

float Math::MatMinor(const Mat4 & m, int r, int c)
{
    int i, j;
    Mat3 n;

    for (i = 0; i < r; ++i)
    {
        for (j = 0; j < c; ++j)
        {
            n[i][j] = m[i][j];
        }

        for (j = c + 1; j < 4; ++j)
        {
            n[i][j - 1] = m[i][j];
        }
    }

    for (i = r + 1; i < 4; ++i)
    {
        for (j = 0; j < c; ++j)
        {
            n[i - 1][j] = m[i][j];
        }

        for (j = c + 1; j < 4; ++j)
        {
            n[i - 1][j - 1] = m[i][j];
        }
    }

    return MatDet(n);
}

float Math::MatMinor(const Mat3 & m, int r0, int r1, int c0, int c1)
{
    return m[r0][c0] * m[r1][c1] - m[r1][c0] * m[r0][c1];
}

float Math::MatMinor(const Mat4 & m, int r0, int r1, int r2,
                                            int c0, int c1, int c2)
{
    return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
           m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
           m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
}

float Math::MatDet(const Mat3 & m)
{
    return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
           m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
           m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
}

bool Math::MatIsAffine(const Mat3 & m)
{
    return m._13 == 0 && m._23 == 0 && m._33 == 1;
}

bool Math::MatIsAffine(const Mat4 & m)
{
    return m._14 == 0 && m._24 == 0 && m._34 == 0 && m._44 == 1;
}

float Math::MatDet(const Mat4 & m)
{
	return m[0][0] * MatMinor(m, 1, 2, 3, 1, 2, 3) -
           m[0][1] * MatMinor(m, 1, 2, 3, 0, 2, 3) +
           m[0][2] * MatMinor(m, 1, 2, 3, 0, 1, 3) -
           m[0][3] * MatMinor(m, 1, 2, 3, 0, 1, 2);
}

void Math::MatTranspose(Mat3 & mOut, const Mat3 & m)
{
    Mat3 mat;

    mat._11 = m._11, mat._12 = m._21, mat._13 = m._31;
    mat._21 = m._12, mat._22 = m._22, mat._23 = m._32;
    mat._31 = m._13, mat._32 = m._23, mat._33 = m._33;

    mOut = mat;
}

void Math::MatTranspose(Mat4 & mOut, const Mat4 & m)
{
    Mat4 mat;

	mat._11 = m._11, mat._12 = m._21, mat._13 = m._31, mat._14 = m._41;
	mat._21 = m._12, mat._22 = m._22, mat._23 = m._32, mat._24 = m._42;
	mat._31 = m._13, mat._32 = m._23, mat._33 = m._33, mat._34 = m._43;
	mat._41 = m._14, mat._42 = m._24, mat._43 = m._34, mat._44 = m._44;
    
    mOut = mat;
}

void Math::MatInverse(Mat3 & mOut, const Mat3 & m)
{
    Mat4 mat(m);
    MatInverse(mat, mat);
    mOut = (Mat3)mat;
}


void Math::MatInverse(Mat4 & matInv, const Mat4 & m)
{
    float m00 = m[0][0], m10 = m[0][1], m20 = m[0][2], m30 = m[0][3];
    float m01 = m[1][0], m11 = m[1][1], m21 = m[1][2], m31 = m[1][3];
    float m02 = m[2][0], m12 = m[2][1], m22 = m[2][2], m32 = m[2][3];
    float m03 = m[3][0], m13 = m[3][1], m23 = m[3][2], m33 = m[3][3];

    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float d00 = t00 * invDet;
    float d10 = t10 * invDet;
    float d20 = t20 * invDet;
    float d30 = t30 * invDet;

    float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

   matInv = Mat4(d00, d10, d20, d30,
                 d01, d11, d21, d31,
                 d02, d12, d22, d32,
                 d03, d13, d23, d33);
}

void Math::MatTranslate(Mat3 & mOut, float x, float y)
{
    mOut = Mat3::Identity;
	mOut._31 = x;
	mOut._32 = y;
}

void Math::MatTranslate(Mat4 & mOut, float x, float y, float z)
{
	mOut = Mat4::Identity;
	mOut._41 = x;
	mOut._42 = y;
	mOut._43 = z;
}

void Math::MatRotation(Mat3 & mat, float rads)
{
	float sine = Math::Sin(rads);
	float cosine = Math::Cos(rads);

	mat = Mat3::Identity;
	mat._11 = cosine;
	mat._12 = sine;
	mat._21 = -sine;
	mat._22 = cosine;
}

void Math::MatRotationX(Mat4 & mat, float rads)
{
	float sine = Math::Sin(rads);
	float cosine = Math::Cos(rads);

	mat = Mat4::Identity;
	mat._22 = cosine;
	mat._32 = -sine;
	mat._23 = sine;
	mat._33 = cosine;
}

void Math::MatRotationY(Mat4 & mat, float rads)
{
	float sine = Math::Sin(rads);
	float cosine = Math::Cos(rads);
	
	mat = Mat4::Identity;
	mat._11 = cosine;
	mat._31 = sine;
	mat._13 = -sine;
	mat._33 = cosine;
}

void Math::MatRotationZ(Mat4 & mat, float rads)
{
	float sine = Math::Sin(rads);
	float cosine = Math::Cos(rads);

	mat = Mat4::Identity;
	mat._11 = cosine;
	mat._21 = -sine;
	mat._12 = sine;
	mat._22 = cosine;
}

void Math::MatRotationAxis(Mat4 & mat, const Vec3 & n, float rads)
{
	//v' = (v - (v * n)n)cos(angle) + (n X v)sin(angle) + (v * n)n
	float sine = Math::Sin(rads);
	float cosine = Math::Cos(rads);
	float _1_sub_cosine = 1.0f - cosine;

	float x_y = n.x * n.y;
	float x_z = n.x * n.z;
	float y_z = n.y * n.z;

	mat = Mat4::Identity;

	mat._11 = n.x * n.x * _1_sub_cosine + cosine;
	mat._12 = x_y * _1_sub_cosine + n.z * sine;
	mat._13 = x_z * _1_sub_cosine - n.y * sine;

	mat._21 = x_y * _1_sub_cosine - n.z * sine;
	mat._22 = n.y * n.y * _1_sub_cosine + cosine;
	mat._23 = y_z * _1_sub_cosine + n.x * sine;

	mat._31 = x_z * _1_sub_cosine + n.y * sine;
	mat._32 = y_z * _1_sub_cosine - n.x * sine;
	mat._33 = n.z * n.z * _1_sub_cosine + cosine;
}

void Math::MatRotationAxis(Mat4 & mat, const Vec3 & xaxis, const Vec3 & yaxis, const Vec3 & zaxis)
{
    mat._11 = xaxis.x,  mat._12 = xaxis.y,  mat._13 = xaxis.z,  mat._14 = 0.0f;
    mat._21 = yaxis.x,  mat._22 = yaxis.y,  mat._23 = yaxis.z,  mat._24 = 0.0f;
    mat._31 = zaxis.x,  mat._32 = zaxis.y,  mat._33 = zaxis.z,  mat._34 = 0.0f;
    mat._41 = 0,        mat._42 = 0,        mat._43 = 0,        mat._44 = 1.0f;
}

void Math::MatRotationYawPitchRoll(Mat4 & mat, float yaw, float pitch, float roll)
{
	float sinx = Math::Sin(pitch);
	float cosx = Math::Cos(pitch);
    float siny = Math::Sin(yaw);
    float cosy = Math::Cos(yaw);
	float sinz = Math::Sin(roll);
	float cosz = Math::Cos(roll);

	mat = Mat4::Identity;

	mat._11 = cosy * cosz + siny * sinx * sinz;
	mat._12 = sinz * cosx;
	mat._13 = -siny * sinz + cosy * sinx * sinz;

	mat._21 = -cosy * sinz + siny * sinx * cosz;
	mat._22 = cosz * cosx;
	mat._23 = sinz * siny + cosy * sinx * cosz;

	mat._31 = siny * cosx;
	mat._32 = -sinx;
	mat._33 = cosy * cosx;
}

//void Math::MatRotationToYawPitchRoll(float & yaw, float & pitch, float & roll, const Mat4 & mOut)
//{
//    //
//    //  rotation matrix:
//    //
//    //          cos(y) * cos(r) + sin(y) * sin(p) * sin(r)
//    //
//}

void Math::MatRotationQuaternion(Mat4 & mat, float x, float y, float z, float w)
{
	float _2_x_x = 2.0f * x * x;
	float _2_y_y = 2.0f * y * y;
	float _2_z_z = 2.0f * z * z;
	float _2_x_y = 2.0f * x * y;
	float _2_x_z = 2.0f * x * z;
	float _2_y_z = 2.0f * y * z;
	float _2_w_x = 2.0f * w * x;
	float _2_w_y = 2.0f * w * y;
	float _2_w_z = 2.0f * w * z;

	mat = Mat4::Identity;

	mat._11 = 1.0f - _2_y_y - _2_z_z;
	mat._12 = _2_x_y + _2_w_z;
	mat._13 = _2_x_z - _2_w_y;

	mat._21 = _2_x_y - _2_w_z;
	mat._22 = 1.0f - _2_x_x - _2_z_z;
	mat._23 = _2_y_z + _2_w_x;

	mat._31 = _2_x_z + _2_w_y;
	mat._32 = _2_y_z - _2_w_x;
	mat._33 = 1.0f - _2_x_x - _2_y_y;
}

void Math::MatScale(Mat4 & mat, float x, float y, float z)
{
	mat = Mat4::Identity;
	mat._11 = x;
	mat._22 = y;
	mat._33 = z;
}

void Math::MatOrtho(Mat4 & mat)
{
	//r1' = r1 
	//r2' = r2 - dot(r2, r1') * r1' / dot(r1', r1')
	//r3' = r3 - dot(r3, r1') * r1' / dot(r1', r1') - dot(r3, r2') * r2' / dot(r2', r2')
	Vec3 r1(mat._11, mat._12, mat._13);
	Vec3 r2(mat._21, mat._22, mat._23);
	Vec3 r3(mat._31, mat._32, mat._33);

	VecNormalize(r1, r1);
	VecNormalize(r2, r2);
	VecNormalize(r3, r3);

	r2 -= r1 * VecDot(r2, r1);
	r3 -= r1 * VecDot(r3, r1) + r2 * VecDot(r3, r2);

	mat._11 = r1.x, mat._12 = r1.y, mat._13 = r1.z;
	mat._21 = r2.x, mat._22 = r2.y, mat._23 = r2.z;
	mat._31 = r3.x, mat._32 = r3.y, mat._33 = r3.z;
}

void Math::MatOrthoLH(Mat4 & mat, float w, float h, float n, float f)
{
	d_assert(w > DEFAULT_EPSILON && 
		     h > DEFAULT_EPSILON && 
		     f - n > DEFAULT_EPSILON);

    /*
        2 / w       0           0               0
        0           2 / h       0               0
        0           0           1 / (f - n)     0
        0           0           -n / (f - n)    1
    */

	float inv = 1.0f / (f - n);
	mat = Mat4::Zero;

	mat._11 = 2.0f / w;
	mat._22 = 2.0f / h;
	mat._33 = inv;
	mat._43 = -n * inv;
    mat._44 = 1.0f;
}

void Math::MatOrthoCenterLH(Mat4 & mat, float l, float r, float t, float b, float n, float f)
{
    d_assert(r - l > DEFAULT_EPSILON &&
             t - b > DEFAULT_EPSILON && 
		     f - n > DEFAULT_EPSILON);

    /*
        2 / (r - l)         0                   0               0
        0                   2 / (t - b)         0               0
        0                   0                   1 / (f - n)     0
        -2l / (r - l) - 1   -2b / (t - b) -1    -n / (f - n)    1
    */

    mat = Mat4::Zero;

    float inv_w = 1.0f / (r - l);
    float inv_h = 1.0f / (t - b);
    float inv_d = 1.0f / (f - n);

    mat._11 = 2.0f * inv_w;
    mat._41 = -(r + l) * inv_w;;

    mat._22 = 2.0f * inv_h;
    mat._42 = -(t + b) * inv_h;

    mat._33 = inv_d;
    mat._43 = -n * inv_d;

    mat._44 = 1.0f;
}

void Math::MatViewLH(Mat4 & mat, const Vec3 & vEye, const Vec3 & vAt, const Vec3 & vUp)
{
    Vec3 xaxis; 
    Vec3 yaxis;
    Vec3 zaxis;

    VecNormalize(zaxis, vAt - vEye);
    VecCross(xaxis, vUp, zaxis);
    VecNormalize(xaxis, xaxis);
    VecCross(yaxis, zaxis, xaxis);

    mat._11 = xaxis.x, mat._21 = xaxis.y, mat._31 = xaxis.z;
    mat._12 = yaxis.x, mat._22 = yaxis.y, mat._32 = yaxis.z;
    mat._13 = zaxis.x, mat._23 = zaxis.y, mat._33 = zaxis.z;
    mat._41 = -VecDot(xaxis, vEye);
    mat._42 = -VecDot(yaxis, vEye);
    mat._43 = -VecDot(zaxis, vEye);
    mat._14 = 0.0f;
    mat._24 = 0.0f;
    mat._34 = 0.0f;
    mat._44 = 1.0f;
}

void Math::MatViewLH(Mat4 & mat, const Vec3 & vEye, const Quat & qOrient)
{
    Vec3 xaxis; 
    Vec3 yaxis;
    Vec3 zaxis;

    QuatAxisXYZ(xaxis, yaxis, zaxis, qOrient);

    mat._11 = xaxis.x, mat._21 = xaxis.y, mat._31 = xaxis.z;
    mat._12 = yaxis.x, mat._22 = yaxis.y, mat._32 = yaxis.z;
    mat._13 = zaxis.x, mat._23 = zaxis.y, mat._33 = zaxis.z;
    mat._41 = -VecDot(xaxis, vEye);
    mat._42 = -VecDot(yaxis, vEye);
    mat._43 = -VecDot(zaxis, vEye);
    mat._14 = 0.0f;
    mat._24 = 0.0f;
    mat._34 = 0.0f;
    mat._44 = 1.0f;
}

void Math::MatViewLHEX(Mat4 & mat, const Vec3 & vEye, const Vec3 & vDir, const Vec3 & vUp)
{
    Vec3 zaxis;
    Vec3 xaxis; 
    Vec3 yaxis;

    VecNormalize(zaxis, vDir);
    VecCross(xaxis, vUp, zaxis);
    VecNormalize(xaxis, xaxis);
    VecCross(yaxis, zaxis, xaxis);

    mat._11 = xaxis.x, mat._21 = xaxis.y, mat._31 = xaxis.z;
    mat._12 = yaxis.x, mat._22 = yaxis.y, mat._32 = yaxis.z;
    mat._13 = zaxis.x, mat._23 = zaxis.y, mat._33 = zaxis.z;
    mat._41 = -VecDot(xaxis, vEye);
    mat._42 = -VecDot(yaxis, vEye);
    mat._43 = -VecDot(zaxis, vEye);
    mat._14 = 0.0f;
    mat._24 = 0.0f;
    mat._34 = 0.0f;
    mat._44 = 1.0f;
}

void Math::MatPerspectiveLH(Mat4 & mat, float w, float h,float n, float f)
{
    d_assert(w  > DEFAULT_EPSILON &&
             h > DEFAULT_EPSILON &&
             n > DEFAULT_EPSILON &&
             f - n > DEFAULT_EPSILON);

    /*
        2n / w      0           0                   0
        0           2n / h      0                   0
        0           0           f / (f - n)         1
        0           0           -n * f /(f - n)     0
    */

    float inv_d = 1.0f / (f - n);

    mat = Mat4::Zero;

    mat._11 = 2.0f * n / w;
    mat._22 = 2.0f * n / h;

    mat._33 = f * inv_d;
    mat._43 = -n * f * inv_d;

    mat._34 = 1.0f;
}

void Math::MatPerspectiveCenterLH(Mat4 & mat, float l, float r, float t, float b, float n, float f)
{
    d_assert(r - l > DEFAULT_EPSILON &&
             t - n > DEFAULT_EPSILON &&
             n > DEFAULT_EPSILON &&
             f - n > DEFAULT_EPSILON);

    /*
        2n / (r - l)        0                   0                   0
        0                   2n / (t - b)        0                   0
        -(l + r) / (r - l)  -(t + b) / (t - b)  f / (f - n)         1
        0                   0                   -n * f /(f - n)     0
    */

    float inv_w = 1.0f / (r - l);
    float inv_h = 1.0f / (t - b);
    float inv_d = 1.0f / (f - n);

    mat = Mat4::Zero;

    mat._11 = 2.0f * n * inv_w;
    mat._31 = -(l + r) * inv_w;

    mat._22 = 2.0f * n * inv_h;
    mat._32 = -(t + b) * inv_h;

    mat._33 = f * inv_d;
    mat._43 = -n * f * inv_d;

    mat._34 = 1.0f;
}

void Math::MatPerspectiveFovLH(Mat4 & mat, float fov, float aspect,float n, float f)
{
	d_assert(fov  > 0.0f && fov < 180.0f &&
             n > DEFAULT_EPSILON &&
             f - n > DEFAULT_EPSILON && 
             aspect > DEFAULT_EPSILON);

    /*
        cot(fov / 2) / aspect       0                   0                   0
        0                           cot(fov / 2)        0                   0
        0                           0                   f / (f - n)         1
        0                           0                   -f * n / (f - n)    0
    */

    float yScale = Math::Cot(fov / 2);
    float xScale = yScale / aspect;

	float inv_d = 1.0f / (f - n);

	mat = Mat4::Zero;

	mat._11 = xScale;
	mat._22 = yScale;

    mat._33 = f * inv_d;
    mat._43 = -n * f * inv_d;

    mat._34 = 1.0f;

}

void Math::MatShadow(Mat4 & mat, const Vec4 & l, const Plane & p)
{
	/*
	*		d = l * p;
	*
	*		d - p.a * l.x     -p.a * l.y     -p.a * l.z    -p.a
	*		-p.b * l.x		  d - p.b * l.y  -p.b * l.z    -p.b
	*		-p.c * l.x        -p.c * l.y     d - p.c * l.z -p.c                        
	*		-p.d * l.x        -p.d * l.y     -p.d * l.z    d - p.d
	*/
	Plane neg = -p;
	float d = p.a * l.x + p.b * l.y + p.c * l.z + p.d * l.w;
	mat._11 = neg.a * l.x,	mat._12 = neg.a * l.y,		mat._13 = neg.a * l.z,		mat._14 = neg.a;
	mat._21 = neg.b * l.x,	mat._22 = neg.b * l.y + d,	mat._23 = neg.b * l.z,		mat._24 = neg.b;
	mat._31 = neg.c * l.x,	mat._32 = neg.c * l.y,		mat._33 = neg.c * l.z + d,	mat._34 = neg.c;
	mat._41 = neg.d * l.x,	mat._42 = neg.d * l.y,		mat._43 = neg.d * l.z,		mat._44 = neg.d + d;
}

void Math::MatReflect(Mat4 & mat, const Plane & p)
{
    mat = Mat4(-2.0f * p.a * p.a + 1.0f,    -2.0f * p.a * p.b,          -2.0f * p.a * p.c,          0.0f,
               -2.0f * p.a * p.b,           -2.0f * p.b * p.b + 1.0f,   -2.0f * p.b * p.c,          0.0f,
               -2.0f * p.a * p.c,           -2.0f * p.b * p.c,          -2.0f * p.c * p.c + 1.0f,   0.0f,
               -2.0f * p.a * p.d,           -2.0f * p.b * p.d,          -2.0f * p.c * p.d,          1.0f);
}



void Math::MatMultiply(Mat4 & mOut, const Mat4 & m1, const Mat4 & m2)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	m1;
        mov		edi,	m2;
        mov		edx,	mOut;
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
        movups	[edx],	xmm0;

        movss	xmm0,	[esi + 16];
        movss	xmm1,	[esi + 20];
        movss	xmm2,	[esi + 24];
        movss	xmm3,	[esi + 28];
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
        movups	[edx + 16], xmm0;

        movss	xmm0,	[esi + 32];
        movss	xmm1,	[esi + 36];
        movss	xmm2,	[esi + 40];
        movss	xmm3,	[esi + 44];
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
        movups	[edx + 32], xmm0;

        movss	xmm0,	[esi + 48];
        movss	xmm1,	[esi + 52];
        movss	xmm2,	[esi + 56];
        movss	xmm3,	[esi + 60];
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
        movups	[edx + 48], xmm0;
    }
#else
    Mat4 m;

    m._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
    m._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
    m._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
    m._14 = m1._11 * m2._14 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;

    m._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
    m._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
    m._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
    m._24 = m1._21 * m2._14 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

    m._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
    m._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
    m._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
    m._34 = m1._31 * m2._14 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

    m._41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
    m._42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
    m._43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
    m._44 = m1._41 * m2._14 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;

    mOut = m;
#endif
}

void Math::MatMultiply(Mat4 & mOut, const Mat4 & m1, float scale)
{
#ifdef WIN32
    __asm
    {
        mov     esi,    m1;
        mov     edi,    mOut;

        movss   xmm4,   scale;
        shufps  xmm4,   xmm4, SHUFFLE_PS(0, 0, 0, 0);
        movups  xmm0,   [esi];
        movups  xmm1,   16[esi];
        movups  xmm2,   32[esi];
        movups  xmm3,   48[esi];

        mulps   xmm0,   xmm4;
        mulps   xmm1,   xmm4;
        mulps   xmm2,   xmm4;
        mulps   xmm3,   xmm4;

        movups  [edi],  xmm0;
        movups  8[edi], xmm1;
        movups  12[edi],xmm2;
        movups  16[edi],xmm3;
    }
#else
    mOut._11 = m1._11 * scale; mOut._12 = m1._12 * scale; mOut._13 = m1._13 * scale; mOut._14 = m1._14 * scale;
    mOut._21 = m1._21 * scale; mOut._22 = m1._22 * scale; mOut._23 = m1._23 * scale; mOut._24 = m1._24 * scale;
    mOut._31 = m1._31 * scale; mOut._32 = m1._32 * scale; mOut._33 = m1._33 * scale; mOut._34 = m1._34 * scale;
    mOut._41 = m1._41 * scale; mOut._42 = m1._42 * scale; mOut._43 = m1._43 * scale; mOut._44 = m1._44 * scale;
#endif
}

void Math::MatTransform(Mat4 & mOut, const Vec3 & trans, const Quat & rotate, const Vec3 & scale)
{
    Mat4 mRotate;
    QuatToMatrix(mRotate, rotate);

    /*float d1 = trans.x * mRotate._11 + trans.y * mRotate._12 + trans.z * mRotate._13;
    float d2 = trans.x * mRotate._21 + trans.y * mRotate._22 + trans.z * mRotate._23;
    float d3 = trans.x * mRotate._31 + trans.y * mRotate._32 + trans.z * mRotate._33;*/

    mOut._11 = mRotate._11 * scale.x;
    mOut._12 = mRotate._12 * scale.y;
    mOut._13 = mRotate._13 * scale.z;

    mOut._21 = mRotate._21 * scale.x;
    mOut._22 = mRotate._22 * scale.y;
    mOut._23 = mRotate._23 * scale.z;

    mOut._31 = mRotate._31 * scale.x;
    mOut._32 = mRotate._32 * scale.y;
    mOut._33 = mRotate._33 * scale.z;

    /*mOut._41 = d1 * scale.x;
    mOut._42 = d2 * scale.y;
    mOut._43 = d3 * scale.z;*/

    mOut._41 = trans.x;
    mOut._42 = trans.y;
    mOut._43 = trans.z;

    mOut._14 = 0.0f;
    mOut._24 = 0.0f;
    mOut._34 = 0.0f;
    mOut._44 = 1.0f;
}


void Math::MatLerp(Mat4 & mOut, const Mat4 & m1, const Mat4 & m2, float t)
{
#ifdef WIN32
    __asm
    {
        mov		esi,	m1;
        mov		edi,	m2;
        mov     edx,    mOut;

        movss   xmm4,   t;
        shufps  xmm4,   xmm4,   0;

        movups  xmm0,   [esi];
        movups  xmm1,   16[esi];
        movups  xmm2,   [edi];
        movups  xmm3,   16[edi];

        subps   xmm2,   xmm0;
        subps   xmm3,   xmm1;

        mulps   xmm2,   xmm4;
        mulps   xmm3,   xmm4;

        addps   xmm0,   xmm2;
        addps   xmm1,   xmm3;

        movups  [edx],  xmm0;
        movups  16[edx],  xmm1;

        movups  xmm0,   32[esi];
        movups  xmm1,   48[esi];
        movups  xmm2,   32[edi];
        movups  xmm3,   48[edi];

        subps   xmm2,   xmm0;
        subps   xmm3,   xmm1;

        mulps   xmm2,   xmm4;
        mulps   xmm3,   xmm4;

        addps   xmm0,   xmm2;
        addps   xmm1,   xmm3;

        movups  32[edx],  xmm0;
        movups  48[edx],  xmm1;
    }
#else
    mOut._11 = m1._11 + (m2._11 - m1._11) * t;
    mOut._12 = m1._12 + (m2._12 - m1._12) * t;
    mOut._13 = m1._13 + (m2._13 - m1._13) * t;
    mOut._14 = m1._14 + (m2._14 - m1._14) * t;

    mOut._21 = m1._21 + (m2._21 - m1._21) * t;
    mOut._22 = m1._22 + (m2._22 - m1._22) * t;
    mOut._23 = m1._23 + (m2._23 - m1._23) * t;
    mOut._24 = m1._24 + (m2._24 - m1._24) * t;

    mOut._31 = m1._31 + (m2._31 - m1._31) * t;
    mOut._32 = m1._32 + (m2._32 - m1._32) * t;
    mOut._33 = m1._33 + (m2._33 - m1._33) * t;
    mOut._34 = m1._34 + (m2._34 - m1._34) * t;

    mOut._11 = m1._41 + (m2._41 - m1._41) * t;
    mOut._12 = m1._42 + (m2._42 - m1._42) * t;
    mOut._13 = m1._43 + (m2._43 - m1._43) * t;
    mOut._14 = m1._44 + (m2._44 - m1._44) * t;
#endif
}

}