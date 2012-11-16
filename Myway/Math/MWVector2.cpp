#include "MWVector2.h"
#include "MWMath.h"

namespace Myway
{

const Vec2 Vec2::Zero            = Vec2(0, 0);
const Vec2 Vec2::Unit            = Vec2(1, 1);
const Vec2 Vec2::UnitX           = Vec2(1, 0);
const Vec2 Vec2::UnitY           = Vec2(0, 1);
const Vec2 Vec2::NegUnitX        = Vec2(-1, 0);
const Vec2 Vec2::NegUnitY        = Vec2(0, -1);


/* :) Vec2
---------------------------------------------------------------------------------------
    @Remark:
        2D vertor, 2 floats
---------------------------------------------------------------------------------------
*/
inline Vec2::Vec2()
{
}

inline Vec2::~Vec2()
{
}

inline Vec2::Vec2(float v)
: x(v), y(v)
{
}

inline Vec2::Vec2(float _x, float _y) : x(_x), y(_y)
{
}

inline Vec2::Vec2(const Vec2 & v) : x(v.x), y(v.y)
{
}

inline Vec2& Vec2::operator = (const Vec2 & v)
{
    x = v.x, y = v.y;
    return *this;
}

inline Vec2::operator float* ()
{
    return (float*)this;
}

inline Vec2::operator const float* () const
{
    return (const float *)this;
}

inline bool Vec2::operator ==(const Vec2 & v) const
{
    return (x == v.x && y == v.y);
}

inline bool Vec2::operator !=(const Vec2 & v) const
{
    return (x != v.x || y != v.y);
}

inline Vec2 Vec2::operator -() const
{
    return Vec2(-x, -y);
}

inline Vec2 Vec2::operator + (const Vec2 & v) const
{
    return Vec2(x + v.x, y + v.y);
}

inline Vec2 Vec2::operator - (const Vec2 & v) const
{
    return Vec2(x - v.x, y - v.y);
}

inline Vec2 Vec2::operator * (const Vec2 & v) const
{
    return Vec2(x * v.x, y * v.y);
}

inline Vec2 Vec2::operator / (const Vec2 & v) const
{
    return Vec2(x / v.x, y / v.y);
}

inline Vec2 Vec2::operator * (float f) const
{
    return Vec2(x * f, y * f);
}

inline Vec2 Vec2::operator / (float f) const
{
    assert(Math::Abs(f) > DEFAULT_EPSILON);
    f = 1.0f / f;
    return Vec2(x * f, y * f);
}

inline Vec2& Vec2::operator += (const Vec2 & v)
{
    x += v.x;
    y += v.y;
    return *this;
}

inline Vec2& Vec2::operator -= (const Vec2 & v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

inline Vec2& Vec2::operator *= (float f)
{
    x *= f;
    y *= f;
    return *this;
}

inline Vec2& Vec2::operator /= (float f)
{
    assert(Math::Abs(f) > DEFAULT_EPSILON);
    f = 1.0f / f;
    x *= f;
    y *= f;
    return *this;
}

Vec2 operator * (float f, const Vec2 & v)
{
    return v * f;
}

Vec2 operator / (float f, const Vec2 & v)
{
    return v / f;
}

std::ostream & operator << (std::ostream & os, const Vec2 & v)
{
    os << v.x << "  " << v.y;
    return os;
}



Vec2 Vec2::operator * (const Mat3 & m) const
{
    return Transform(m);
}

Vec2 Vec2::Minimum(const Vec2 & rk) const
{
    Vec2 vOut;
    Math::VecMinimum(vOut, *this, rk);
    return vOut;
}

Vec2 Vec2::Normalize() const
{
    Vec2 vOut;

    Math::VecNormalize(vOut, *this);

    return vOut;
}

float Vec2::NormalizeL()
{
	float len = Length();

	x /= len;
	y /= len;

	return len;
}

float Vec2::Dot(const Vec2 & rk) const
{
    return Math::VecDot(*this, rk);
}

float Vec2::Length() const
{
    return Math::VecLength(*this);
}

float Vec2::LengthSq() const
{
    return Math::VecLengthSq(*this);
}

float Vec2::InvLength() const
{
    return Math::VecInvLength(*this);
}

float Vec2::Distance(const Vec2 & rk) const
{
    return Math::VecDistance(*this, rk);
}

float Vec2::DistanceSq(const Vec2 & rk) const
{
    return Math::VecDistanceSq(*this, rk);
}

Vec2 Vec2::Reflect(const Vec2 & n) const
{
    Vec2 vOut;
    Math::VecReflect(vOut, *this, n);
    return vOut;
}

Vec2 Vec2::Refract(const Vec2 & n, float eta) const
{
    Vec2 vOut;
    Math::VecRefract(vOut, *this, n, eta);
    return vOut;
}

Vec2 Vec2::Transform(const Mat3 & m) const
{
    Vec2 vOut;
    Math::VecTransform(vOut, *this, m);
    return vOut;
}

Vec2 Vec2::TransformN(const Mat3 & m) const
{
    Vec2 vOut;
    Math::VecTransformN(vOut, *this, m);
    return vOut;
}

Vec2 Vec2::Lerp(const Vec2 & rk, float t) const
{
    Vec2 vOut;
    Math::VecLerp(vOut, *this, rk, t);
    return vOut;
}

Vec2 Vec2::Perpendicular() const
{
    return Vec2(-y, x);
}

}
