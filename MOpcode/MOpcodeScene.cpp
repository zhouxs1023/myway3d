#include "MOpcodeScene.h"
#include "MWWorld.h"
#include "MWEnvironment.h"
#include "Opcode.h"

namespace Myway {

	Opcode::CollisionFaces * MOpcodeScene::mQueryResult = NULL;

	MOpcodeScene::MOpcodeScene()
	{
		mQueryResult = new Opcode::CollisionFaces;
	}

	MOpcodeScene::~MOpcodeScene()
	{
		safe_delete (mQueryResult);

		d_assert (mColObjs.Size() == 0);
	}

	IColObjPtr MOpcodeScene::AddColMesh(void * uId, ColMesh * colMesh, float scale)
	{
		MOpcodeMesh * mesh = new MOpcodeMesh(uId, scale);

		mesh->Build(colMesh);

		mColObjs.PushBack(mesh);

		return mesh;
	}

	IColObjPtr MOpcodeScene::GetColMesh(void * uId, float scale)
	{
		for (int i = 0; i < mColObjs.Size(); ++i)
		{
			float colScale = mColObjs[i]->GetScale();

			if (mColObjs[i]->GetUId() == uId && Math::Abs(colScale - scale) < Math::EPSILON_E4)
			{
				return mColObjs[i];
			}
		}

		return NULL;
	}

	void MOpcodeScene::RemoveColMesh(IColObj * obj)
	{
		for (int i = 0; i < mColObjs.Size(); ++i)
		{
			if (mColObjs[i] == obj)
			{
				delete obj;
				mColObjs.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	void MOpcodeScene::AddNode(SceneNode * sceneNode, IColObjPtr colObj)
	{
		d_assert (sceneNode->GetPhyData() == NULL);

		MOpcodeNode * opNode = new MOpcodeNode(sceneNode);

		opNode->SetColObj(colObj);

		sceneNode->SetPhyData(opNode);
	}

	void MOpcodeScene::RemoveNode(SceneNode * sceneNode)
	{
		if (sceneNode->GetPhyData() == NULL)
			return ;

		MOpcodeNode * opNode = (MOpcodeNode *)sceneNode->GetPhyData();

		delete opNode;

		sceneNode->SetPhyData(NULL);
	}

	void MOpcodeScene::OnNodeScaleChanged(SceneNode * sceneNode)
	{
		if (sceneNode->GetPhyData() == NULL)
			 return ;

		MOpcodeNode * opNode = (MOpcodeNode *)sceneNode->GetPhyData();
		
		IColObjPtr colObj = opNode->GetColObj();

		float scale = sceneNode->GetScale().x;

		if (colObj->GetType() == CT_Mesh && !colObj->IsSameScale(scale))
		{
			IColObjPtr obj = GetColMesh(colObj->GetUId(), scale);

			if (obj == NULL)
			{
				obj = ((MOpcodeMesh *)colObj.c_ptr())->Clone(scale);

				mColObjs.PushBack(obj.c_ptr());

				opNode->SetColObj(obj);
			}

		}
	}

	

	PhyHitInfo MOpcodeScene::RayTrace(const Ray & ray, float dist, int flag, bool ifNoPhyData)
	{
		PhyHitInfo result;

		Array<Scene::TraceInfo> traceArray;

		Scene::TraceInfo::SortOp op;

		// test on terrain
		{
			Terrain * terrain = Environment::Instance()->GetTerrain();

			if (terrain && terrain->GetConfig().phyFlags.TestFlags(flag))
			{
				PhyHitInfo hitInfo;

				if (terrain->RayTrace(hitInfo, ray, dist) && hitInfo.Distance < result.Distance)
				{
					result = hitInfo;
				}
			}
		}

		World::Instance()->RayTracing(ray, dist, traceArray, flag);

		if (traceArray.Size() == 0)
			return result;

		Sort(&traceArray[0], traceArray.Size(), op);
		
		{
			Array<Scene::TraceInfo>::Iterator whr = traceArray.Begin();
			Array<Scene::TraceInfo>::Iterator end = traceArray.End();

			while (whr != end)
			{
				if (result.node && result.Distance < whr->dist)
					break;

				SceneNode * node = whr->node;

				if (node->GetPhyData())
				{
					PhyHitInfo hitInfo;
					MOpcodeNode * opNode = (MOpcodeNode *)node->GetPhyData();

					if (opNode->RayTrace(hitInfo, ray) && hitInfo.Distance < result.Distance)
					{
						result = hitInfo;
						result.node = opNode->GetSceneNode();
					}
				}
				else if (ifNoPhyData)
				{
					if (!result.node || whr->dist < result.Distance)
					{
						result.Hitted = true;
						result.node = node;
						result.Distance = whr->dist;
						result.Normal = Vec3::Zero;
						result.MaterialId = -1;
					}
				}

				++whr;
			}
		}

		

		return result;
	}
}