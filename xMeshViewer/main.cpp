#include "Myway.h"
#include "MWApp_Win32.h"
#include <stdio.h>

using namespace Myway;

const char * GetCommand(const char * cmd)
{
	static char tmp[256];

	while (cmd[0] != '\"' && cmd[0] != 0)
		++cmd;

	if (cmd[0] != '\"')
		++cmd;

	int length = strlen(cmd);

	Strcpy (tmp, 256, cmd);

	while (length > 0 && tmp[length - 1] != '\"')
		--length;

	tmp[length - 1] = 0;

	return tmp;
}

class MyApp : public App_Win32
{
	char cmpLine[2048];

public:
	MyApp()
	{
		mEntity = NULL;
		mSceneNode = NULL;
	}

	~MyApp() {}

	virtual bool Init()
	{
		App_Win32::Init();

		DragAcceptFiles(mhWnd, TRUE);

		const char * cmdLine = GetCommandLine();

		if (Strcmp(cmdLine, "") == 0)
			return true;

		cmdLine = GetCommand(cmdLine);

		LookMesh(cmdLine);

		return true;
	}

	void LookMesh(const char * filename)
	{
		TString128 meshFile = filename;
		TString128 base, path;

		meshFile.SplitFileNameR(base, path); 

		TString128 externName;

		externName = File::GetExternName(base);

		if (externName != "mesh")
			return ;

		if (mEntity)
		{
			World::Instance()->DestroyEntity(mEntity);
			mEntity = NULL;
		}

		if (mSceneNode)
		{
			World::Instance()->DestroySceneNode(mSceneNode);
		}

		mEntity = World::Instance()->CreateEntity("xxx", base);

		mSceneNode = World::Instance()->CreateSceneNode();

		mSceneNode->Attach(mEntity);

		Aabb bound = mEntity->GetWorldAabb();

		float size = 0;

		size = Math::Maximum(size, bound.GetWidth());
		size = Math::Maximum(size, bound.GetHeight());
		size = Math::Maximum(size, bound.GetDepth());

		Camera * pCamera = World::Instance()->MainCamera();
		pCamera->SetPosition(bound.GetCenter() + Vec3(0, 0, -1.5f) * size);
		pCamera->LookAt(bound.GetCenter());

	}

	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
	{
		switch (iMsg)
		{
		case WM_DROPFILES:
			{
				TCHAR meshFile[MAX_PATH] = { 0 };
				HDROP hDrop = (HDROP)wParam;
				int len = DragQueryFile(hDrop, 0, meshFile, MAX_PATH);

				if (len > 0)
					LookMesh(meshFile);
			}
			break;
		}
	}

	virtual void Update()
	{
		App_Win32::Update();

		InputSystem::Instance()->Update();

		SceneNode * cam = World::Instance()->MainCameraNode();

		if (IKeyboard::Instance()->KeyPressed(KC_W))
		{
			cam->Move(5.0f);
		}
		else if (IKeyboard::Instance()->KeyPressed(KC_S))
		{
			cam->Move(-5.0f);
		}
		else if (IKeyboard::Instance()->KeyPressed(KC_A))
		{
			cam->Right(-5.0f);
		}
		else if (IKeyboard::Instance()->KeyPressed(KC_D))
		{
			cam->Right(5.0f);
		}

		if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPositionDiff();

			if (abs(pt.y) >abs(pt.x))
			{
				cam->Pitch(pt.y * 0.005f, TS_LOCAL);
			}
			else
			{
				cam->Yaw(pt.x * 0.005f, TS_PARENT);
			}
		}
	}

protected:
	Entity * mEntity;
	SceneNode * mSceneNode;
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char sFileName[1024];
	GetModuleFileName(GetModuleHandle(NULL), sFileName, 1024);

	TString128 fileDir = File::GetFileDir(sFileName);

	SetCurrentDirectory(fileDir.c_str());

	MyApp app;

	app.Run(hInstance);

	return 0;
}