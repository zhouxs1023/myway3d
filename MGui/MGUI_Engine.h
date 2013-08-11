#pragma once

#include "MGUI_Types.h"
#include "MGUI_LookFeel.h"
#include "MGUI_Helper.h"
#include "MGUI_Serializer.h"
#include "MGUI_Shader.h"
#include "MGUI_ShaderDefault.h"
#include "MGUI_Font.h"
#include "MGUI_Timer.h"
#include "MGUI_Layout.h"
#include "MGUI_Clipboard.h"


namespace Myway {

	class MGUI_ENTRY MGUI_Engine
	{
		DECLARE_SINGLETON(MGUI_Engine);

	public:
		MGUI_Engine();
		~MGUI_Engine();

		// Layout
		void AddLayout(MGUI_Layout * _layout);
		void RemoveLayout(const TString128 & name);
		void RemoveLayout(MGUI_Layout * _layout);
		void RemoveAllLayout();
		MGUI_Layout * GetLayout(const TString128 & name);

		// LookFeel
		

		// Widget Loader
		void AddSerializer(MGUI_Serializer * _loader);
		MGUI_Serializer * GetSerializer(const char * _typeName);
		void RemoveAllSerializer();

		void Resize();
		void Update();
		void Render();

		ShaderLib * GetShaderLib() { return mShaderLib; }
		MGUI_ShaderDefault * GetDefaultShader() { return mDefaultShader; }

		const MGUI_Rect & GetRect() { return mRect; }

		// input 
		bool InjectMouseEvent();
		bool InjectKeyEvent(DWORD uMsg, WPARAM wParam,LPARAM lParam);

		MGUI_Widget * GetWidget(int _x, int _y);

		void SetKeyFocusedWidget(MGUI_Widget * _widget);
		MGUI_Widget * GetKeyFocusedWidget();
		MGUI_Widget * GetMouseFocusedWidget();

	protected:
		bool _injectMouseMove(int _x, int _y);
		bool _injectMouseWheel(int _z);
		bool _injectMousePressed(int _x, int _y, MGUI_MouseButton _id);
		bool _injectMouseReleased(int _x, int _y, MGUI_MouseButton _id);
		bool _injectKeyPressed(MGUI_KeyCode _key, MGUI_Char _text = 0);
		bool _injectKeyReleased(MGUI_KeyCode _key);

	protected:
		MGUI_Helper * mHelper;

		Array<MGUI_Layout *> mLayouts;
		Array<MGUI_LookFeel *> mLookFeels;
		Array<MGUI_Serializer *> mSerializers;

		MGUI_Rect mRect;

		ShaderLib * mShaderLib;
		MGUI_ShaderDefault * mDefaultShader;

		MGUI_Font * mFont;

		// input
		int mMouseX, mMouseY, mMouseZ;
		bool mMouseCapture;
		MGUI_Widget* mMouseFocusWidget;
		MGUI_Widget* mKeyFocusWidget;

		MGUI_Timer mTimer;

		MGUI_Clipboard mCilpboard;
	};

}