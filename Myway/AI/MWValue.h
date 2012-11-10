#pragma once

#include "MWMath.h"

namespace Myway
{

class MW_ENTRY Value
{
public:
    enum Type
    {
        VT_UNKNOWN,

        VT_bool,

        VT_INT,
        VT_FLOAT,

        VT_VECTOR3,
        VT_VECTOR4,

        VT_STRING,

        VT_PTR,
    };

    static const int SIZE = 128;

public:
    Value();
    Value(int x);
    Value(float x);
    Value(const Vec3 & v);
    Value(const Vec4 & v);
    Value(float x, float y, float z);
    Value(float x, float y, float z, float w);
    Value(const char * x);
    Value(const Value & rk);
    ~Value();

    void Setbool(bool b);
    void SetInt(int x);
    void SetFloat(float x);
    void SetVector3(float x, float y, float z);
    void SetVector3(const Vec3 & v);
    void SetVector4(float x, float y, float z, float w);
    void SetVector4(const Vec4 & v);
    void SetString(const char * str);
    void SetPtr(const void * ptr);

    int GetType() const;
    bool Tobool() const;
    int ToInt() const;
    float ToFloat() const;
    Vec3 ToVector3() const;
    Vec4 ToVector4() const;
    const char * ToString() const;
    void * ToPtr() const;

    Value & operator =(const Value & rk);

    bool operator == (const Value & rk) const;
    bool operator != (const Value & rk) const;
    bool operator >  (const Value & rk) const;
    bool operator >= (const Value & rk) const;
    bool operator <  (const Value & rk) const;
    bool operator <= (const Value & rk) const;

protected:
    int			m_type;

    char        m_data[SIZE];
};

}