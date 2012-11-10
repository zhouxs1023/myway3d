#pragma once

#include "MWCore.h"

namespace Myway
{

class MW_ENTRY Mat3
{
    DECLARE_ALLOC();

public:
    static const Mat3 Identity;
    static const Mat3 Zero;

public:
    union
    {
        struct
        {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };

        float m[3][3];
        float m9[9];
    };

public:
    Mat3();
    ~Mat3();
    Mat3(float _11, float _12, float _13,
         float _21, float _22, float _23,
         float _31, float _32, float _33);
    Mat3(float * v);
    Mat3(const Mat3 & mat);

    Mat3& operator =(const Mat3 & mat);

    /*
    operator float*();
    operator const float*() const;
    */

    Mat3 operator -() const;
    Mat3 operator +(const Mat3 & mat) const; 
    Mat3 operator -(const Mat3 & mat) const; 
    Mat3 operator *(const Mat3 & mat) const; 
    Mat3 operator *(float v) const; 
    Mat3 operator /(float v) const; 

    Mat3& operator +=(const Mat3 & mat);
    Mat3& operator -=(const Mat3 & mat);
    Mat3& operator *=(const Mat3 & mat);
    Mat3& operator *=(float f);
    Mat3& operator /=(float f);

    bool operator ==(const Mat3 & mat) const;
    bool operator !=(const Mat3 & mat) const;

    float* operator [](int index);
    const float* operator [](int index) const;

#ifdef _DEBUG
    void Dump()
    {
        debug_printf("%f, %f, %f\n" \
            "%f, %f, %f\n" \
            "%f, %f, %f\n",
            _11, _12, _13,
            _21, _22, _23,
            _31, _32, _33);
    }
#endif

    friend MW_ENTRY Mat3 operator *(float v, const Mat3 & mat);
    friend MW_ENTRY std::ostream & operator <<(std::ostream & os, const Mat3 & mat); 

public:
    float   Minor(int r, int c) const;
    float   Minor(int r0, int r1, int c0, int c1) const;
    bool    IsAffine() const;

    float   Det() const;
    Mat3    Transpose() const;
    Mat3    Inverse() const;
    void    MakeTranslate(float x, float y);
    void    MakeRotation(float rads);
};

}