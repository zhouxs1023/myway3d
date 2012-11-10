#include "stdafx.h"
#include "EntityShape.h"

namespace xInfi {

    DF_PROPERTY_BEGIN(EntityShape)
        DF_PROPERTY(EntityShape, Name, "General", "Name", PT_String, 128)
        DF_PROPERTY(EntityShape, MeshFile, "General", "File", PT_String, 128)
        DF_PROPERTY(EntityShape, Position, "Transform", "Position", PT_Vec3, 12)
        DF_PROPERTY(EntityShape, Orientation, "Transform", "Orientation", PT_Vec3, 12)
        DF_PROPERTY(EntityShape, Scale, "Transform", "Scale", PT_Vec3, 12)
    DF_PROPERTY_END();

    EntityShape::EntityShape(const char * name)
        : Shape(name)
    {
        Strcpy(Name, 128, name);

        mNode = World::Instance()->CreateSceneNode();
        mEntity = World::Instance()->CreateEntity(name);

        Position = Vec3::Zero; 
        Orientation = Vec3::Zero;
        Scale = Vec3::Unit;
    }

    EntityShape::~EntityShape()
    {
        if (mEntity)
            World::Instance()->DestroyEntity(mEntity);

        if (mNode)
            World::Instance()->DestroySceneNode(mNode);
    }

    bool EntityShape::SetPosition(float x, float y, float z)
    {
        Position = Vec3(x, y, z);
        return true;
    }

    bool EntityShape::SetOrientation(float x, float y, float z)
    {
        Orientation = Vec3(x, y, z);
        return true;
    }

    bool EntityShape::SetScale(float x, float y, float z)
    {
        Scale = Vec3(x, y, z);
        return true;
    }

    bool EntityShape::OnPropertyChanged(const Property * p)
    {
        if (p->name == "MeshFile")
        {
            _setMeshFile(MeshFile);
        }

        return true;
    }

    void EntityShape::_setName(const TString128 & name)
    {
        if (mEntity && World::Instance()->RenameEntity(name, mEntity))
        {
        }
    }

    void EntityShape::_setMeshFile(const TString128 & meshFile)
    {
        if (mEntity && mEntity->GetMesh()->GetSourceName() == meshFile)
            return ;

        mEntity->SetMesh(meshFile, "core");
    }

    void EntityShape::_setPosition(const Vec3 & position)
    {
        if (mNode)
        {
            mNode->SetPosition(position);
        }
    }

    void EntityShape::_setOrientation(const Vec3 & ort)
    {
        if (mNode)
        {
            Quat q;

            q.FromPitchYawRoll(ort.x, ort.y, ort.z);

            mNode->SetOrientation(q);
        }
    }

    void EntityShape::_setScale(const Vec3 & scale)
    {
        if (mNode)
        {
            mNode->SetScale(scale);
        }
    }


    EntityShapeCreatorListener gListener;
}