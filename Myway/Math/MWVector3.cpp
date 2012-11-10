#include "MWVector3.h"
#include "MWMath.h"

namespace Myway
{

const Vec3 Vec3::Zero          = Vec3(0, 0, 0);
const Vec3 Vec3::Unit          = Vec3(1.0f, 1.0f, 1.0f); 
const Vec3 Vec3::UnitX         = Vec3(1.0f, 0, 0); 
const Vec3 Vec3::UnitY         = Vec3(0, 1.0f, 0);
const Vec3 Vec3::UnitZ         = Vec3(0, 0, 1.0);
const Vec3 Vec3::NegUnitX      = Vec3(-1.0f, 0, 0);
const Vec3 Vec3::NegUnitY      = Vec3(0, -1.0f, 0);
const Vec3 Vec3::NegUnitZ      = Vec3(0, 0, -1.0f);



/* :) Vec3
---------------------------------------------------------------------------------------
    @Remark:
        3D vertor, 3 floats
---------------------------------------------------------------------------------------
*/
inline Vec3::Vec3()
{
}

inline Vec3::~Vec3()
{
}

inline Vec3::Vec3(float v)
: x(v), y(v), z(v)
{
}

inline Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) 
{
}


inline Vec3::Vec3(const Vec3 & v) : x(v.x), y(v.y), z(v.z)
{
}

inline Vec3& Vec3::operator = (const Vec3 & v)
{
	x = v.x, y = v.y, z = v.z;
	return *this;
}

inline Vec3::operator float* ()
{
	return (float*)this;
}

inline Vec3::operator const float* () const
{
	return (const float*)this;
}

inline bool Vec3::operator ==(const Vec3 & v) const
{
	return x == v.x && y == v.y && z == v.z;
}

inline bool Vec3::operator ==(float v) const
{
    return x == v && y == v && z == v;
}

inline bool Vec3::operator !=(const Vec3 & v) const
{
	return x != v.x || y != v.y || z != v.z;
}

inline bool Vec3::operator !=(float v) const
{
    return x != v || y != v || z != v;
}

inline bool Vec3::operator > (const Vec3 & v) const
{
   return x > v.x && y > v.y && z > v.z;
}

inline bool Vec3::operator >=(const Vec3 & v) const
{
    return x >= v.x && y >= v.y && z >= v.z;
}

inline bool Vec3::operator < (const Vec3 & v) const
{
    return x < v.x && y < v.y && z < v.z;
}

inline bool Vec3::operator <=(const Vec3 & v) const
{
    return x <= v.x && y <= v.y && z <= v.z;
}

inline bool Vec3::operator > (float v) const
{
    return x > v && y > v && z > v;
}

inline bool Vec3::operator >=(float v) const
{
    return x >= v && y >= v && z >= v;
}

inline bool Vec3::operator < (float v) const
{
    return x < v && y < v && z < v;
}

inline bool Vec3::operator <=(float v) const
{
    return x <= v && y <= v && z <= v;
}

inline Vec3 Vec3::operator + (const Vec3 & v) const
{
	return Vec3(x + v.x, y + v.y, z + v.z); 
}

inline Vec3 Vec3::operator - (const Vec3 & v) const
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

inline Vec3 Vec3::operator * (const Vec3 & v) const
{
    return Vec3(x * v.x, y * v.y, z * v.z);
}

inline Vec3 Vec3::operator / (const Vec3 & v) const
{
    return Vec3(x / v.x, y / v.y, z / v.z);
}

inline Vec3 Vec3::operator * (float f) const
{
	return Vec3(x * f, y * f, z * f);
}

inline Vec3 Vec3::operator / (float f) const
{
	assert(Math::Abs(f) > DEFAULT_EPSILON);
	f = 1.0f / f;

	return Vec3(x * f, y * f, z * f);
}

inline Vec3& Vec3::operator += (const Vec3 & v)
{
	x += v.x, y += v.y, z += v.z;
	return *this;
}

inline Vec3& Vec3::operator -= (const Vec3 & v)
{
	x -= v.x, y -= v.y, z -= v.z;
	return *this;
}

inline Vec3& Vec3::operator *= (float f)
{
	x *= f, y *= f, z *= f;
	return *this;
}

inline Vec3& Vec3::operator /= (float f)
{
	assert(Math::Abs(f) > DEFAULT_EPSILON);

	f = 1.0f / f;
	x *= f, y *= f, z *= f;

	return *this;
}

inline Vec3 Vec3::operator -() const
{
	return Vec3(-x, -y, -z);
}

Vec3 operator *(float f, const Vec3 & v)
{
	return v * f;
}

Vec3 operator /(float f, const Vec3 & v)
{
	return Vec3(f / v.x, f / v.y, f / v.z);
}

std::ostream & operator << (std::ostream & os, const Vec3 & v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
}


Vec3 Vec3::operator * (const Mat4 & m) const
{
    return Transform(m);
}

Vec3 & Vec3::operator *= (const Mat4 & m)
{
    Math::VecTransform(*this, *this, m);
    return *this;
}

Vec3 Vec3::Minimum(const Vec3 & rk) const
{
    Vec3 vOut;
    Math::VecMinimum(vOut, *this, rk);
    return vOut;
}

float Vec3::Dot(const Vec3 & rk) const
{
    return Math::VecDot(*this, rk);
}

float Vec3::Length() const
{
    return Math::VecLength(*this);
}

float Vec3::LengthSq() const
{
    return Math::VecLengthSq(*this);
}

float Vec3::InvLength() const
{
    return Math::VecInvLength(*this);
}

float Vec3::Distance(const Vec3 & rk) const
{
    return Math::VecDistance(*this, rk);
}

float Vec3::DistanceSq(const Vec3 & rk) const
{
    return Math::VecDistanceSq(*this, rk);
}

Vec3 Vec3::Reflect(const Vec3 & n) const
{
    Vec3 vOut;
    Math::VecReflect(vOut, *this, n);
    return vOut;
}

Vec3 Vec3::Refract(const Vec3 & n, float eta) const
{
    Vec3 vOut;
    Math::VecRefract(vOut, *this, n, eta);
    return vOut;
}

Vec3 Vec3::Cross(const Vec3 & rk) const
{
    Vec3 vOut;
    Math::VecCross(vOut, *this, rk);
    return vOut;
}

Vec3 Vec3::CrossN(const Vec3 & rk) const
{
    Vec3 vOut;
    Math::VecCross(vOut, *this, rk);
    return vOut;
}

Vec3 Vec3::Transform(const Mat4 & m) const
{
    Vec3 vOut;
    Math::VecTransform(vOut, *this, m);
    return vOut;
}

Vec3 Vec3::TransformN(const Mat4 & m) const
{
    Vec3 vOut;
    Math::VecTransformN(vOut, *this, m);
    return vOut;
}

Vec3 Vec3::TransformA(const Mat4 & m) const
{
    Vec3 vOut;
    Math::VecTransformAffine(vOut, *this, m);
    return vOut;
}

Vec3 Vec3::Lerp(const Vec3 & rk, float t) const
{
    Vec3 vOut;
    Math::VecLerp(vOut, *this, rk, t);
    return vOut;
}

Vec3 Vec3::Normalize() const
{
    Vec3 vOut;

    Math::VecNormalize(vOut, *this);

    return vOut;
}

float Vec3::NormalizeL()
{
    float len = Length();
    
    float invLen = 1 / len;

    x *= invLen;
    y *= invLen;
    z *= invLen;

    return len;
}

}