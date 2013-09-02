#include "MGUI_InputManager.h"

#include "MGUI_Engine.h"

namespace Myway {

	const unsigned long INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds

	MGUI_InputManager * MGUI_InputManager::msInstance = NULL;

	MGUI_InputManager::MGUI_InputManager()
	{
		msInstance = this;

		mMouseX = mMouseY = mMouseZ = 0;
		mMouseCapture = false;
		mMouseFocusWidget = NULL;
		mKeyFocusWidget = NULL;
	}

	MGUI_InputManager::~MGUI_InputManager()
	{
		msInstance = NULL;
	}

	bool MGUI_InputManager::_injectMouseMove(int _x, int _y)
	{
		eventMouseMove(_x, _y);

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

		MGUI_Widget * widget = MGUI_Engine::Instance()->GetWidget(_x, _y);

		if (widget == mMouseFocusWidget)
		{
			if (mMouseFocusWidget != NULL)
				mMouseFocusWidget->_notifyMouseMove(_x, _y);

			return mMouseFocusWidget != NULL;
		}

		SetMouseFocusedWidget(widget);

		return mMouseFocusWidget != NULL;
	}

	bool MGUI_InputManager::_injectMouseWheel(int _z)
	{
		eventMouseWheel(_z);

		int dz = _z - mMouseZ;
		mMouseZ = _z;

		if (mMouseFocusWidget != NULL)
		{
			mMouseFocusWidget->_notifyMouseWheel(dz);

			return false;
		}

		return true;
	}

	bool MGUI_InputManager::_injectMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		eventMousePressed(_x, _y, _id);

		if (_id == MGUI_MouseButton::Left)
			mMouseCapture = true;

		if (mMouseFocusWidget == NULL)
		{
			SetKeyFocusedWidget(NULL);
			return false;
		}

		if (!mMouseFocusWidget->GetEnable())
			return true;

		if (mMouseFocusWidget)
		{
			SetKeyFocusedWidget(mMouseFocusWidget);
			mMouseFocusWidget->_notifyMousePressed(_x, _y, _id);
		}

		return true;
	}

	bool MGUI_InputManager::_injectMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		eventMouseReleased(_x, _y, _id);

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
						MGUI_Widget* item = MGUI_Engine::Instance()->GetWidget(_x, _y);

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

	bool MGUI_InputManager::_injectKeyPressed(MGUI_KeyCode _key, MGUI_Char _text)
	{
		eventKeyPressed(_key, _text);

		if (mKeyFocusWidget != NULL)
			mKeyFocusWidget->_notifyKeyPressed(_key, _text);

		return mKeyFocusWidget != NULL;
	}

	bool MGUI_InputManager::_injectKeyReleased(MGUI_KeyCode _key)
	{
		eventKeyReleased(_key);

		if (mKeyFocusWidget != NULL)
			mKeyFocusWidget->_notifyKeyReleased(_key);

		return mKeyFocusWidget != NULL;
	}

	void MGUI_InputManager::SetKeyFocusedWidget(MGUI_Widget * _widget)
	{
		if (_widget && !_widget->CanKeyFoucsed() && _widget->GetEnable())
			_widget = NULL;

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

	MGUI_Widget * MGUI_InputManager::GetKeyFocusedWidget()
	{
		return mKeyFocusWidget;
	}

	void MGUI_InputManager::SetMouseFocusedWidget(MGUI_Widget * _widget)
	{
		if (mMouseFocusWidget != _widget)
		{
			MGUI_Widget * oldWidget = mMouseFocusWidget ;

			mMouseFocusWidget  = _widget;

			if (oldWidget != NULL)
				oldWidget->_notifyMouseLostFocus(_widget);

			if (_widget != NULL)
				_widget->_notifyMouseSetFocus(oldWidget);
		}
	}

	MGUI_Widget * MGUI_InputManager::GetMouseFocusedWidget()
	{
		return mMouseFocusWidget;
	}

	void MGUI_InputManager::Update()
	{
		List<MGUI_Counter>::Iterator whr = mCounters.Begin();
		List<MGUI_Counter>::Iterator end = mCounters.End();

		unsigned long curTime = mTimer.GetCurrentMilliseconds();

		while (whr != end)
		{
			unsigned long dtime = curTime - whr->LastTime;
			if (dtime >= whr->IntervalTime)
			{
				whr->LastTime = curTime;
				eventTimer(whr->Id);
			}

			++whr;
		}
	}

	int MGUI_InputManager::StartTimer(int _dtime)
	{
		d_assert(_dtime > 0);

		static int u_id = 1;

		MGUI_Counter ct;

		ct.Id = u_id++;
		ct.IntervalTime = (unsigned long)_dtime;
		ct.LastTime = mTimer.GetCurrentMilliseconds();

		mCounters.PushBack(ct);

		return ct.Id;
	}

	void MGUI_InputManager::EndTimer(int _id)
	{
		List<MGUI_Counter>::Iterator whr = mCounters.Begin();
		List<MGUI_Counter>::Iterator end = mCounters.End();

		while (whr != end)
		{
			if (whr->Id == _id)
			{
				mCounters.Erase(whr);
				return ;
			}

			++whr;
		}

		d_assert (0);
	}

}