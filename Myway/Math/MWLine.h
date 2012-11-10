#pragma once

#include "MWVector2.h"
#include "MWVector3.h"
#include "MWVector4.h"

namespace Myway
{

template <class T>
struct Line
{
    T p0;
    T p1;
};

typedef Line<Vec2>    Line2;
typedef Line<Vec3>    Line3;
typedef Line<Vec4>    Line4;

}