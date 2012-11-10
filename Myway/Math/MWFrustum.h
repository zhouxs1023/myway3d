#pragma once

#include "MWPlane.h"

namespace Myway
{

struct Frustum
{
    Plane       Near;
    Plane       Far;
    Plane       Left;
    Plane       Right;
    Plane       Top;
    Plane       Bottom;
};

}