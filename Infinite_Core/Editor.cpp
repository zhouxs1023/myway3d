#include "stdafx.h"

#include "Editor.h"
#include "xScene.h"
#include "xMesh.h"
#include "xLight.h"
#include "xOcean.h"
#include "xTerrain.h"
#include "xTree.h"
#include "MainWorkSpace.h"

namespace Infinite {

	IMP_SLN(Editor);

	Editor::Editor()
		: OnUnloadScene(xEvent::OnUnloadScene, this, &Editor::_unloadScene)
	{
		INIT_SLN;

		mOperator = eOP_Unknown;
		mFoucs = true;
		mMousePosition = Point2f(0, 0);

		mColorPanel = NULL;
		mMessageBox = NULL;
		mFileDialog = NULL;
		mPuginDialog = NULL;
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

		d_assert (mHelperShaderLib);

		mShapeMgr.AddFactory(new xMeshFactory);
		mShapeMgr.AddFactory(new xPointLightFactory);
		mShapeMgr.AddFactory(new xTreeFactory);
		mShapeMgr.AddFactory(new xOceanFactory);
		mShapeMgr.AddFactory(new xTerrainFactory);

		mColorPanel = new ColourPanel();
		mMessageBox = new MMessageBox();

		xEvent::OnInit(NULL, NULL);
		xEvent::OnInitUI(NULL, NULL);

		_loadPlugin();
	}

	void Editor::Shutdown()
	{
		xEvent::OnShutdown(NULL, NULL);

		safe_delete (mColorPanel);
		safe_delete (mMessageBox);
		safe_delete (mFileDialog);
		safe_delete (mPuginDialog);
	}

	void Editor::Update()
	{
		RenderWindow::Instance()->Update();
		xEvent::OnUpdate();
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

	void Editor::SetOperator(eOperator op)
	{
		if (mOperator == op)
			return ;

		eOperator old = mOperator;
		mOperator = op;

		xEvent::OnOperatorChanged(&old, &op);
	}

	void Editor::_unloadScene(Event * sender)
	{
		SetSelectedShape(NULL);
	}

	int Editor::MessageBox(const char * text, const char * caption, UINT type)
	{
		const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

		return ::MessageBox(dp->hWnd, text, caption, type);
	}



	ColourPanel * Editor::getColorPanel()
	{
		if (mColorPanel == NULL)
			mColorPanel = new ColourPanel();

		return mColorPanel;
	}

	FileDialog * Editor::getFileDialog()
	{
		if (mFileDialog == NULL)
			mFileDialog = new FileDialog();

		return mFileDialog;
	}

	MMessageBox * Editor::getMessageBox()
	{
		if (mMessageBox == NULL)
			mMessageBox = new MMessageBox();

		return mMessageBox;
	}

	PluginDialog * Editor::getPluginDialog()
	{
		if (mPuginDialog == NULL)
			mPuginDialog = new PluginDialog();

		return mPuginDialog;
	}

	void Editor::_loadPlugin()
	{
		FileSystem fs("Infinite_Pugin");

		fs.Load();

		Archive::FileInfoVisitor v = fs.GetFileInfos();

		while (!v.Endof())
		{
			TString128 plugin = v.Cursor()->second.name;

			if (File::GetExternName(plugin) == "dll")
			{
				Dll dll("Infinite_Pugin\\" + plugin);

				dll.Load();
				//DllManager::Instance()->Load();
			}

			++v;
		}
	}
}
