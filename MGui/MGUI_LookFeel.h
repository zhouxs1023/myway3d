#pragma once

#include "MGUI_Types.h"
#include "MGUI_Shader.h"

namespace Myway {

	class MGUI_Widget;

	class MGUI_ENTRY MGUI_LookFeel
	{
		DECLARE_ALLOC();

	public:
		MGUI_LookFeel(const TString128 & _name);
		virtual ~MGUI_LookFeel();

		const TString128 & GetName() const { return mName; }

		void SetSkin(const char * _source);
		void SetSkin(TexturePtr _skin) { mSkin = _skin; }
		void SetShader(MGUI_Shader * _shader) { mShader = _shader; }

		Texture * GetSkin() const { return mSkin.c_ptr(); }
		MGUI_Shader * GetShader() const { return mShader; }

		void SetUVRect(int _state, const MGUI_Rect & _rect) { mUVRect[_state] = _rect; }
		const MGUI_Rect & GetUVRect(int _state) const { return mUVRect[_state]; }

		void SetTextColor(int _state, const Color4 & _color) { mTextColor[_state] = _color; }
		const Color4 & GetTextColor(int _state) const { return mTextColor[_state]; }

		void SetClientRect(const MGUI_Rect & _rect) { mClientRect = _rect; }
		const MGUI_Rect & GetClientRect() const { return mClientRect; }

		void Init(MGUI_Widget * _widget) const;
		void Affect(MGUI_Widget * _widget) const;

		MGUI_Rect GetUVClientRect(int _state) const;

		void AddChild(MGUI_LookFeel * _lookfeel) { mChildren.PushBack(_lookfeel); }
		int GetChildCount() { return mChildren.Size(); }
		MGUI_LookFeel * GetChild(int _index) { return mChildren[_index]; }
		MGUI_LookFeel * GetChild(const TString128 & _name);
		const MGUI_LookFeel * GetChild(const TString128 & _name) const;

		MGUI_LookFeel * Clone(const TString128 & _name);

	protected:
		TString128 mName;
		TexturePtr mSkin;
		MGUI_Shader * mShader;

		tFixedArray<MGUI_Rect, MGUI_WidgetState::Max> mUVRect;
		tFixedArray<Color4, MGUI_WidgetState::Max> mTextColor;
		MGUI_Rect mClientRect;

		Array<MGUI_LookFeel *> mChildren;
	};


	class MGUI_ENTRY MGUI_LookFeelManager
	{
		DECLARE_SINGLETON (MGUI_LookFeelManager);

	public:
		MGUI_LookFeelManager();
		~MGUI_LookFeelManager();

		void Load(const char * source);
		void Unload();

		bool Save();

		void AddLookFeel(int _type, MGUI_LookFeel * _lookfeel);
		void RemoveLookFeel(int _type, MGUI_LookFeel * _lookfeel);

		int GetLookFeelCount(int _type);
		MGUI_LookFeel * GetLookFeel(int _type, int _index);
		MGUI_LookFeel * GetLookFeel(int _type, const TString128 & _name);

	protected:
		TString128 mFilename;

		tFixedArray<Array<MGUI_LookFeel*>, MGUI_WidgetType::Max> mLookFeelGroup;
	};
}