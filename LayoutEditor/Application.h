/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "StateController.h"
#include "EditorState.h"
#include "TestState.h"
#include "MWApp_Win32.h"
#include "Input/PointerManager.h"

namespace tools
{
	using namespace Myway;

	class Application :
		public App_Win32,
		public MyGUI::Singleton<Application>,
		public StateController
	{
	public:
		Application();
		virtual ~Application();

		virtual void createScene();
		virtual void destroyScene();

		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);
		virtual void prepare();

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams();

		virtual bool Init()
		{
			SetResourceConfig("Res_LayoutEditor.ini");
			App_Win32::Init();

			mUISystem.Init();

			bool hr = MyGUI::ResourceManager::getInstance().load("MyGUI_BlackBlueTheme.xml");
			hr = MyGUI::ResourceManager::getInstance().load("Editor.xml");

			mPointManager.createPointerManager((size_t)mhWnd);

			createScene();

			return true;
		}

		virtual void Shutdown()
		{
			destroyScene();

			mPointManager.destroyPointerManager();

			mUISystem.Shutdown();

			App_Win32::Shutdown();
		}

		virtual void OnMessage(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			if (mhWnd == hWnd && uMsg == WM_SIZE)
			{
				if (wParam != SIZE_MINIMIZED)
				{
					RECT rc;

					GetClientRect(hWnd, &rc);

					Engine::Instance()->Resize(rc.right - rc.left, rc.bottom - rc.top);
				}
			}

			mUISystem.InjectKeyEvent(uMsg, wParam, lParam);
		}

		virtual void Update()
		{
			App_Win32::Update();

			InputSystem::Instance()->Update();

			mUISystem.InjectMouseEvent();

			Sleep(5);

			//Point2f mousePt = IMouse::Instance()->GetPositionUnit();

			//if (mousePt.x >= 0 && mousePt.x <= 1.0f &&
			//	mousePt.y >= 0 && mousePt.y <= 1.0f)
			//{
			//	while (::ShowCursor(FALSE) > 1) ;
			//}
			//else
			//{
			//	while (::ShowCursor(TRUE) < 1) ;
			//}
		}

	protected:
		void _OnKeyPress(Event * sender);
		void command_QuitApp(const MyGUI::UString& _commandName, bool& _result);

	private:
		MGUI_System mUISystem;
		input::PointerManager mPointManager;

		EditorState* mEditorState;
		TestState* mTestState;

		std::string mLocale;
		VectorWString mParams;

		tEventListener<Application> OnKeyPress;
	};

} // namespace tools

#endif // __DEMO_KEEPER_H__
