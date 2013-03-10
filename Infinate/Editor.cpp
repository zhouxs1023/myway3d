#include "stdafx.h"

#include "Editor.h"
#include "xScene.h"
#include "xMesh.h"
#include "xLight.h"
#include "xOcean.h"
#include "xTerrain.h"
#include "xTree.h"


namespace Infinite {

	IMP_SLN(Editor);

	Editor::Editor()
	{
		INIT_SLN;
	}

	Editor::~Editor()
	{
		SHUT_SLN;
	}

	void Editor::Init()
	{
		SceneNode * cam = World::Instance()->MainCameraNode();

		cam->SetPosition(0, 200, 0);

		Quat q; 
		q.FromAxis(Vec3::UnitX, Vec3::UnitZ, Vec3::NegUnitY);
		cam->SetOrientation(q);

		World::Instance()->Resize(2048, 2048, 2048);

		mHelperShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Helper", "Helper.ShaderLib");

		mShapeMgr.AddFactory(new xMeshFactory);
		mShapeMgr.AddFactory(new xPointLightFactory);
		mShapeMgr.AddFactory(new xTreeFactory);
		mShapeMgr.AddFactory(new xOceanFactory);
		mShapeMgr.AddFactory(new xTerrainFactory);

		xEvent::OnInit(NULL, NULL);
		xEvent::OnInitUI(NULL, NULL);
	}

	void Editor::Shutdown()
	{
		xEvent::OnShutdown(NULL, NULL);
	}


	void Editor::SetSelectedShape(Shape * obj)
	{
		mSelectedShapes.Clear();

		if (obj == NULL)
		{
			xEvent::OnUnSelectObj(NULL, NULL);

			return ;
		}

		mSelectedShapes.PushBack(obj);

		xEvent::OnSelectObj(NULL, NULL);
	}

	void Editor::SetSelectedShapes(Shape ** objs, int size)
	{
		mSelectedShapes.Clear();

		for (int i = 0; i < size; ++i)
		{
			mSelectedShapes.PushBack(objs[i]);
		}

		xEvent::OnSelectObj(NULL, NULL);
	}

	int Editor::GetSelectedShapeSize()
	{
		return mSelectedShapes.Size();
	}

	Shape * Editor::GetSelectedShape(int index)
	{
		return GetSelectedShapeSize() ? mSelectedShapes[index] : NULL;
	}

	Vec3 Editor::GetHitPosition(float x, float y)
	{
		Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

		Vec3 pos = ray.origin + ray.direction * 80;

		return pos;
	}

}
