#pragma once

#include "MWMath.h"

namespace Myway
{

template <int SIZE>
class Value 
{
	DECLARE_ALLOC();

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

		VT_USER
    };

public:
    Value()
	{
		mType = VT_UNKNOWN;
	}

	Value(bool b)
	{
		mType = VT_BOOL;
		Memcpy(mData, &b, sizeof(bool));
	}

    Value(int x)
	{
		mType = VT_INT;
		Memcpy(mData, &x, sizeof(int));
	}

    Value(float x)
	{
		mType = VT_FLOAT;
		Memcpy(mData, &x, sizeof(float));
	}

    Value(const Vec3 & v)
	{
		mType = VT_VECTOR3;
		Memcpy(mData, &v, sizeof(Vec3));
	}

    Value(const Vec4 & v)
	{
		mType = VT_VECTOR4;
		Memcpy(mData, &v, sizeof(Vec4));
	}

    Value(const char * x)
	{
		mType = VT_STRING;

		Strcpy(mData, SIZE, x);
	}

    Value(const Value & rk)
	{
		*this == mData;
	}

    ~Value()
	{
	}

    void SetBool(bool b)
	{
		mType = VT_BOOL;
		Memcpy(mData, &b, sizeof(bool));
	}

    void SetInt(int x)
	{
		mType = VT_INT;
		Memcpy(mData, &x, sizeof(int));
	}

    void SetFloat(float x)
	{
		mType = VT_FLOAT;
		Memcpy(mData, &x, sizeof(float));
	}

    void SetVector3(const Vec3 & v)
	{
		mType = VT_VECTOR3;
		Memcpy(mData, &v, sizeof(Vec3));
	}

    void SetVector4(const Vec4 & v)
	{
		mType = VT_VECTOR4;
		Memcpy(mData, &v, sizeof(Vec3));
	}

    void SetString(const char * str)
	{
		mType = VT_STRING;
		Strcpy(mData, SIZE, x);
	}

    void SetPtr(const void * ptr)
	{
		mType = VT_PTR;
		t_ptr _ptr = (int)ptr;
		Memcpy(mData, &_ptr, sizeof(t_ptr));
	}

	void SetUser(const void * data, int size)
	{
		d_assert (size <= SIZE);

		mType = VT_USER;
		Memcpy(mData, data, size);
	}

    int GetType() const
	{
		return mType;
	}

    bool ToBool() const
	{
		d_assert (mType == VT_bool);
		return *(const bool *)mData;
	}

    int ToInt() const
	{
		d_assert (mType == VT_INT);
		return *(const int *)mData;
	}

    float ToFloat() const
	{
		d_assert (mType == VT_FLOAT);
		return *(const float *)mData;
	}

    Vec3 ToVector3() const
	{
		d_assert (mType == VT_VECTOR3);
		return *(const Vec3 *)mData;
	}

    Vec4 ToVector4() const
	{
		d_assert (mType == VT_VECTOR4);
		return *(const Vec4 *)mData;
	}

    const char * ToString() const
	{
		d_assert (mType == VT_STRING);
		return mData;
	}

    void * ToPtr() const
	{
		d_assert (mType == VT_PTR);

		t_ptr ptr = *(const t_ptr *)mData;

		return (void*)ptr;
	}

	void * ToUser() const
	{
		d_assert (mType == VT_USER);
		return mData;
	}

    Value & operator =(const Value & rk)
	{
		mType = mType;
		Memcpy(mData, rk.mData, SIZE);

		return *this;
	}

    bool operator == (const Value & rk) const
	{
		d_assert (mType == rk.mType);

		switch (mType)
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

    bool operator != (const Value & rk) const
	{
		d_assert (mType == rk.mType);

		switch (mType)
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

    bool operator >  (const Value & rk) const
	{
		d_assert (mType == rk.mType);

		switch (mType)
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

    bool operator >= (const Value & rk) const
	{
		d_assert (mType == rk.mType);

		switch (mType)
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

    bool operator <  (const Value & rk) const
	{
		d_assert (mType == rk.mType);

		switch (mType)
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

    bool operator <= (const Value & rk) const
	{
		d_assert (mType == rk.mType);

		switch (mType)
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

protected:
    int			mType;
    char        mData[SIZE];
};

}
