#pragma once

#include "MGUI_Types.h"
#include "MGUI_Widget.h"
#include "MGUI_Timer.h"

namespace Myway {

	class MGUI_ENTRY MGUI_InputManager
	{
		DECLARE_SINGLETON(MGUI_InputManager);

	public:
		tEvent2<int, int> eventMouseMove;
		tEvent1<int> eventMouseWheel;
		tEvent3<int, int, MGUI_MouseButton> eventMousePressed;
		tEvent3<int, int, MGUI_MouseButton> eventMouseReleased;
		tEvent2<MGUI_KeyCode, MGUI_Char> eventKeyPressed;
		tEvent1<MGUI_KeyCode> eventKeyReleased;
		tEvent1<int> eventTimer;

	public:
		MGUI_InputManager();
		~MGUI_InputManager();

		void SetKeyFocusedWidget(MGUI_Widget * _widget);
		MGUI_Widget * GetKeyFocusedWidget();

		void SetMouseFocusedWidget(MGUI_Widget * _widget);
		MGUI_Widget * GetMouseFocusedWidget();

		bool IsMouseCaptured() { return mMouseCapture; }

		bool _injectMouseMove(int _x, int _y);
		bool _injectMouseWheel(int _z);
		bool _injectMousePressed(int _x, int _y, MGUI_MouseButton _id);
		bool _injectMouseReleased(int _x, int _y, MGUI_MouseButton _id);
		bool _injectKeyPressed(MGUI_KeyCode _key, MGUI_Char _text = 0);
		bool _injectKeyReleased(MGUI_KeyCode _key);

		void Update();

		int StartTimer(int _dtime);
		void EndTimer(int _id);

	protected:
		int mMouseX, mMouseY, mMouseZ;
		bool mMouseCapture;
		MGUI_Widget* mMouseFocusWidget;
		MGUI_Widget* mKeyFocusWidget;

		MGUI_Timer mTimer;

		List<MGUI_Counter> mCounters;
	};

}