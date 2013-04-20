#include "stdafx.h"

#include "Editor.h"
#include "MainWorkSpace.h"

namespace MotionFX {

	IMP_SLN(Editor);

	Editor::Editor()
	{
		INIT_SLN;

		mFoucs = true;
		mMousePosition = Point2f(0, 0);

		mActor = NULL;
		mSceneNode = NULL;
	}

	Editor::~Editor()
	{
		SHUT_SLN;
	}

	void Editor::Init()
	{
		SceneNode * cam = World::Instance()->MainCameraNode();

		cam->SetPosition(0, 200, 0);

		World::Instance()->Resize(2048, 2048, 2048);

		mHelperShaderLib = ShaderLibManager::Instance()->LoadShaderLib(
			"Shaders\\Helper.ShaderLib", "Shaders\\Helper.ShaderLib");

		mActor = World::Instance()->CreateActor("EditorActor");
		mSceneNode = World::Instance()->CreateSceneNode();

		mSceneNode->Attach(mActor);

		mBackCell.Init();
	}

	void Editor::Shutdown()
	{
		World::Instance()->DestroyActor(mActor);
		World::Instance()->DestroySceneNode(mSceneNode);

		mBackCell.Shutdown();
	}

	void Editor::Update()
	{
		RenderWindow::Instance()->Update();
	}

}