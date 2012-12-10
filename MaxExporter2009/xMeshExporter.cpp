#include "stdafx.h"

#include "xMeshExporter.h"
#include "xExporterConfig.h"

xMeshExporter::xMeshExporter(ExpInterface * ei, Interface * i)
	: mExpInterface(ei)
	, mInterface(i)
	, mGameScene(NULL)
	, mBoneIndex(0)
{
}

xMeshExporter::~xMeshExporter()
{
}

int xMeshExporter::callback(INode *node)
{
	TimeValue start = mInterface->GetAnimRange().Start();
	Object *obj = node->EvalWorldState(start).obj;
	Class_ID cid = obj->ClassID();

	// nodes that have Biped controllers are bones -- ignore the ones that are dummies
	if (cid == Class_ID(DUMMY_CLASS_ID, 0))
		return TREE_CONTINUE;

	Control *c = node->GetTMController();
	if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
		(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
		(c->ClassID() == FOOTPRINT_CLASS_ID)) {

			if (node->GetParentNode() != NULL)
			{
				mBoneIndexMap.Insert(node->GetName(), mBoneIndex++);
			}

			return TREE_CONTINUE;
	}

	if (!obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		return TREE_CONTINUE;

	if (xExportConfig::Instance()->IsExportSelected())
	{
		if (node->Selected())
		{
			mNodeTab.Append(1, &node);
		}
	}
	else
	{
		mNodeTab.Append(1, &node);
	}

	return TREE_CONTINUE;
}

void xMeshExporter::Build()
{
	try 
	{
		if (mGameScene)
		{
			mGameScene->ReleaseIGame();
			mGameScene = 0;
		}

		mExpInterface->theScene->EnumTree(this);

		mGameScene = GetIGameInterface();
		IGameConversionManager* cm = GetConversionManager();
		cm->SetCoordSystem(IGameConversionManager::IGAME_OGL);
		mGameScene->InitialiseIGame(mNodeTab, false);
		mGameScene->SetStaticFrame(0);
		int nodeCount = mGameScene->GetTopLevelNodeCount();

		if (nodeCount == 0) {
			MessageBox(GetActiveWindow(), "No nodes available!", "Error", MB_ICONINFORMATION);
			mGameScene->ReleaseIGame();
			return ;
		}

		int nodeIdx = 0;
		while (nodeIdx < nodeCount)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(nodeIdx);
			IGameObject* obj = node->GetIGameObject();

			obj->InitializeData();

			IGameMaterial* mtl = node->GetNodeMaterial();



			node->ReleaseIGameObject();
			nodeIdx++;
		}

		mGameScene->ReleaseIGame();
	}
	catch (...) 
	{
		MessageBox(GetActiveWindow(), "Error!", "Error", MB_ICONEXCLAMATION);

		if (mGameScene)
			mGameScene->ReleaseIGame();
	}
}