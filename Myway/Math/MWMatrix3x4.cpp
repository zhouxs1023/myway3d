#include "MWMatrix3x4.h"

namespace Myway
{


inline Mat3x4::Mat3x4()
{
}

inline Mat3x4::~Mat3x4()
{
}

inline Mat3x4::Mat3x4(float m11, float m12, float m13, float m14, 
                      float m21, float m22, float m23, float m24, 
                      float m31, float m32, float m33, float m34)
: _11(m11), _12(m12), _13(m13), _14(m14), 
  _21(m21), _22(m22), _23(m23), _24(m24),
  _31(m31), _32(m32), _33(m33), _34(m34)
{
}

inline Mat3x4::Mat3x4(const Mat3x4 & m)
{
    Memcpy(this, &m, sizeof(Mat3x4));
}

inline Mat3x4 Mat3x4::operator -() const
{
    return Mat3x4(-_11, -_12, -_13, -_14,
                  -_21, -_22, -_23, -_24,
                  -_31, -_32, -_33, -_34);
}

inline Mat3x4 Mat3x4::operator +(const Mat3x4 & mat) const
{
    return Mat3x4(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
                  _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
                  _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34);
}

inline Mat3x4 Mat3x4::operator -(const Mat3x4 & mat) const
{
    return Mat3x4(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
                  _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
                  _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34);
}

inline Mat3x4 & Mat3x4::operator +=(const Mat3x4 & mat)
{
    _11 += mat._11, _12 += mat._12, _13 += mat._13, _14 += mat._14;
    _21 += mat._21, _22 += mat._22, _23 += mat._23, _24 += mat._24;
    _31 += mat._31, _32 += mat._32, _33 += mat._33, _34 += mat._34;

    return *this;
}

inline Mat3x4 & Mat3x4::operator -=(const Mat3x4 & mat)
{
    _11 -= mat._11, _12 -= mat._12, _13 -= mat._13, _14 -= mat._14;
    _21 -= mat._21, _22 -= mat._22, _23 -= mat._23, _24 -= mat._24;
    _31 -= mat._31, _32 -= mat._32, _33 -= mat._33, _34 -= mat._34;

    return *this;
}

inline bool Mat3x4::operator ==(const Mat3x4 & m) const
{
    return _11 == m._11 && _12 == m._12 && _13 == m._13 && _14 == m._14 &&
           _21 == m._21 && _22 == m._22 && _23 == m._23 && _24 == m._24 &&
           _31 == m._31 && _32 == m._32 && _33 == m._33 && _34 == m._34;
}

inline bool Mat3x4::operator !=(const Mat3x4 & m) const
{
    return _11 != m._11 || _12 != m._12 || _13 != m._13 || _14 != m._14 ||
           _21 != m._21 || _22 != m._22 || _23 != m._23 || _24 != m._24 ||
           _31 != m._31 || _32 != m._32 || _33 != m._33 || _34 != m._34;
}

inline float* Mat3x4::operator [](int index)
{
    assert(index < 3);
    return m[index];
}

inline const float* Mat3x4::operator [](int index) const
{
    assert(index < 3);
    return m[index];
}


}