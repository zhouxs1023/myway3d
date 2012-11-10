#pragma once

#include "MWMatrix3.h"

namespace Myway
{

class Mat3;
class Vec3;
class Vec4;
class Quat;
class Plane;

class MW_ENTRY Mat4
{
    DECLARE_ALLOC();

public:
    static const Mat4 Identity;
    static const Mat4 Zero;


public:
    union 
    {
	    struct 
	    {
		    float _11, _12, _13, _14;
		    float _21, _22, _23, _24;
		    float _31, _32, _33, _34;
		    float _41, _42, _43, _44;
	    };
	    float m[4][4];
        float m16[16];
    };

public:
    Mat4();
    ~Mat4();
    Mat4(float m11, float m12, float m13, float m14, 
	     float m21, float m22, float m23, float m24, 
	     float m31, float m32, float m33, float m34,  
	     float m41, float m42, float m43, float m44);
    Mat4(float * pValue);
    Mat4(const Mat4 & mat);
    Mat4(const Mat3 & mat);
    Mat4& operator =(const Mat4 & mat);

    /*
    operator float*();
    operator const float*() const;
    */
    operator Mat3();

    Mat4 operator -() const;
    Mat4 operator +(const Mat4 & mat) const; 
    Mat4 operator -(const Mat4 & mat) const; 
    Mat4 operator *(const Mat4 & mat) const; 
    Mat4 operator *(float v) const; 
    Mat4 operator /(float v) const; 

    Mat4& operator +=(const Mat4 & mat);
    Mat4& operator -=(const Mat4 & mat);
    Mat4& operator *=(const Mat4 & mat);
    Mat4& operator *=(float v);
    Mat4& operator /=(float v);

    bool operator ==(const Mat4 & mat) const;
    bool operator !=(const Mat4 & mat) const;

    float* operator [](int index);
    const float* operator [](int index) const;

    friend MW_ENTRY Mat4 operator *(float v, const Mat4 & mat);
    friend MW_ENTRY std::ostream & operator <<(std::ostream & os, const Mat4 & mat); 

public:
    float   Minor(int r, int c) const;
    float   Minor(int r0, int r1, int r2, int c0, int c1, int c2) const;
    bool    IsAffine() const;

    float   Det() const;
    Mat4    Transpose() const;
    Mat4    Inverse() const;

    Vec4    GetColumn(int c) const;

    void    MakeTransform(const Vec3 & trans, const Quat & rotate, const Vec3 & scale);
    void    MakeTranslate(float x, float y, float z);
    void    MakeRotationX(float rads);
    void    MakeRotationY(float rads);
    void    MakeRotationZ(float rads);
    void    MakeRotationAxis(const Vec3 & vAxis, float rads);
    void    MakeRotationAxis(const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis);
    void    MakeRotationYawPitchRoll(float yaw, float pitch, float roll);
    void    MakeRotationQuaternion(float x, float y, float z, float w);
    void    MakeScale(float x, float y, float z);
    void    MakeOrtho();

    void    MakeViewLH(const Vec3 & vEye, const Vec3 & vAt, const Vec3 & vUp);
    void    MakeViewLH(const Vec3 & vEye, const Quat & qOrient);
    void    MakeViewLHEX(const Vec3 & vEye, const Vec3 & vDir, const Vec3 & vUp);
    void    MakeOrthoLH(float w, float h, float zn, float zf);
    void    MakeOrthoCenterLH(float l, float r, float t, float b, float n, float f);
    void    MakePerspectiveLH(float w, float h,float n, float f);
    void    MakePerspectiveCenterLH(float l, float r, float t, float b, float n, float f);
    void    MakePerspectiveFovLH(float fov, float aspect,float n, float f);
    void    MakeShadow(const Vec4 & l, const Plane & p);
    void    MakeReflect(const Plane & p);

    Mat4    Lerp(const Mat4 & m, float t);
};

}