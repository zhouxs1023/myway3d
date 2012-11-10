#pragma once

#include "MWMath.h"

namespace Myway
{

namespace Physics
{

struct Material
{
    float   volume;             //体积
    float   weight;             //重量
    float   friction;           //阻力
    float   restitution;        //回复里
};


enum ShapeType
{
    SHAPE_SPHERE,
    SHAPE_AABB,
    SHAPE_OBB,
    SHAPE_CAPSULE,
    SHAPE_MESH,

    _ALIGN_ENUM(ShapeType)
};

struct Shape;

MW_ENTRY class Actor : public AllocObj
{
public:
    Actor();
    ~Actor();

protected:
    Material            m_material;
    Shape *             m_shape;
};


MW_ENTRY class PhysicsSystem : public Singleton<PhysicsSystem>
{
public:
    PhysicsSystem() {}
    virtual ~PhysicsSystem() {}
};

}

}
