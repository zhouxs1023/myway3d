#pragma once

#include "MWCore.h"

namespace Myway
{

class MW_ENTRY Mat3x4
{
    DECLARE_ALLOC();

public:
    union 
    {
        struct 
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
        };
        float m[3][4];
        float m12[12];
    };

	static const Mat3x4 Identity;
	static const Mat3x4 Zero;

public:
    Mat3x4();
    ~Mat3x4();
    Mat3x4(float m11, float m12, float m13, float m14, 
           float m21, float m22, float m23, float m24, 
           float m31, float m32, float m33, float m34);
    Mat3x4(const Mat3x4 & m);

    Mat3x4 operator -() const;
    Mat3x4 operator +(const Mat3x4 & mat) const; 
    Mat3x4 operator -(const Mat3x4 & mat) const;

    Mat3x4& operator +=(const Mat3x4 & mat);
    Mat3x4& operator -=(const Mat3x4 & mat);

    bool operator ==(const Mat3x4 & mat) const;
    bool operator !=(const Mat3x4 & mat) const;

    float* operator [](int index);
    const float* operator [](int index) const;
};

}