#include "MOpcodeMesh.h"
#include "MOpcodeScene.h"

namespace Myway {

	MOpcodeMesh::MOpcodeMesh(void * uid, float scale)
		: MOpcodeColObj(uid, scale)
	{
		mCollisionModel = 0;
	}

	MOpcodeMesh::~MOpcodeMesh()
	{
		safe_delete (mCollisionModel);
	}

	void MOpcodeMesh::DeleteSelf()
	{
		IPhyWorld::Instance()->RemoveColMesh(this);
	}

	void MOpcodeMesh::Build(ColMesh * mesh)
	{
		Array<Vec3> & Verts = mesh->GetPositions();

		for (int i = 0; i < Verts.Size(); ++i)
			mVertices.PushBack(Verts[i] * mScale);

		Array<int> & Tris = mesh->GetIndices();

		for (int i = 0; i < Tris.Size(); ++i)
		{
			mIndices.PushBack(Tris[i]);
		}

		safe_delete(mCollisionModel);

		if(mIndices.Size() > 0 && mVertices.Size() > 0) {
			mCollisionModel = new Opcode::Model;
			mMeshInterface.SetNbVertices(mVertices.Size());
			mMeshInterface.SetNbTriangles(mIndices.Size() / 3);
			mMeshInterface.SetPointers((IceMaths::IndexedTriangle*)&mIndices[0], (IceMaths::Point*)&mVertices[0]);

			Opcode::OPCODECREATE info;
			info.mIMesh = &mMeshInterface;
			info.mCanRemap = true;
			mCollisionModel->Build(info);
		}
	}

	void MOpcodeMesh::Build(MOpcodeMesh * rk)
	{
		Array<Vec3> & Verts = rk->mVertices;

		for (int i = 0; i < Verts.Size(); ++i)
			mVertices.PushBack(Verts[i] * (mScale / rk->mScale));

		Array<int> & Tris = rk->mIndices;

		for (int i = 0; i < Tris.Size(); ++i)
		{
			mIndices.PushBack(Tris[i]);
		}

		safe_delete(mCollisionModel);

		if(mIndices.Size() > 0 && mVertices.Size() > 0) {
			mCollisionModel = new Opcode::Model;
			mMeshInterface.SetInterfaceType(Opcode::MESH_TRIANGLE);
			mMeshInterface.SetNbVertices(mVertices.Size());
			mMeshInterface.SetNbTriangles(mIndices.Size() / 3);
			mMeshInterface.SetPointers((IceMaths::IndexedTriangle*)&mIndices[0], (IceMaths::Point*)&mVertices[0]);

			Opcode::OPCODECREATE info;
			info.mIMesh = &mMeshInterface;
			info.mCanRemap = true;
			mCollisionModel->Build(info);
		}	
	}

	IColObj * MOpcodeMesh::Clone(float scale)
	{
		d_assert (!IsSameScale(scale));

		MOpcodeMesh * mesh = new MOpcodeMesh(mUId, scale);

		mesh->Build(this);

		return mesh;
	}

}