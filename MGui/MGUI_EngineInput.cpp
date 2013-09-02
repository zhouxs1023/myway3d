#include "MGUI_Engine.h"
#include "MGUI_Input.h"

namespace Myway {

	bool MGUI_Engine::InjectMouseEvent()
	{
		bool inject = false;
		if (IMouse::Instance()->MouseMoved())
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMouseMove(pt.x, pt.y))
				inject = true;
		}

		if (IMouse::Instance()->MouseWheel())
		{
			static int pt_z = 0;
			pt_z += IMouse::Instance()->MouseWheel();

			if (mInputManager._injectMouseWheel(pt_z))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMousePressed(pt.x, pt.y, MGUI_MouseButton::Left))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMousePressed(pt.x, pt.y, MGUI_MouseButton::Right))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMousePressed(pt.x, pt.y, MGUI_MouseButton::Middle))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMouseReleased(pt.x, pt.y, MGUI_MouseButton::Left))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMouseReleased(pt.x, pt.y, MGUI_MouseButton::Right))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (mInputManager._injectMouseReleased(pt.x, pt.y, MGUI_MouseButton::Middle))
				inject = true;
		}

		return inject;
	}

	bool MGUI_Engine::InjectMessage(DWORD uMsg, WPARAM wParam,LPARAM lParam)
	{
		bool inject = false;

		if (WM_KEYDOWN == uMsg)
		{
			bool repeat = (lParam & (1 >> 30)) != 0;
			if (!repeat)
			{
				int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
				int text = MGUI_Input::VirtualKeyToText(wParam);

				if (mInputManager._injectKeyPressed(MGUI_KeyCode::Enum(scan_code), (MGUI_Char)text))
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

			if (mInputManager._injectKeyPressed(MGUI_KeyCode::None, (MyGUI::Char)text))
				inject = true;
		}
		else if (WM_KEYUP == uMsg)
		{
			int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
			MGUI_KeyCode code = MGUI_KeyCode::Enum(scan_code);

			if (code == MGUI_KeyCode::SysRq)
			{
				if (mInputManager._injectKeyPressed(code, (MGUI_Char)0))
					inject = true;
			}

			if (mInputManager._injectKeyReleased(code))
				inject = true;
		}

		return inject;
	}


	

}