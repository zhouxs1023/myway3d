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
    MyApp() {}
    ~MyApp() {}

     virtual bool Init()
     {
         App_Win32::Init();

         /*const char * cmdLine = GetCommandLine();

		 if (Strcmp(cmdLine, "") == 0)
			 return true;

		 cmdLine = GetCommand(cmdLine);

		 TString128 meshFile = cmdLine;
		 TString128 base, path;

		 meshFile.SplitFileNameR(base, path); 

		 TString128 externName;

		 externName = File::GetExternName(base);

		 if (externName != "mesh")
			 return true;*/

		 const char * base = "temp.mesh";

         Entity * pEntity = World::Instance()->CreateEntity("xx", base, "core");
         SceneNode * pSceneNode = World::Instance()->CreateSceneNode();

         pSceneNode->Attach(pEntity);

         Aabb bound = pEntity->GetWorldAabb();

         float size = 0;

         size = Math::Maximum(size, bound.GetWidth());
         size = Math::Maximum(size, bound.GetHeight());
         size = Math::Maximum(size, bound.GetDepth());

         Camera * pCamera = World::Instance()->MainCamera();
         pCamera->SetPosition(bound.GetCenter() + Vec3(0, 0, -1.5f) * size);
         pCamera->LookAt(bound.GetCenter());

         return true;
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