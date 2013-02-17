#include "MOpcodeNode.h"
#include "MOpcodeScene.h"
#include "Opcode.h"

namespace Myway {

	MOpcodeMesh::MOpcodeMesh(void * uid, float scale)
		: IColObj(uid, scale)
	{
		mMeshInterface = new Opcode::MeshInterface;
		mCollisionModel = 0;
	}

	MOpcodeMesh::~MOpcodeMesh()
	{
		safe_delete (mMeshInterface);
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
			mMeshInterface->SetNbVertices(mVertices.Size());
			mMeshInterface->SetNbTriangles(mIndices.Size() / 3);
			mMeshInterface->SetPointers((IceMaths::IndexedTriangle*)&mIndices[0], (IceMaths::Point*)&mVertices[0]);

			Opcode::OPCODECREATE info;
			info.mIMesh = mMeshInterface;
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
			mMeshInterface->SetNbVertices(mVertices.Size());
			mMeshInterface->SetNbTriangles(mIndices.Size() / 3);
			mMeshInterface->SetPointers((IceMaths::IndexedTriangle*)&mIndices[0], (IceMaths::Point*)&mVertices[0]);

			Opcode::OPCODECREATE info;
			info.mIMesh = mMeshInterface;
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

	bool MOpcodeMesh::RayTrace(PhyHitInfo & info, const Ray & ray, const Mat4 & worldTm, unsigned int * cache)
	{
		if (!mCollisionModel)
			return false;

		Opcode::CollisionFaces * queryResult = MOpcodeScene::_getCollisionFaces();

		Opcode::RayCollider collider;

		IceMaths::Ray _ray;
		_ray.mDir.x = ray.direction.x;
		_ray.mDir.y = ray.direction.y;
		_ray.mDir.z = ray.direction.z;

		_ray.mOrig.x = ray.origin.x;
		_ray.mOrig.y = ray.origin.y;
		_ray.mOrig.z = ray.origin.z;

		collider.SetClosestHit(true);
		collider.SetCulling(true);
		collider.SetDestination(queryResult);

		if (collider.Collide(_ray, *mCollisionModel, (const IceMaths::Matrix4x4 *)&worldTm, cache))
		{
			int bnFaces = queryResult->GetNbFaces();
			if( bnFaces == 0 )
				return false;

			Opcode::VertexPointers vp;

			mMeshInterface->GetTriangle(vp, queryResult->GetFaces()->mFaceID);

			info.Distance = queryResult->GetFaces()->mDistance;
			info.MaterialId = -1;//mMateirals[mQueryResult->GetFaces()->mFaceID];

			{			
				const IceMaths::Point& p0 = *vp.Vertex[0];
				const IceMaths::Point& p1 = *vp.Vertex[1];
				const IceMaths::Point& p2 = *vp.Vertex[2];

				// Compute normal direction
				IceMaths::Point Normal = (p2 - p1) ^ (p0 - p1);
				info.Normal.x = Normal.x;
				info.Normal.y = Normal.y;
				info.Normal.z = Normal.z;
				info.Normal.NormalizeL();
			}

			info.Normal.TransformN(worldTm);

			info.Hitted = true;

			return true;
		}

		return false;
	}




















	MOpcodeNode::MOpcodeNode(SceneNode * node)
		: mSceneNode(node)
		, mColObj(NULL)
		, mCache(0)
	{
	}

	MOpcodeNode::~MOpcodeNode()
	{
	}

	bool MOpcodeNode::RayTrace(PhyHitInfo & info, const Ray & ray)
	{
		d_assert (mColObj != NULL);

		Mat4 worldTM;

		worldTM.MakeTransform(mSceneNode->GetWorldPosition(), mSceneNode->GetWorldOrientation(), Vec3::Unit);

		return mColObj->RayTrace(info, ray, worldTM, &mCache);
	}


}