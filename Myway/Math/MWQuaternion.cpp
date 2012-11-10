#include "MWQuaternion.h"
#include "MWMath.h"

namespace Myway
{

const Quat Quat::Identity = Quat(0, 0, 0, 1);

/* Quat 
--------------------------------------------------------------------------
    @Remark:
        Quaternion.
--------------------------------------------------------------------------
*/
inline Quat::Quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{
}

inline Quat::Quat(const Quat &q) : x(q.x), y(q.y), z(q.z), w(q.w)
{
}

inline Quat & Quat::operator =(const Quat & q)
{
    x = q.x, y = q.y, z = q.z, w = q.w;
    return *this;
}

inline Quat Quat::operator -() const
{
    return Quat(-x, -y, -z, -w);
}

inline Quat Quat::operator +(const Quat & q) const
{
    return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
}

inline Quat Quat::operator -(const Quat & q) const
{
    return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
}

inline Quat Quat::operator *(const Quat & q) const
{
    Quat quat;
    Math::QuatMultiply(quat, *this, q);
    return quat;
}

inline Quat Quat::operator *(float v) const
{
    return Quat(x * v, y * v, z * v, w * v);
}

inline Quat Quat::operator /(float v) const
{
    v = 1.0f / v;
    return Quat(x * v, y * v, z * v, w * v);
}

inline Quat & Quat::operator +=(const Quat & q)
{
    x += q.x, y += q.y, z += q.z, w += q.w;
    return *this;
}

inline Quat & Quat::operator -=(const Quat & q)
{
    x -= q.x, y -= q.y, z -= q.z, w -= q.w;
    return *this;
}

inline Quat & Quat::operator *=(const Quat & q)
{
    Math::QuatMultiply(*this, *this, q);
    return *this;
}

inline Quat & Quat::operator *=(float v)
{
    x *= v, y *=v, z *= v, w *= v;
    return *this;
}

inline Quat & Quat::operator /=(float v)
{
    v = 1.0f / v;
    x *= v, y *=v, z *= v, w *= v;
    return *this;
}

inline bool Quat::operator ==(const Quat & q) const
{
    return x == q.x && y == q.y && z == q.z && w == q.w;
}

inline bool Quat::operator !=(const Quat & q) const
{
    return x != q.x || y != q.y || z != q.z || w != q.w;
}

inline Quat Myway::operator *(float v, const Quat & q)
{
    return q * v;
}

inline Quat Myway::operator /(float v, const Quat & q)
{
    return q / v;
}

Vec3 Myway::operator *(const Vec3 & v, const Quat & q)
{
    Vec3 vOut;
    Math::QuatRotation(vOut, q, v);
    return vOut;
}


float Quat::Dot(const Quat & rk) const
{
    return Math::QuatDot(*this, rk);
}

float Quat::Length() const
{
    return Math::QuatLength(*this);
}

Quat Quat::Inverse() const
{
    Quat quat;
    Math::QuatInverse(quat, *this);
    return quat;
}

Quat Quat::Normalize() const
{
    Quat quat;
    Math::QuatNormalize(quat, *this);
    return quat;
}

Quat Quat::Conjugate() const
{
    Quat quat;
    Math::QuatConjugate(quat, *this);
    return quat;
}

void Quat::FromAxis(const Vec3 & vAxis, float rads)
{
    Math::QuatFromAxis(*this, vAxis, rads);
}

void Quat::FromAxis(const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis)
{
    Math::QuatFromAxis(*this, xAxis, yAxis, zAxis);
}

void Quat::FromDir(const Vec3 & dir1, const Vec3 & dir2, const Vec3 & fallbackAxis, bool normalize)
{
    Math::QuatFromDir(*this, dir1, dir2, fallbackAxis, normalize);
}

void Quat::FromMatrix(const Mat4 & rot)
{
    Math::QuatFromMatrix(*this, rot);
}

void Quat::FromPitchYawRoll(float pitch, float yaw, float roll)
{
    Math::QuatFromPitchYawRoll(*this, pitch, yaw, roll);
}

Quat Quat::Slerp(const Quat & rk, float t)
{
    Quat quat;
    Math::QuatSlerp(quat, *this, rk, t);
    return quat;
}

void Quat::ToAxis(Vec3 & axis, float & rads) const
{
    Math::QuatToAxis(axis, rads, *this);
}

Mat4 Quat::ToMatrix() const
{
    Mat4 mat;
    Math::QuatToMatrix(mat, *this);
    return mat;
}

Vec3 Quat::Rotation(const Vec3 & v) const
{
    Vec3 vOut;
    Math::QuatRotation(vOut, *this, v);
    return vOut;
}

Vec3 Quat::AxisX() const
{
    Vec3 xAxis;
    Math::QuatAxisX(xAxis, *this);
    return xAxis;
}

Vec3 Quat::AxisY() const
{
    Vec3 yAxis;
    Math::QuatAxisY(yAxis, *this);
    return yAxis;
}

Vec3 Quat::AxisZ() const
{
    Vec3 zAxis;
    Math::QuatAxisZ(zAxis, *this);
    return zAxis;
}

void Quat::AxisXYZ(Vec3 & xAxis, Vec3 & yAxis, Vec3 & zAxis) const
{
    Math::QuatAxisXYZ(xAxis, yAxis, zAxis, *this);
}

}
