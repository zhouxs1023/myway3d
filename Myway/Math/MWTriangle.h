#pragma once

#include "MWVector2.h"
#include "MWVector3.h"
#include "MWVector4.h"

namespace Myway
{

template <class T>
class Triangle
{
    DECLARE_ALLOC();

public:
    Triangle() {}
    Triangle(T p1, T p2, T p3) : a(p1), b(p2), c(p3) {}
    ~Triangle() {}

    T a;
    T b;
    T c;
};

typedef Triangle<float>   Tri1;
typedef Triangle<Vec2>    Tri2;
typedef Triangle<Vec3>    Tri3;
typedef Triangle<Vec4>    Tri4;

}