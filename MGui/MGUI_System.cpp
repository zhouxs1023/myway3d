#include "MGUI_System.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"

namespace Myway {

	IMP_SLN(MGUI_System);

	MGUI_System::MGUI_System()
		: OnResize(&RenderEvent::OnResize, this, &MGUI_System::_OnResize)
		, OnRender(&RenderEvent::OnRenderGUI1, this, &MGUI_System::_OnRender)
	{
		INIT_SLN;

		mLog = NULL;
		mDataManager = NULL;
		mRenderManager = NULL;
		
		mGui = NULL;
	}

	MGUI_System::~MGUI_System()
	{
		SHUT_SLN;

		d_assert (mLog == NULL);
	}

	void MGUI_System::Init()
	{
		mLog = new MyGUI::LogManager;
		mDataManager = new MGUI_DataManager;
		mRenderManager = new MGUI_RenderSystem;

		mLog->createDefaultSource("MyGUI.log");

		mGui = new MyGUI::Gui;
		mGui->initialise();
	}

	void MGUI_System::Shutdown()
	{
		d_assert (mLog);

		mGui->shutdown();
		delete mGui;

		delete mRenderManager;
		delete mDataManager;
		delete mLog;

		mGui = NULL;
		mRenderManager = NULL;
		mDataManager = NULL;
		mLog = NULL;
	}

	void MGUI_System::Render()
	{
		mRenderManager->drawOneFrame();
	}

	void MGUI_System::_OnResize(Event * sender)
	{
		int w = Engine::Instance()->GetDeviceProperty()->Width;
		int h = Engine::Instance()->GetDeviceProperty()->Height;

		mRenderManager->setViewSize(w, h);
	}

	void MGUI_System::_OnRender(Event * sender)
	{
		mRenderManager->drawOneFrame();
	}









	void MGUI_System::InjectMouseEvent()
	{
		if (IMouse::Instance()->MouseMoved() || IMouse::Instance()->MouseWheel())
		{
			static int pt_z = 0;
			Point2i pt = IMouse::Instance()->GetPosition();
			pt_z += IMouse::Instance()->MouseWheel();

			injectMouseMove(pt.x, pt.y, pt_z);
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			injectMousePress(pt.x, pt.y, MyGUI::MouseButton::Left);
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			injectMousePress(pt.x, pt.y, MyGUI::MouseButton::Right);
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			injectMousePress(pt.x, pt.y, MyGUI::MouseButton::Middle);
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			injectMouseRelease(pt.x, pt.y, MyGUI::MouseButton::Left);
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			injectMouseRelease(pt.x, pt.y, MyGUI::MouseButton::Right);
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			injectMouseRelease(pt.x, pt.y, MyGUI::MouseButton::Middle);
		}
	}

	void MGUI_System::InjectKeyEvent(DWORD uMsg, WPARAM wParam,LPARAM lParam)
	{
		if (WM_KEYDOWN == uMsg)
		{
			bool repeat = (lParam & (1 >> 30)) != 0;
			if (!repeat)
			{
				int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
				int text = MGUI_Input::VirtualKeyToText(wParam);
				injectKeyPress(MyGUI::KeyCode::Enum(scan_code), (MyGUI::Char)text);
			}
		}
		else if (WM_IME_CHAR == uMsg)
		{
			int text = 0;
#ifdef _UNICODE
			text = wParam;
#else
			char mbstr[3];
			BYTE hiByte = wParam >> 8;
			BYTE loByte = wParam & 0x000000FF;
			if (hiByte == 0)
			{
				mbstr[0] = loByte;
				mbstr[1] = '\0';
			}
			else
			{
				mbstr[0] = hiByte;
				mbstr[1] = loByte;
				mbstr[2] = '\0';
			}

			wchar_t wstr[2];
			/*int num = */MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, mbstr, -1, wstr, _countof(wstr));
			text = wstr[0];
#endif // _UNICODE
			injectKeyPress(MyGUI::KeyCode::None, (MyGUI::Char)text);
		}
		else if (WM_KEYUP == uMsg)
		{
			int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
			MyGUI::KeyCode code = MyGUI::KeyCode::Enum(scan_code);

			if (code == MyGUI::KeyCode::SysRq)
				injectKeyPress(code, (MyGUI::Char)0);

			injectKeyRelease(code);
		}
	}



	void MGUI_System::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGui)
			return;

		MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
	}

	void MGUI_System::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGui)
			return;

		MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}

	void MGUI_System::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGui)
			return;

		MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
	}

	void MGUI_System::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (!mGui)
			return;

		MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
	}

	void MGUI_System::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGui)
			return;

		MyGUI::InputManager::getInstance().injectKeyRelease(_key);
	}
}