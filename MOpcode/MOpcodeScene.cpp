#include "MOpcodeScene.h"
#include "MWWorld.h"
#include "MWEnvironment.h"

namespace Myway {

	Opcode::CollisionFaces * MOpcodeScene::mQueryResult = NULL;

	MOpcodeScene::MOpcodeScene()
	{
		mTerrain = NULL;
		mQueryResult = new Opcode::CollisionFaces;
	}

	MOpcodeScene::~MOpcodeScene()
	{
		safe_delete (mQueryResult);

		d_assert (mTerrain == NULL);
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

	void MOpcodeScene::CreateTerrain()
	{
		d_assert (mTerrain == NULL);
		mTerrain = new MOpcodeTerrain(NULL, 1);
		mTerrain->Build();
	}

	void MOpcodeScene::DestroyTerrain()
	{
		safe_delete(mTerrain);
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

	

	void MOpcodeScene::RayCheck(HitInfoSetArray & hitInfos, const Ray & ray, float dist, int flag, bool ifNoPhyData)
	{
		Array<Scene::TraceInfo> traceArray;

		Scene::TraceInfo::SortOp op;

		// test on terrain
		/*{
			Terrain * terrain = Environment::Instance()->GetTerrain();

			if (terrain && terrain->GetConfig().phyFlags.TestFlags(flag))
			{
				PhyHitInfo hitInfo;

				if (terrain->RayTrace(hitInfo, ray, dist) && hitInfo.Distance < result.Distance)
				{
					result = hitInfo;
				}
			}
		}*/

		HitInfoSet info;

		World::Instance()->RayTracing(ray, dist, traceArray, flag);

		if (traceArray.Size() == 0)
			return ;

		Sort(&traceArray[0], traceArray.Size(), op);
		
		{
			Array<Scene::TraceInfo>::Iterator whr = traceArray.Begin();
			Array<Scene::TraceInfo>::Iterator end = traceArray.End();

			while (whr != end)
			{
				if (info.node && info.r_distance < whr->dist)
					break;

				SceneNode * node = whr->node;

				if (node->GetPhyData())
				{
					HitInfoSet t_info;
					MOpcodeNode * opNode = (MOpcodeNode *)node->GetPhyData();

					if (opNode->RayCheck(t_info, ray) && t_info.r_distance < info.r_distance)
					{
						info = t_info;
						info.node = opNode->GetSceneNode();
					}
				}
				else if (ifNoPhyData)
				{
					if (whr->dist < info.r_distance)
					{
						info.node = node;
						info.r_distance = whr->dist;
						info.r_that_position = ray.GetPosition(info.r_distance);
						info.r_that_normal = Vec3::Zero;
					}
				}

				++whr;
			}

			hitInfos.PushBack(info);
		}
	}
}