#pragma once

#include "MWVector3.h"

namespace Myway
{

class MW_ENTRY Quat
{
    DECLARE_ALLOC();

public:
    static const Quat Identity;

public:
    union
    {
        struct {
            float x, y, z, w;
        };
        float m[4];
    };

public:
    Quat(void) {}
    ~Quat(void) {}
    Quat(float x, float y, float z, float w);
    Quat(const Quat & q);

    Quat & operator =(const Quat & q);
    Quat operator -() const;

    Quat operator +(const Quat & q) const;
    Quat operator -(const Quat & q) const;
    Quat operator *(const Quat & q) const;
    Quat operator *(float v) const;
    Quat operator /(float v) const;

    Quat & operator +=(const Quat & q);
    Quat & operator -=(const Quat & q);
    Quat & operator *=(const Quat & q);
    Quat & operator *=(float v);
    Quat & operator /=(float v);

    bool operator ==(const Quat & q) const;
    bool operator !=(const Quat & q) const;

    friend MW_ENTRY Quat operator *(float v, const Quat & q);
    friend MW_ENTRY Quat operator /(float v, const Quat & q);

    friend MW_ENTRY Vec3 operator *(const Vec3 & v, const Quat & q);

public:
    float Dot(const Quat & rk) const;
    float Length() const;
    Quat Inverse() const;
    Quat Normalize() const;
    Quat Conjugate() const;

    void FromAxis(const Vec3 & vAxis, float rads);
    void FromAxis(const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis);
    void FromDir(const Vec3 & dir1, const Vec3 & dir2, const Vec3 & fallbackAxis = Vec3::Zero, bool normalize = true);
    void FromMatrix(const Mat4 & rot);
    void FromPitchYawRoll(float pitch, float yaw, float roll);

	static Quat S_FromAxis(const Vec3 & vAxis, float rads);
	static Quat S_FromAxis(const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis);
	static Quat S_FromDir(const Vec3 & dir1, const Vec3 & dir2, const Vec3 & fallbackAxis = Vec3::Zero, bool normalize = true);
	static Quat S_FromMatrix(const Mat4 & rot);
	static Quat S_FromPitchYawRoll(float pitch, float yaw, float roll);

    Quat Slerp(const Quat & rk, float t);

    void ToAxis(Vec3 & axis, float & rads) const;
    Mat4 ToMatrix() const;

    Vec3 Rotate(const Vec3 & v) const;

    Vec3 AxisX() const;
    Vec3 AxisY() const;
    Vec3 AxisZ() const;
    void AxisXYZ(Vec3 & xAxis, Vec3 & yAxis, Vec3 & zAxis) const;
};

}