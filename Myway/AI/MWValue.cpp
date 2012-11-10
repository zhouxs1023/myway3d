#include "MWValue.h"

namespace Myway
{

Value::Value()
{
    m_type = VT_UNKNOWN;

    Memzero(m_data, SIZE);
}

Value::Value(int x)
{
    m_type = VT_INT;

    Memcpy(m_data, &x, sizeof(int));
}

Value::Value(float x)
{
    m_type = VT_FLOAT;

    Memcpy(m_data, &x, sizeof(float));
}

Value::Value(const Vec3 & v)
{
    m_type = VT_VECTOR3;

    Memcpy(m_data, &v, sizeof(Vec3));
}

Value::Value(const Vec4 & v)
{
    m_type = VT_VECTOR4;

    Memcpy(m_data, &v, sizeof(Vec4));
}

Value::Value(float x, float y, float z)
{
    Vec3 v(x, y, z);

    m_type = VT_VECTOR3;

    Memcpy(m_data, &v, sizeof(Vec3));
}

Value::Value(float x, float y, float z, float w)
{
    Vec4 v(x, y, z, w);

    m_type = VT_VECTOR4;

    Memcpy(m_data, &v, sizeof(Vec4));
}

Value::Value(const char * x)
{
    m_type = VT_STRING;

    Strcpy(m_data, SIZE, x);
}

Value::Value(const Value & rk)
{
    *this == m_data;
}

Value::~Value()
{
}

void Value::Setbool(bool b)
{
    m_type = VT_bool;

    Memcpy(m_data, &b, sizeof(bool));
}

void Value::SetInt(int x)
{
    m_type = VT_INT;

    Memcpy(m_data, &x, sizeof(int));
}

void Value::SetFloat(float x)
{
    m_type = VT_FLOAT;

    Memcpy(m_data, &x, sizeof(float));
}

void Value::SetVector3(float x, float y, float z)
{
    Vec3 v(x, y, z);

    m_type = VT_VECTOR3;

    Memcpy(m_data, &v, sizeof(Vec3));
}

void Value::SetVector4(float x, float y, float z, float w)
{
    Vec4 v(x, y, z, w);

    m_type = VT_VECTOR4;

    Memcpy(m_data, &v, sizeof(Vec4));
}

void Value::SetVector3(const Vec3 & v)
{
    m_type = VT_VECTOR3;

    Memcpy(m_data, &v, sizeof(Vec3));
}

void Value::SetVector4(const Vec4 & v)
{
    m_type = VT_VECTOR4;

    Memcpy(m_data, &v, sizeof(Vec4));
}

void Value::SetString(const char * x)
{
    m_type = VT_STRING;

    Strcpy(m_data, SIZE, x);
}

void Value::SetPtr(const void * ptr)
{
    m_type = VT_PTR;

    Memcpy(m_data, &ptr, sizeof(void *));
}


int Value::GetType() const
{
    return m_type;
}

bool Value::Tobool() const
{
    d_assert (m_type == VT_bool);

    return *(const bool *)m_data;
}

int Value::ToInt() const
{
    d_assert (m_type == VT_INT);

    return *(const int *)m_data;
}

float Value::ToFloat() const
{
    d_assert (m_type == VT_FLOAT);

    return *(const float *)m_data;
}

Vec3 Value::ToVector3() const
{
    d_assert (m_type == VT_VECTOR3);

    return *(const Vec3 *)m_data;
}

Vec4 Value::ToVector4() const
{
    d_assert (m_type == VT_VECTOR4);

    return *(const Vec4 *)m_data;
}

const char * Value::ToString() const
{
    d_assert (m_type == VT_STRING);

    return m_data;
}

void * Value::ToPtr() const
{
    d_assert (m_type == VT_PTR);

    int ptr = *(const int *)m_data;

    return (void*)ptr;
}

Value & Value::operator =(const Value & rk)
{
    m_type = m_type;
    Memcpy(m_data, rk.m_data, SIZE);

    return *this;
}

bool Value::operator == (const Value & rk) const
{
    d_assert (m_type == rk.m_type);

    switch (m_type)
    {
    case VT_INT:
        return ToInt() == rk.ToInt();

    case VT_FLOAT:
        return ToFloat() == rk.ToFloat();

    case VT_VECTOR3:
        return ToVector3() == rk.ToVector3();

    case VT_VECTOR4:
        return ToVector4() == rk.ToVector4();

    case VT_STRING:
        return Strcmp(ToString(), rk.ToString()) == 0;

    case VT_PTR:
        return ToPtr() == rk.ToPtr();
    }

    return false;
}

bool Value::operator != (const Value & rk) const
{
    d_assert (m_type == rk.m_type);

    switch (m_type)
    {
    case VT_INT:
        return ToInt() != rk.ToInt();

    case VT_FLOAT:
        return ToFloat() != rk.ToFloat();

    case VT_VECTOR3:
        return ToVector3() != rk.ToVector3();

    case VT_VECTOR4:
        return ToVector4() != rk.ToVector4();

    case VT_STRING:
        return Strcmp(ToString(), rk.ToString()) != 0;

    case VT_PTR:
        return ToPtr() != rk.ToPtr();
    }

    return false;
}

bool Value::operator >  (const Value & rk) const
{
    d_assert (m_type == rk.m_type);

    switch (m_type)
    {
    case VT_INT:
        return ToInt() > rk.ToInt();

    case VT_FLOAT:
        return ToFloat() > rk.ToFloat();

    case VT_VECTOR3:
        return ToVector3() > rk.ToVector3();

    case VT_VECTOR4:
        return ToVector4() > rk.ToVector4();

    case VT_STRING:
        return Strcmp(ToString(), rk.ToString()) > 0;

    case VT_PTR:
        return ToPtr() > rk.ToPtr();
    }

    return false;
}

bool Value::operator >= (const Value & rk) const
{
    d_assert (m_type == rk.m_type);

    switch (m_type)
    {
    case VT_INT:
        return ToInt() >= rk.ToInt();

    case VT_FLOAT:
        return ToFloat() >= rk.ToFloat();

    case VT_VECTOR3:
        return ToVector3() >= rk.ToVector3();

    case VT_VECTOR4:
        return ToVector4() >= rk.ToVector4();

    case VT_STRING:
        return Strcmp(ToString(), rk.ToString()) >= 0;

    case VT_PTR:
        return ToPtr() >= rk.ToPtr();
    }

    return false;
}

bool Value::operator <  (const Value & rk) const
{
    d_assert (m_type == rk.m_type);

    switch (m_type)
    {
    case VT_INT:
        return ToInt() < rk.ToInt();

    case VT_FLOAT:
        return ToFloat() < rk.ToFloat();

    case VT_VECTOR3:
        return ToVector3() < rk.ToVector3();

    case VT_VECTOR4:
        return ToVector4() < rk.ToVector4();

    case VT_STRING:
        return Strcmp(ToString(), rk.ToString()) < 0;

    case VT_PTR:
        return ToPtr() < rk.ToPtr();
    }

    return false;
}

bool Value::operator <= (const Value & rk) const
{
    d_assert (m_type == rk.m_type);

    switch (m_type)
    {
    case VT_INT:
        return ToInt() <= rk.ToInt();

    case VT_FLOAT:
        return ToFloat() <= rk.ToFloat();

    case VT_VECTOR3:
        return ToVector3() <= rk.ToVector3();

    case VT_VECTOR4:
        return ToVector4() <= rk.ToVector4();

    case VT_STRING:
        return Strcmp(ToString(), rk.ToString()) <= 0;

    case VT_PTR:
        return ToPtr() <= rk.ToPtr();
    }

    return false;
}

}