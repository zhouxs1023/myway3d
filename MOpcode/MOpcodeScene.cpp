#include "MOpcodeScene.h"
#include "Opcode.h"

namespace Myway {

	MOpcodeScene::MOpcodeScene()
	{
		mQueryResult = new Opcode::CollisionFaces;
		mMeshInterface = new Opcode::MeshInterface;
		mCollisionModel = 0;
	}

	MOpcodeScene::~MOpcodeScene()
	{
		safe_delete (mQueryResult);
		safe_delete (mMeshInterface);
		safe_delete (mCollisionModel);
	}

	void MOpcodeScene::AddMesh(ColMesh * mesh, const Mat4 & form)
	{
		Array<Vec3> & Verts = mesh->GetPositions();

		int startVertex = mVertices.Size();

		for (int i = 0; i < Verts.Size(); ++i)
			mVertices.PushBack(Verts[i]);


		Array<ColMesh::STri> & Tris = mesh->GetTrangles();

		for (int i = 0; i < Tris.Size(); ++i)
		{

			mTriangles.PushBack(Tris[i].i0);
			mTriangles.PushBack(Tris[i].i1);
			mTriangles.PushBack(Tris[i].i2);

			mMateirals.PushBack(Tris[i].mtlId);
		}
	}

	void MOpcodeScene::Build()
	{
		safe_delete(mCollisionModel);

		if(mTriangles.Size() > 0 && mVertices.Size() > 0) {
			mCollisionModel = new Opcode::Model;
			mMeshInterface->SetNbVertices(mVertices.Size());
			mMeshInterface->SetNbTriangles(mTriangles.Size() / 3);
			mMeshInterface->SetPointers((IceMaths::IndexedTriangle*)&mTriangles[0], (IceMaths::Point*)&mVertices[0]);

			Opcode::OPCODECREATE info;
			info.mIMesh = mMeshInterface;
			info.mCanRemap = true;
			mCollisionModel->Build(info);
		}	
	}

	void MOpcodeScene::Clear()
	{
		safe_delete (mCollisionModel);
		mVertices.Clear();
		mTriangles.Clear();
		mMateirals.Clear();
	}

	bool MOpcodeScene::RayTrace(const Ray & ray, float * dist, unsigned int * cache, int * mtlId, Vec3 * colPos, Vec3 * colNml)
	{
		if (!mCollisionModel)
			return false;

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
		collider.SetDestination(mQueryResult);

		if (collider.Collide(_ray, *mCollisionModel, 0, cache))
		{

			int bnFaces = mQueryResult->GetNbFaces();
			if( bnFaces == 0 )
				return false;

			Opcode::VertexPointers vp;

			mMeshInterface->GetTriangle(vp, mQueryResult->GetFaces()->mFaceID);

			if (dist)
			{
				*dist = mQueryResult->GetFaces()->mDistance;
			}

			if (mtlId)
			{
				*mtlId = mMateirals[mQueryResult->GetFaces()->mFaceID];
			}

			if(colPos)
			{
				// calculates Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
				const Opcode::CollisionFace * pColFace = mQueryResult->GetFaces();

				const IceMaths::Point& p0 = *vp.Vertex[0];
				const IceMaths::Point& p1 = *vp.Vertex[1];
				const IceMaths::Point& p2 = *vp.Vertex[2];

				IceMaths::Point pt = p0 + pColFace->mU * (p1-p0) +	pColFace->mV * (p2-p0);

				colPos->x = pt.x;
				colPos->y = pt.y;
				colPos->z = pt.z;
			}

			if (colNml)
			{			
				const IceMaths::Point& p0 = *vp.Vertex[0];
				const IceMaths::Point& p1 = *vp.Vertex[1];
				const IceMaths::Point& p2 = *vp.Vertex[2];

				// Compute normal direction
				IceMaths::Point Normal = (p2 - p1)^(p0 - p1);
				colNml->x = Normal.x;
				colNml->y = Normal.y;
				colNml->z = Normal.z;
				colNml->NormalizeL();
			}

			return true;
		}

		return false;
	}
}