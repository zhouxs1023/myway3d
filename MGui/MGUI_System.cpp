#include "MGUI_System.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"

namespace Myway {

	IMP_SLN(MGUI_System);

	MGUI_System::MGUI_System()
		: OnResize(RenderEvent::OnResize, this, &MGUI_System::_OnResize)
		, OnRender(RenderEvent::OnRenderGUI1, this, &MGUI_System::_OnRender)
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









	bool MGUI_System::InjectMouseEvent()
	{
		bool inject = false;
		if (IMouse::Instance()->MouseMoved() || IMouse::Instance()->MouseWheel())
		{
			static int pt_z = 0;
			Point2i pt = IMouse::Instance()->GetPosition();
			pt_z += IMouse::Instance()->MouseWheel();

			if (injectMouseMove(pt.x, pt.y, pt_z))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			
			if (injectMousePress(pt.x, pt.y, MyGUI::MouseButton::Left))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			
			if (injectMousePress(pt.x, pt.y, MyGUI::MouseButton::Right))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			
			if (injectMousePress(pt.x, pt.y, MyGUI::MouseButton::Middle))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			
			if (injectMouseRelease(pt.x, pt.y, MyGUI::MouseButton::Left))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();
			
			if (injectMouseRelease(pt.x, pt.y, MyGUI::MouseButton::Right))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (injectMouseRelease(pt.x, pt.y, MyGUI::MouseButton::Middle))
				inject = true;
		}

		return inject;
	}

	bool MGUI_System::InjectKeyEvent(DWORD uMsg, WPARAM wParam,LPARAM lParam)
	{
		bool inject = false;

		if (WM_KEYDOWN == uMsg)
		{
			bool repeat = (lParam & (1 >> 30)) != 0;
			if (!repeat)
			{
				int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
				int text = MGUI_Input::VirtualKeyToText(wParam);
				
				if (injectKeyPress(MyGUI::KeyCode::Enum(scan_code), (MyGUI::Char)text))
					inject = true;
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
			
			if (injectKeyPress(MyGUI::KeyCode::None, (MyGUI::Char)text))
				inject = true;
		}
		else if (WM_KEYUP == uMsg)
		{
			int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
			MyGUI::KeyCode code = MyGUI::KeyCode::Enum(scan_code);

			if (code == MyGUI::KeyCode::SysRq)
			{
				if (injectKeyPress(code, (MyGUI::Char)0))
					inject = true;
			}

			if (injectKeyRelease(code))
				inject = true;
		}

		return inject;
	}



	bool MGUI_System::injectMouseMove(int _absx, int _absy, int _absz)
	{
		bool _inject = true;

		OnMouseMove(&_inject, &_absx, &_absy, &_absz);

		if (!_inject)
			return false;

		return MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
	}

	bool MGUI_System::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		bool _inject = true;

		OnMousePress(&_inject, &_absx, &_absy, &_id);

		if (!_inject)
			return false;

		return MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}

	bool MGUI_System::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		bool _inject = true;

		OnMouseRelease(&_inject, &_absx, &_absy, &_id);

		if (!_inject)
			return false;

		return MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
	}

	bool MGUI_System::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		bool _inject = true;

		OnKeyPress(&_inject, &_key, &_text);

		if (!_inject)
			return false;

		return MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
	}

	bool MGUI_System::injectKeyRelease(MyGUI::KeyCode _key)
	{
		bool _inject = true;

		OnKeyRelease(&_inject, &_key);

		if (!_inject)
			return false;

		return MyGUI::InputManager::getInstance().injectKeyRelease(_key);
	}
}