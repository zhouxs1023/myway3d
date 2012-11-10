#pragma once

#include "MWCore.h"

namespace Myway
{

template <class T, int num>
class VectorX : public AllocObj
{
public:
    VectorX();
    ~VectorX();
    VectorX(const T * mem);
    VectorX(const VectorX & v);

    VectorX& operator = (const VectorX & v);
    operator T* ();
    operator const T* () const;

    bool operator ==(const VectorX & v) const;
    bool operator !=(const VectorX & v) const;
    VectorX operator -() const;
    VectorX operator + (const VectorX & v) const;
    VectorX operator - (const VectorX & v) const;
    VectorX operator * (const VectorX & v) const;
    VectorX operator / (const VectorX & v) const;
    VectorX operator * (T i) const;
    VectorX operator / (T i) const;

    VectorX& operator += (const VectorX & v);
    VectorX& operator -= (const VectorX & v);
    VectorX& operator *= (T i);
    VectorX& operator /= (T i);

    T& operator [](int index);
    const T& operator [](int index) const;

    friend VectorX operator * (T i, const VectorX & v);
    friend VectorX operator / (T i, const VectorX & v);

public:
    T m[num];
};

#include "MWVectorX.inl"

typedef VectorX<int, 2>     Veci2;
typedef VectorX<int, 3>     Veci3;
typedef VectorX<int, 4>     Veci4;

struct Point2i
{
	int x, y;

	Point2i() {}
	Point2i(int m, int n) : x(m), y(n) {}
};

struct Point2f
{
	float x, y;

	Point2f() {}
	Point2f(float m, float n) : x(m), y(n) {}
};


}