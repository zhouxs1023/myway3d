#include "stdafx.h"
#include "xMesh.h"

namespace xInfi {

    DF_PROPERTY_BEGIN(xMesh)
        DF_PROPERTY(xMesh, Name, "General", "Name", PT_String, 128)
        DF_PROPERTY(xMesh, MeshFile, "General", "File", PT_String, 128)
        DF_PROPERTY(xMesh, Position, "Transform", "Position", PT_Vec3, 12)
        DF_PROPERTY(xMesh, Orientation, "Transform", "Orientation", PT_Vec3, 12)
        DF_PROPERTY(xMesh, Scale, "Transform", "Scale", PT_Vec3, 12)
    DF_PROPERTY_END();

    xMesh::xMesh(const char * name)
        : xObj(name)
    {
        Strcpy(Name, 128, name);
		Strcpy(MeshFile, 128, "");

        mNode = World::Instance()->CreateSceneNode();
        mEntity = World::Instance()->CreateEntity(name);
		mNode->Attach(mEntity);

        Position = Vec3::Zero; 
        Orientation = Vec3::Zero;
        Scale = Vec3::Unit;
    }

    xMesh::~xMesh()
    {
        if (mEntity)
            World::Instance()->DestroyEntity(mEntity);

        if (mNode)
            World::Instance()->DestroySceneNode(mNode);
    }

    bool xMesh::SetPosition(float x, float y, float z)
    {
        Position = Vec3(x, y, z);
		mNode->SetPosition(x, y, z);
        return true;
    }

    bool xMesh::SetOrientation(float x, float y, float z)
    {
        Orientation = Vec3(x, y, z);
        return true;
    }

    bool xMesh::SetScale(float x, float y, float z)
    {
        Scale = Vec3(x, y, z);
        return true;
    }

    bool xMesh::OnPropertyChanged(const Property * p)
    {
        if (p->name == "MeshFile")
        {
            _setMeshFile(MeshFile);
        }

        return true;
    }

    void xMesh::_setName(const TString128 & name)
    {
        if (mEntity && World::Instance()->RenameEntity(name, mEntity))
        {
        }
    }

    void xMesh::_setMeshFile(const TString128 & meshFile)
    {
        if (mEntity && mEntity->GetMesh() != NULL && mEntity->GetMesh()->GetSourceName() == meshFile)
            return ;

        mEntity->SetMesh(meshFile, "core");
    }

    void xMesh::_setPosition(const Vec3 & position)
    {
        if (mNode)
        {
            mNode->SetPosition(position);
        }
    }

    void xMesh::_setOrientation(const Vec3 & ort)
    {
        if (mNode)
        {
            Quat q;

            q.FromPitchYawRoll(ort.x, ort.y, ort.z);

            mNode->SetOrientation(q);
        }
    }

    void xMesh::_setScale(const Vec3 & scale)
    {
        if (mNode)
        {
            mNode->SetScale(scale);
        }
    }

    xMeshFactoryListener gListener;
}