#include "stdafx.h"
#include "xMesh.h"

namespace xInfi {

    DF_PROPERTY_BEGIN(xMesh)
        DF_PROPERTY(xMesh, MeshFile, "General", "File", PT_String, 128)
    DF_PROPERTY_END();

    xMesh::xMesh(const char * name)
        : xObj(name)
    {
		Strcpy(MeshFile, 128, "temp.mesh");

        mNode = World::Instance()->CreateSceneNode();
        mEntity = World::Instance()->CreateEntity(name, MeshFile);
		mNode->Attach(mEntity);
    }

    xMesh::~xMesh()
    {
        if (mEntity)
            World::Instance()->DestroyEntity(mEntity);

        if (mNode)
            World::Instance()->DestroySceneNode(mNode);
    }

	Aabb xMesh::GetBound()
	{
		return mNode->GetWorldAabb();
	}

    bool xMesh::OnPropertyChanged(const Property * p)
    {
		if (p->name == "Name")
		{
			_setName(Name);
		}
        else if (p->name == "MeshFile")
        {
            _setMeshFile(MeshFile);
        }
		else if (p->name == "Position")
		{
			_setPosition(Position);
		}
		else if (p->name == "Orientation")
		{
			_setOrientation(Orientation);
		}
		else if (p->name == "Scale")
		{
			_setScale(Scale);
		}

        return true;
    }

    void xMesh::_setName(const TString128 & name)
    {
        World::Instance()->RenameEntity(name, mEntity);
    }

    void xMesh::_setMeshFile(const TString128 & meshFile)
    {
        if (mEntity && mEntity->GetMesh() != NULL && mEntity->GetMesh()->GetSourceName() == meshFile)
            return ;

        mEntity->SetMesh(meshFile);
    }

    void xMesh::_setPosition(const Vec3 & position)
    {
        mNode->SetPosition(position);
    }

    void xMesh::_setOrientation(const Quat & ort)
    {
		mNode->SetOrientation(ort);
    }

    void xMesh::_setScale(const Vec3 & scale)
    {
        mNode->SetScale(scale);
    }

    xMeshFactoryListener gListener;
}