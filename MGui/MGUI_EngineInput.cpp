#include "MGUI_Engine.h"
#include "MGUI_Input.h"

namespace Myway {

	const unsigned long INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds

	bool MGUI_Engine::InjectMouseEvent()
	{
		bool inject = false;
		if (IMouse::Instance()->MouseMoved())
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMouseMove(pt.x, pt.y))
				inject = true;
		}

		if (IMouse::Instance()->MouseWheel())
		{
			static int pt_z = 0;
			pt_z += IMouse::Instance()->MouseWheel();

			_injectMouseWheel(pt_z);
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMousePressed(pt.x, pt.y, MGUI_MouseButton::Left))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMousePressed(pt.x, pt.y, MGUI_MouseButton::Right))
				inject = true;
		}

		if (IMouse::Instance()->KeyDown(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMousePressed(pt.x, pt.y, MGUI_MouseButton::Middle))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMouseReleased(pt.x, pt.y, MGUI_MouseButton::Left))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMouseReleased(pt.x, pt.y, MGUI_MouseButton::Right))
				inject = true;
		}

		if (IMouse::Instance()->KeyUp(MKC_BUTTON2))
		{
			Point2i pt = IMouse::Instance()->GetPosition();

			if (_injectMouseReleased(pt.x, pt.y, MGUI_MouseButton::Middle))
				inject = true;
		}

		return inject;
	}

	bool MGUI_Engine::InjectKeyEvent(DWORD uMsg, WPARAM wParam,LPARAM lParam)
	{
		bool inject = false;

		if (WM_KEYDOWN == uMsg)
		{
			bool repeat = (lParam & (1 >> 30)) != 0;
			if (!repeat)
			{
				int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
				int text = MGUI_Input::VirtualKeyToText(wParam);

				if (_injectKeyPressed(MGUI_KeyCode::Enum(scan_code), (MGUI_Char)text))
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

			if (_injectKeyPressed(MGUI_KeyCode::None, (MyGUI::Char)text))
				inject = true;
		}
		else if (WM_KEYUP == uMsg)
		{
			int scan_code = MGUI_Input::VirtualKeyToScanCode(wParam);
			MGUI_KeyCode code = MGUI_KeyCode::Enum(scan_code);

			if (code == MGUI_KeyCode::SysRq)
			{
				if (_injectKeyPressed(code, (MGUI_Char)0))
					inject = true;
			}

			if (_injectKeyReleased(code))
				inject = true;
		}

		return inject;
	}


	bool MGUI_Engine::_injectMouseMove(int _x, int _y)
	{
		mMouseX = _x;
		mMouseY = _y;

		if (mMouseCapture)
		{
			if (mMouseFocusWidget != NULL)
				mMouseFocusWidget->_notifyMouseDrag(_x, _y);
			else
				mMouseCapture = false;

			return true;
		}


		MGUI_Widget * widget = GetWidget(_x, _y);

		if (widget == mMouseFocusWidget)
		{
			if (mMouseFocusWidget != NULL)
				mMouseFocusWidget->_notifyMouseMove(_x, _y);

			return mMouseFocusWidget != NULL;
		}

		if (mMouseFocusWidget != NULL)
			mMouseFocusWidget->_notifyMouseLostFocus(widget);

		mMouseFocusWidget = widget;

		if (mMouseFocusWidget != NULL)
			mMouseFocusWidget->_notifyMouseSetFocus(widget);
		
		return mMouseFocusWidget != NULL;
	}

	bool MGUI_Engine::_injectMouseWheel(int _z)
	{
		int dz = _z - mMouseZ;
		mMouseZ = _z;

		if (mMouseFocusWidget != NULL)
		{
			mMouseFocusWidget->_notifyMouseWheel(dz);

			return false;
		}

		return true;
	}

	bool MGUI_Engine::_injectMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
			mMouseCapture = true;

		if (mMouseFocusWidget == NULL)
		{
			mKeyFocusWidget = NULL;
			return false;
		}

		if (!mMouseFocusWidget->GetEnable())
			return true;

		if (mMouseFocusWidget)
		{
			if (mKeyFocusWidget != mMouseFocusWidget)
			{
				if (mKeyFocusWidget && mKeyFocusWidget->GetEnable())
					mKeyFocusWidget->_notifyKeyLostFocus(mMouseFocusWidget);

				MGUI_Widget * _old_widget = mKeyFocusWidget;
				mKeyFocusWidget = mMouseFocusWidget;
				
				if (mKeyFocusWidget->GetEnable())
					mKeyFocusWidget->_notifyKeySetFocus(_old_widget);
			}

			mMouseFocusWidget->_notifyMousePressed(_x, _y, _id);
		}

		return false;
	}

	bool MGUI_Engine::_injectMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		if (mMouseFocusWidget)
		{
			if (!mMouseFocusWidget->GetEnable())
				return true;

			if (_id == MGUI_MouseButton::Left)
				mMouseCapture = false;

			mMouseFocusWidget->_notifyMouseReleased(_x, _y, _id);

			if (mMouseFocusWidget != NULL)
			{
				if (MGUI_MouseButton::Left == _id)
				{
					if (mTimer.GetMilliseconds() < INPUT_TIME_DOUBLE_CLICK)
					{
						mMouseFocusWidget->_notifyMouseClick();

						if (mMouseFocusWidget != NULL)
							mMouseFocusWidget->_notifyMouseDoubleClick();
					}
					else
					{
						MGUI_Widget* item = GetWidget(_x, _y);

						if ( item == mMouseFocusWidget)
						{
							mMouseFocusWidget->_notifyMouseClick();
						}

						mTimer.Reset();
					}
				}
			}

			_injectMouseMove(_x, _y);

			return true;
		}

		return false;
	}

	bool MGUI_Engine::_injectKeyPressed(MGUI_KeyCode _key, MGUI_Char _text)
	{
		if (mKeyFocusWidget != NULL)
			mKeyFocusWidget->_notifyKeyPressed(_key, _text);

		return mKeyFocusWidget != NULL;
	}

	bool MGUI_Engine::_injectKeyReleased(MGUI_KeyCode _key)
	{
		if (mKeyFocusWidget != NULL)
			mKeyFocusWidget->_notifyKeyReleased(_key);

		return mKeyFocusWidget != NULL;
	}

	MGUI_Widget * MGUI_Engine::GetWidget(int _x, int _y)
	{
		for (int i = 0; i < mLayouts.Size(); ++i)
		{
			MGUI_Widget * item = mLayouts[i]->Pick(_x, _y);

			if (item)
				return item;
		}

		return NULL;
	}

	void MGUI_Engine::SetKeyFocusedWidget(MGUI_Widget * _widget)
	{
		if (mKeyFocusWidget != _widget)
		{
			MGUI_Widget * oldWidget = mKeyFocusWidget;

			mKeyFocusWidget = _widget;

			if (oldWidget != NULL)
				oldWidget->_notifyKeyLostFocus(_widget);

			if (_widget != NULL)
				_widget->_notifyKeySetFocus(oldWidget);
		}
	}

	MGUI_Widget * MGUI_Engine::GetKeyFocusedWidget()
	{
		return mKeyFocusWidget;
	}

	MGUI_Widget * MGUI_Engine::GetMouseFocusedWidget()
	{
		return mMouseFocusWidget;
	}

}