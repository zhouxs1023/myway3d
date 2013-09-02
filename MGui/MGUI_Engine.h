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
#include "MGUI_SkinManager.h"
#include "MGUI_InputManager.h"


namespace Myway {

	class MGUI_ENTRY MGUI_Engine
	{
		DECLARE_SINGLETON(MGUI_Engine);

	public:
		MGUI_Engine();
		~MGUI_Engine();

		// Layout
		void AddLayout(MGUI_Layout * _layout);
		void RemoveLayout(MGUI_Layout * _layout);
		void RemoveAllLayout(bool _delete = true);
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
		bool InjectMessage(DWORD uMsg, WPARAM wParam,LPARAM lParam);

		MGUI_Widget * GetWidget(int _x, int _y);

	protected:
		MGUI_Helper * mHelper;

		Array<MGUI_Layout *> mLayouts;
		Array<MGUI_LookFeel *> mLookFeels;
		Array<MGUI_Serializer *> mSerializers;

		MGUI_Rect mRect;

		ShaderLib * mShaderLib;
		MGUI_ShaderDefault * mDefaultShader;

		MGUI_Font * mFont;

		MGUI_LookFeelManager mLookFeelManager;

		MGUI_Clipboard mCilpboard;

		MGUI_SkinManager mSkinManager;

		MGUI_InputManager mInputManager;
	};

}