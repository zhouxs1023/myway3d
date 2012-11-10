#include "MWVector4.h"
#include "MWMath.h"

namespace Myway
{

const Vec4  Vec4::Zero       = Vec4(0, 0, 0, 0);
const Vec4  Vec4::Unit       = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
const Vec4  Vec4::UnitX      = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
const Vec4  Vec4::UnitY      = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
const Vec4  Vec4::UnitZ      = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
const Vec4  Vec4::NegUnitX   = Vec4(-1.0f, 0.0f, 0.0f, 1.0f);
const Vec4  Vec4::NegUnitY   = Vec4(0.0f, -1.0f, 0.0f, 1.0f);
const Vec4  Vec4::NegUnitZ   = Vec4(0.0f, 0.0f, -1.0f, 1.0f);

/* :) Vec4
---------------------------------------------------------------------------------------
    @Remark:
        4D vertor, 4 floats
---------------------------------------------------------------------------------------
*/
inline Vec4::Vec4()
{
}

inline Vec4::~Vec4()
{
}

inline Vec4::Vec4(float v)
: x(v), y(v), z(v), w(1.0f)
{
}

inline Vec4::Vec4(float _x, float _y, float _z, float _w) 
: x(_x), y(_y), z(_z), w(_w)
{
}

inline Vec4::Vec4(const Vec3 & xyz, float _w)
: x(xyz.x), y(xyz.y), z(xyz.z), w(_w)
{
}

inline Vec4::Vec4(const Vec4 & v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
}

inline Vec4& Vec4::operator = (const Vec4 & v)
{
	x = v.x, y = v.y, z = v.z, w = v.w;
	return *this;
}

inline Vec4::operator float* ()
{
	return (float*)this;
}

inline Vec4::operator const float* () const
{
	return (const float *)this;
}

inline bool Vec4::operator ==(const Vec4 & v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

inline bool Vec4::operator ==(float v) const
{
    return x == v && y == v && z == v && w == v;
}

inline bool Vec4::operator !=(const Vec4 & v) const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

inline bool Vec4::operator !=(float v) const
{
    return x != v || y != v || z != v || w != v;
}

inline bool Vec4::operator > (const Vec4 & v) const
{
    return x > v.x && y > v.y && z > v.z && w > v.w;
}

inline bool Vec4::operator >=(const Vec4 & v) const
{
    return x >= v.x && y >= v.y && z >= v.z && w >= v.w;
}

inline bool Vec4::operator < (const Vec4 & v) const
{
    return x < v.x && y < v.y && z < v.z && w < v.w;
}

inline bool Vec4::operator <=(const Vec4 & v) const
{
    return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
}

inline bool Vec4::operator > (float v) const
{
     return x > v && y > v && z > v && w > v;
}

inline bool Vec4::operator >=(float v) const
{
    return x >= v && y >= v && z >= v && w >= v;
}

inline bool Vec4::operator < (float v) const
{
    return x < v && y < v && z < v && w < v;
}

inline bool Vec4::operator <=(float v) const
{
    return x <= v && y <= v && z <= v && w <= v;
}

inline Vec4 Vec4::operator + (const Vec4 & v) const
{
	return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline Vec4 Vec4::operator - (const Vec4 & v) const
{
	return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline Vec4 Vec4::operator * (const Vec4 & v) const
{
    return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

inline Vec4 Vec4::operator / (const Vec4 & v) const
{
    return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
}

inline Vec4 Vec4::operator * (float f) const
{
	return Vec4(x * f, y * f, z * f, w * f);
}

inline Vec4 Vec4::operator / (float f) const
{
	assert(Math::Abs(f) > DEFAULT_EPSILON);
	f = 1.0f / f;

	return Vec4(x * f, y * f, z * f, w * f);
}

inline Vec4& Vec4::operator += (const Vec4 & v)
{
	x += v.x, y += v.y, z += v.z, w = 1.0f;
	return *this;
}

inline Vec4& Vec4::operator -= (const Vec4 & v)
{
	x -= v.x, y -= v.y, z -= v.z, w -= v.w;
	return *this;
}

inline Vec4& Vec4::operator *= (float f)
{
	x *= f, y *= f, z *= f,  w *= f;
	return *this;
}

inline Vec4& Vec4::operator /= (float f)
{
	assert(Math::Abs(f) > DEFAULT_EPSILON);

	f = 1.0f / f;
	x *= f, y *= f, z *= f,  w *= f;

	return *this;
}

inline Vec4 Vec4::operator -() const
{
	return Vec4(-x, -y, -z, -w);
}

Vec4& Vec4::operator *= (const Vec4 & v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;

    return *this;
}

Vec4& Vec4::operator /= (const Vec4 & v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;

    return *this;
}

Vec4 operator * (float f, const Vec4 & v)
{
	return v * f;
}

Vec4 operator / (float f, const Vec4 & v)
{
	return Vec4(f / v.x, f / v.y, f / v.z, f / v.w);
}

std::ostream & operator << (std::ostream & os, const Vec4 & v)
{
	os << v.x << " " << v.y << " " << v.z << " " << v.w;
	return os;
}


Vec4 Vec4::operator * (const Mat4 & m) const
{
    return Transform(m);
}

Vec4 Vec4::Minimum(const Vec4 & rk) const
{
    Vec4 vOut;
    Math::VecMinimum(vOut, *this, rk);
    return vOut;
}

float Vec4::Dot(const Vec4 & rk) const
{
    return Math::VecDot(*this, rk);
}

float Vec4::Length() const
{
    return Math::VecLength(*this);
}

float Vec4::LengthSq() const
{
    return Math::VecLengthSq(*this);
}

float Vec4::InvLength() const
{
    return Math::VecInvLength(*this);
}

float Vec4::Distance(const Vec4 & rk) const
{
    return Math::VecDistance(*this, rk);
}

float Vec4::DistanceSq(const Vec4 & rk) const
{
    return Math::VecDistanceSq(*this, rk);
}

Vec4 Vec4::Reflect(const Vec4 & n) const
{
    Vec4 vOut;
    Math::VecReflect(vOut, *this, n);
    return vOut;
}

Vec4 Vec4::Refract(const Vec4 & n, float eta) const
{
    Vec4 vOut;
    Math::VecRefract(vOut, *this, n, eta);
    return vOut;
}

Vec4 Vec4::Cross(const Vec4 & rk) const
{
    Vec4 vOut;
    Math::VecCross(vOut, *this, rk);
    return vOut;
}

Vec4 Vec4::CrossN(const Vec4 & rk) const
{
    Vec4 vOut;
    Math::VecCross(vOut, *this, rk);
    return vOut;
}

Vec4 Vec4::Transform(const Mat4 & m) const
{
    Vec4 vOut;
    Math::VecTransform(vOut, *this, m);
    return vOut;
}

Vec4 Vec4::TransformN(const Mat4 & m) const
{
    Vec4 vOut;
    Math::VecTransformN(vOut, *this, m);
    return vOut;
}

Vec4 Vec4::Lerp(const Vec4 & rk, float t) const
{
    Vec4 vOut;
    Math::VecLerp(vOut, *this, rk, t);
    return vOut;
}

}
