#include "MWMatrix3.h"
#include "MWMath.h"

namespace Myway
{


const Mat3 Mat3::Identity = Mat3(1, 0, 0,
                                 0, 1, 0,
                                 0, 0, 1);

const Mat3 Mat3::Zero = Mat3(0, 0, 0,
                             0, 0, 0,
                             0, 0, 0);



/* :) Mat3
---------------------------------------------------------------------------------------
    @Remark:
        Matrix 3x3, 6 floats
---------------------------------------------------------------------------------------
*/
inline Mat3::Mat3()
{
}

inline Mat3::~Mat3()
{
}

inline Mat3::Mat3(float m11, float m12, float m13,
				 float m21, float m22, float m23,
				 float m31, float m32, float m33)
				 : _11(m11), _12(m12), _13(m13),
				 _21(m21), _22(m22), _23(m23),
				 _31(m31), _32(m32), _33(m33)
{

}

inline Mat3::Mat3(float * v)
{
	memcpy(this, v, 36);
}

inline Mat3::Mat3(const Mat3 & mat)
{
	memcpy(this, &mat, 36);
}

inline Mat3& Mat3::operator =(const Mat3 & mat)
{
	memcpy(this, &mat, 36);
	return *this;
}

/*
inline Mat3::operator float*()
{
    return m9;
}

inline Mat3::operator const float*() const
{
    return m9;
}
*/

inline Mat3 Mat3::operator -() const
{
	return Mat3(-_11, -_12, -_13,
		        -_21, -_22, -_23,
		        -_31, -_32, -_33);
}

inline Mat3 Mat3::operator +(const Mat3 & mat) const
{
	return Mat3(_11 + mat._11, _12 + mat._12, _13 + mat._13,
		        _21 + mat._21, _22 + mat._22, _23 + mat._23,
		        _31 + mat._31, _32 + mat._32, _33 + mat._33);
}

inline Mat3 Mat3::operator -(const Mat3 & mat) const
{
	return Mat3(_11 - mat._11, _12 - mat._12, _13 - mat._13,
		        _21 - mat._21, _22 - mat._22, _23 - mat._23,
		        _31 - mat._31, _32 - mat._32, _33 - mat._33);
}

inline Mat3 Mat3::operator *(const Mat3 & mat) const
{
	Mat3 ret;

	ret._11 = _11 * mat._11 + _12 * mat._21 + _13 * mat._31;
	ret._12 = _11 * mat._12 + _12 * mat._22 + _13 * mat._32;
	ret._13 = _11 * mat._13 + _12 * mat._23 + _13 * mat._33;
	ret._21 = _21 * mat._11 + _22 * mat._21 + _23 * mat._31;
	ret._22 = _21 * mat._12 + _22 * mat._22 + _23 * mat._32;
	ret._23 = _21 * mat._13 + _22 * mat._23 + _23 * mat._33;
	ret._31 = _31 * mat._11 + _32 * mat._21 + _33 * mat._31;
	ret._32 = _31 * mat._12 + _32 * mat._22 + _33 * mat._32;
	ret._33 = _31 * mat._13 + _32 * mat._23 + _33 * mat._33;

	return ret;
}

inline Mat3 Mat3::operator *(float v) const
{
	return Mat3(_11 * v, _12 * v, _13 * v,
	            _21 * v, _22 * v, _23 * v,
		        _31 * v, _32 * v, _33 * v);
}

inline Mat3 Mat3::operator /(float v) const
{
	assert(Math::Abs(v) > DEFAULT_EPSILON);
	v = 1.0f / v;
	return operator *(v);
}

inline Mat3& Mat3::operator +=(const Mat3 & mat)
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13;
	_21 += mat._21; _22 += mat._22; _23 += mat._23;
	_31 += mat._31; _32 += mat._32; _33 += mat._33;

	return *this;
}

inline Mat3& Mat3::operator -=(const Mat3 & mat)
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33;

	return *this;
}

inline Mat3& Mat3::operator *=(const Mat3 & mat)
{
	Mat3 tmp;

	tmp._11 = _11 * mat._11 + _12 * mat._21 + _13 * mat._31;
	tmp._12 = _11 * mat._12 + _12 * mat._22 + _13 * mat._32;
	tmp._13 = _11 * mat._13 + _12 * mat._23 + _13 * mat._33;
	tmp._21 = _21 * mat._11 + _22 * mat._21 + _23 * mat._31;
	tmp._22 = _21 * mat._12 + _22 * mat._22 + _23 * mat._32;
	tmp._23 = _21 * mat._13 + _22 * mat._23 + _23 * mat._33;
	tmp._31 = _31 * mat._11 + _32 * mat._21 + _33 * mat._31;
	tmp._32 = _31 * mat._12 + _32 * mat._22 + _33 * mat._32;
	tmp._33 = _31 * mat._13 + _32 * mat._23 + _33 * mat._33;

	*this = tmp;
	return *this;
}

inline Mat3& Mat3::operator *=(float f)
{
    _11 *= f; _12 *= f; _13 *= f;
    _21 *= f; _22 *= f; _23 *= f;
    _31 *= f; _32 *= f; _33 *= f;

    return *this;
}

inline Mat3& Mat3::operator /=(float v)
{
	assert(Math::Abs(v) > DEFAULT_EPSILON);
	v = 1.0f / v;
	return *this *=(v);
}

inline bool Mat3::operator ==(const Mat3 & m) const
{
    return _11 == m._11 && _12 == m._12 && _13 == m._13 &&
           _21 == m._21 && _22 == m._22 && _23 == m._23 &&
           _31 == m._31 && _32 == m._32 && _33 == m._33;
}

inline bool Mat3::operator !=(const Mat3 & m) const
{
    return _11 != m._11 || _12 != m._12 || _13 != m._13 ||
           _21 != m._21 || _22 != m._22 || _23 != m._23 ||
           _31 != m._31 || _32 != m._32 || _33 != m._33;
}

inline float* Mat3::operator [](int index)
{
    assert(index < 3);
    return m[index];
}

inline const float* Mat3::operator [](int index) const
{
    assert(index < 3);
    return m[index];
}

inline Mat3 operator *(float v, const Mat3 & mat)
{
	return mat * v;
}

inline std::ostream & operator <<(std::ostream & os, const Mat3 & mat)
{
	os << mat._11 << "  " << mat._12 << "  " << mat._13 << "  "
		<< mat._21 << "  " << mat._22 << "  " << mat._23 << "  "
		<< mat._31 << "  " << mat._32 << "  " << mat._33;

	return os;
}



float Mat3::Minor(int r, int c) const
{
    return Math::MatMinor(*this, r, c);
}

float Mat3::Minor(int r0, int r1, int c0, int c1) const
{
    return Math::MatMinor(*this, r0, r1, c0, c1);
}

bool Mat3::IsAffine() const
{
    return Math::MatIsAffine(*this);
}

float Mat3::Det() const
{
    return Math::MatDet(*this);
}

Mat3 Mat3::Transpose() const
{
    Mat3 mat;
    Math::MatTranspose(mat, *this);
    return mat;
}

Mat3 Mat3::Inverse() const
{
    Mat3 mat;
    Math::MatInverse(mat, *this);
    return mat;
}

void Mat3::MakeTranslate(float x, float y)
{
    Math::MatTranslate(*this, x, y);
}

void Mat3::MakeRotation(float rads)
{
    Math::MatRotation(*this, rads);
}

}