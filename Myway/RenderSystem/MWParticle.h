#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

struct Particle : public AllocObj
{
    Vec3        size;
    Vec3        position;
    Vec3        direction;
    float       velocity;
    Color4      color;
    float       life;
    float       maxlife;
};

}