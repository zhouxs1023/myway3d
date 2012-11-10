#pragma once

#include "MWVector2.h"
#include "MWVector3.h"
#include "MWVector4.h"

namespace Myway
{

struct MW_ENTRY Rect
{
    DECLARE_ALLOC();

    int x1, y1;
    int x2, y2;
};

struct MW_ENTRY RectF
{
    DECLARE_ALLOC();

    RectF() {}
    RectF(float xx1, float yy1, float xx2, float yy2) { x1 = xx1; y1 = yy1; x2 = xx2; y2 = yy2; }

    float x1, y1;
    float x2, y2;
};

struct MW_ENTRY Size
{
    DECLARE_ALLOC();

    int x, y, z;
};

struct MW_ENTRY SizeF
{
    DECLARE_ALLOC();

    float x, y, z;
};

}