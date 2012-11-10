#include "MWMatrix4.h"
#include "MWMath.h"

namespace Myway
{

const Mat4 Mat4::Identity = Mat4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);
const Mat4 Mat4::Zero = Mat4(0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0);

/* :) Mat4
---------------------------------------------------------------------------------------
    @Remark:
        Matrix 4x4, 16 floats
---------------------------------------------------------------------------------------
*/

inline Mat4::Mat4()
{
}

inline Mat4::~Mat4()
{
}

inline Mat4::Mat4(float m11, float m12, float m13, float m14, 
				  float m21, float m22, float m23, float m24, 
				  float m31, float m32, float m33, float m34,  
				  float m41, float m42, float m43, float m44) 

: _11(m11), _12(m12), _13(m13), _14(m14), 
  _21(m21), _22(m22), _23(m23), _24(m24),
  _31(m31), _32(m32), _33(m33), _34(m34),
  _41(m41), _42(m42), _43(m43), _44(m44) 
{
}

inline Mat4::Mat4(const Mat3 & m)
: _11(m._11), _12(m._12), _13(m._13), _14(0.0f), 
  _21(m._21), _22(m._22), _23(m._23), _24(0.0f),
  _31(m._31), _32(m._32), _33(m._33), _34(0.0f),
  _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f) 
{
}

inline Mat4::Mat4(float * pValue)
{
	Memcpy(this, pValue, 64);
}

inline Mat4::Mat4(const Mat4 & mat)
{
	Memcpy(this, &mat, 64);
}

inline Mat4& Mat4::operator =(const Mat4 & mat)
{
	Memcpy(this, &mat, 64);
	return *this;
}

/*
inline Mat4::operator float*()
{
    return m16;
}

inline Mat4::operator const float*() const
{
    return m16;
}
*/

Mat4::operator Mat3()
{
    return Mat3(_11, _12, _13,
                _21, _22, _23,
                _31, _32, _33);
}


inline Mat4 Mat4::operator -() const
{
	return Mat4(-_11, -_12, -_13, -_14,
		        -_21, -_22, -_23, -_24,
		        -_31, -_32, -_33, -_34,
		        -_41, -_42, -_43, -_44);
}

inline Mat4 Mat4::operator +(const Mat4 & mat) const
{
	return Mat4(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		        _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
				_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
				_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

inline Mat4 Mat4::operator -(const Mat4 & mat) const
{
	return Mat4(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		        _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		        _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		        _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

inline Mat4 Mat4::operator *(const Mat4 & mat) const
{
	Mat4 ret;
    Math::MatMultiply(ret, *this, mat);
	return ret;
}

inline Mat4 Mat4::operator *(float v) const
{
	return Mat4(_11 * v, _12 * v, _13 * v, _14 * v,
		        _21 * v, _22 * v, _23 * v, _24 * v,
				_31 * v, _32 * v, _33 * v, _34 * v,
				_41 * v, _42 * v, _43 * v, _44 * v);
}

inline Mat4 Mat4::operator /(float v) const
{
	assert(Math::Abs(v) > DEFAULT_EPSILON);
	v = 1.0f / v;
	return (*this) * v;
}

inline Mat4& Mat4::operator +=(const Mat4 & mat)
{
	_11 += mat._11, _12 += mat._12, _13 += mat._13, _14 += mat._14;
	_21 += mat._21, _22 += mat._22, _23 += mat._23, _24 += mat._24;
	_31 += mat._31, _32 += mat._32, _33 += mat._33, _34 += mat._34;
	_41 += mat._41, _42 += mat._42, _43 += mat._43, _44 += mat._44;

	return *this;
}

inline Mat4& Mat4::operator -=(const Mat4 & mat)
{
	_11 -= mat._11, _12 -= mat._12, _13 -= mat._13, _14 -= mat._14;
	_21 -= mat._21, _22 -= mat._22, _23 -= mat._23, _24 -= mat._24;
	_31 -= mat._31, _32 -= mat._32, _33 -= mat._33, _34 -= mat._34;
	_41 -= mat._41, _42 -= mat._42, _43 -= mat._43, _44 -= mat._44;

	return *this;
}

inline Mat4& Mat4::operator *=(const Mat4 & mat)
{
	*this = (*this) * mat;
	return *this;
}

inline Mat4& Mat4::operator *=(float v)
{
	_11 *= v, _12 *= v, _13 *= v, _14 *= v;
	_21 *= v, _22 *= v, _23 *= v, _24 *= v;
	_31 *= v, _32 *= v, _33 *= v, _34 *= v;
	_41 *= v, _42 *= v, _43 *= v, _44 *= v;
	return *this;
}

inline Mat4& Mat4::operator /=(float v)
{
	assert(Math::Abs(v) > DEFAULT_EPSILON);
	v = 1.0f / v;
	return operator *=(v);
}

inline bool Mat4::operator ==(const Mat4 & m) const
{
    return _11 == m._11 && _12 == m._12 && _13 == m._13 && _14 == m._14 &&
           _21 == m._21 && _22 == m._22 && _23 == m._23 && _24 == m._24 &&
           _31 == m._31 && _32 == m._32 && _33 == m._33 && _34 == m._34 &&
           _41 == m._41 && _42 == m._42 && _43 == m._43 && _44 == m._44;
}

inline bool Mat4::operator !=(const Mat4 & m) const
{
    return _11 != m._11 || _12 != m._12 || _13 != m._13 || _14 != m._14 ||
           _21 != m._21 || _22 != m._22 || _23 != m._23 || _24 != m._24 ||
           _31 != m._31 || _32 != m._32 || _33 != m._33 || _34 != m._34 ||
           _41 != m._41 || _42 != m._42 || _43 != m._43 || _44 != m._44;
}

inline float* Mat4::operator [](int index)
{
    assert(index < 4);
    return m[index];
}

inline const float* Mat4::operator [](int index) const
{
    assert(index < 4);
    return m[index];
}

inline Mat4 Myway::operator *(float v, const Mat4 & mat)
{
	return mat * v;
}

inline std::ostream & Myway::operator <<(std::ostream & os, const Mat4 & mat)
{
	os << mat._11 << "  " << mat._12 << "  " << mat._13 << "  " << mat._14 << std::endl
		<< mat._21 << "  " << mat._22 << "  " << mat._23 << "  " << mat._24 << std::endl
		<< mat._31 << "  " << mat._32 << "  " << mat._33 << "  " << mat._34 << std::endl
		<< mat._41 << "  " << mat._42 << "  " << mat._43 << "  " << mat._44;

	return os;
}




float Mat4::Minor(int r, int c) const
{
    return Math::MatMinor(*this, r, c);
}

float Mat4::Minor(int r0, int r1, int r2, int c0, int c1, int c2) const
{
    return Math::MatMinor(*this, r0, r1, r2, c0, c1, c2);
}

bool Mat4::IsAffine() const
{
    return Math::MatIsAffine(*this);
}

float Mat4::Det() const
{
    return Math::MatDet(*this);
}

Mat4 Mat4::Transpose() const
{
    Mat4 mat;
    Math::MatTranspose(mat, *this);
    return mat;
}

Mat4 Mat4::Inverse() const
{
    Mat4 mat;
    Math::MatInverse(mat, *this);
    return mat;
}

Vec4 Mat4::GetColumn(int c) const
{
    return Vec4(m[c][0], m[c][1], m[c][2], m[c][3]);
}

void Mat4::MakeTransform(const Vec3 & trans, const Quat & rotate, const Vec3 & scale)
{
    Math::MatTransform(*this, trans, rotate, scale);
}

void Mat4::MakeTranslate(float x, float y, float z)
{
    Math::MatTranslate(*this, x, y, z);
}

void Mat4::MakeRotationX(float rads)
{
    Math::MatRotationX(*this, rads);
}

void Mat4::MakeRotationY(float rads)
{
    Math::MatRotationY(*this, rads);
}

void Mat4::MakeRotationZ(float rads)
{
    Math::MatRotationZ(*this, rads);
}

void Mat4::MakeRotationAxis(const Vec3 & vAxis, float rads)
{
    Math::MatRotationAxis(*this, vAxis, rads);
}

void Mat4::MakeRotationAxis(const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis)
{
    Math::MatRotationAxis(*this, xAxis, yAxis, zAxis);
}

void Mat4::MakeRotationYawPitchRoll(float yaw, float pitch, float roll)
{
    Math::MatRotationYawPitchRoll(*this, yaw, pitch, roll);
}

void Mat4::MakeRotationQuaternion(float x, float y, float z, float w)
{
    Math::MatRotationQuaternion(*this, x, y, z, w);
}

void Mat4::MakeScale(float x, float y, float z)
{
    Math::MatScale(*this, x, y, z);
}

void Mat4::MakeOrtho()
{
    Math::MatOrtho(*this);
}

void Mat4::MakeViewLH(const Vec3 & vEye, const Vec3 & vAt, const Vec3 & vUp)
{
    Math::MatViewLH(*this, vEye, vAt, vUp);
}

void Mat4::MakeViewLH(const Vec3 & vEye, const Quat & qOrient)
{
    Math::MatViewLH(*this, vEye, qOrient);
}

void Mat4::MakeViewLHEX(const Vec3 & vEye, const Vec3 & vDir, const Vec3 & vUp)
{
    Math::MatViewLHEX(*this, vEye, vDir, vUp);
}

void Mat4::MakeOrthoLH(float w, float h, float zn, float zf)
{
    Math::MatOrthoLH(*this, w, h, zn, zf);
}

void Mat4::MakeOrthoCenterLH(float l, float r, float t, float b, float n, float f)
{
    Math::MatOrthoCenterLH(*this, l, r, t, b, n, f);
}

void Mat4::MakePerspectiveLH(float w, float h,float n, float f)
{
    Math::MatPerspectiveLH(*this, w, h, n, f);
}

void Mat4::MakePerspectiveCenterLH(float l, float r, float t, float b, float n, float f)
{
    Math::MatPerspectiveCenterLH(*this, l, r, t, b, n, f);
}

void Mat4::MakePerspectiveFovLH(float fov, float aspect,float n, float f)
{
    Math::MatPerspectiveFovLH(*this, fov, aspect, n, f);
}

void Mat4::MakeShadow(const Vec4 & l, const Plane & p)
{
    Math::MatShadow(*this, l, p);
}

void Mat4::MakeReflect(const Plane & p)
{
    Math::MatReflect(*this, p);
}

Mat4 Mat4::Lerp(const Mat4 & m, float t)
{
    Mat4 mat;
    Math::MatLerp(mat, *this, m, t);
    return mat;
}



}
