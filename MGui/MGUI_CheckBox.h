#pragma once

#include "MGUI_Widget.h"

namespace Myway {


	class MGUI_ENTRY MGUI_CheckBox : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		typedef tEvent1<bool> EventChecked;

		EventChecked eventChecked;

	public:
		MGUI_CheckBox(MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_CheckBox();

		void SetChecked(bool checked) { mChecked = true; }
		bool GetChecked() { return mChecked; }

		void SetCaption(const TString128 & caption) { mCaption = caption; }
		const TString128 & GetCaption() const { return mCaption; }

		void SetCharHeight(int _height) { mCharHeight = _height; }
		int GetCharDY() const { return mCharHeight; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

	protected:
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id);
		virtual void OnMouseReleased(int _x, int _y, MGUI_MouseButton _id);

	protected:
		bool mChecked;

		TString128 mCaption;
		int mCharHeight;
	};


}