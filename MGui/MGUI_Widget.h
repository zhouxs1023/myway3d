//////////////////////////////////////////////////////////////////////////
//
// MyGui Integration.
//   MyGui engine: http://mygui.info
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "MGUI_Entry.h"
#include "MGUI_Types.h"
#include "MGUI_LookFeel.h"
#include "MGUI_Event.h"

namespace Myway {

	class MGUI_Layout;

	typedef tEvent1<MGUI_Widget*>					EventMouseLostFocus;
	typedef tEvent1<MGUI_Widget*>					EventMouseSetFocus;
	typedef tEvent2<int, int>						EventMouseDrag;
	typedef tEvent2<int, int>						EventMouseMove;
	typedef tEvent1<int>							EventMouseWheel;
	typedef tEvent3<int, int, MGUI_MouseButton>		EventMousePressed;
	typedef tEvent3<int, int, MGUI_MouseButton>		EventMouseReleased;
	typedef tEvent0									EventMouseClick;
	typedef tEvent0									EventMouseDoubleClick;
	typedef tEvent1<MGUI_Widget*>					EventKeyLostFocus;
	typedef tEvent1<MGUI_Widget*>					EventKeySetFocus;
	typedef tEvent2<MGUI_KeyCode, MGUI_Char>		EventKeyPressed;
	typedef tEvent1<MGUI_KeyCode>					EventKeyReleased;
		 
	class MGUI_ENTRY MGUI_Widget
	{
		DeclareRootRTTI(MGUI_Widget);
		DECLARE_ALLOC();

	public:
		EventMouseLostFocus eventMouseLostFocus;
		EventMouseSetFocus eventMouseSetFocus;
		EventMouseDrag eventMouseDrag;
		EventMouseMove eventMouseMove;
		EventMouseWheel eventMouseWheel;
		EventMousePressed eventMousePressed;
		EventMouseReleased eventMouseReleased;
		EventMouseClick eventMouseClick;
		EventMouseDoubleClick eventMouseDoubleClick;
		EventKeyLostFocus eventKeyLostFocus;
		EventKeySetFocus eventKeySetFocus;
		EventKeyPressed eventKeyPressed;
		EventKeyReleased eventKeyReleased;

	public:
		MGUI_Widget(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_Widget();

		void SetName(const TString128 & _name);
		const TString128 & GetName() const;

		MGUI_Widget * GetParent();
		const MGUI_LookFeel * GetLookFeel();

		void SetRect(const MGUI_Rect & _rect);
		const MGUI_Rect & GetRect();
		const MGUI_Rect & GetAbsRect();
		void SetRect(int x0, int y0, int x1, int y1) { SetRect(MGUI_Rect(x0, y0, x1, y1)); }
		void Move(int x, int y);

		void SetClientRect(const MGUI_Rect & _rect);
		const MGUI_Rect & GetClientRect();
		const MGUI_Rect & GetAbsClientRect();
		const MGUI_Rect & GetClipRect();

		void SetSize(const MGUI_Size & _size);
		const MGUI_Size & GetSize();

		void SetAlign(MGUI_Align _align);
		MGUI_Align GetAlign();

		void SetColor(const MGUI_Color & _color);
		const MGUI_Color & GetColor();

		void SetOrder(int _order);
		int GetOrder();
		int GetAbsOrder();

		virtual void SetEnable(bool enable) { mEnable = enable; }
		bool GetEnable() { return mEnable; }

		void SetVisible(bool visible) { mVisible = visible; }
		bool GetVisible() { return mVisible; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

		virtual void Update();
		virtual void OnUpdate() {}

		virtual MGUI_Widget * Pick(int _x, int _y);

		int GetState() const { return mState; }
		MGUI_Widget * GetChild(const TString128 & _name);



		// internal
		void _notifyMouseLostFocus(MGUI_Widget* _new);
		void _notifyMouseSetFocus(MGUI_Widget* _old);
		void _notifyMouseDrag(int _x, int _y);
		void _notifyMouseMove(int _x, int _y);
		void _notifyMouseWheel(int _dz);
		void _notifyMousePressed(int _x, int _y, MGUI_MouseButton _id);
		void _notifyMouseReleased(int _x, int _y, MGUI_MouseButton _id);
		void _notifyMouseClick();
		void _notifyMouseDoubleClick();
		void _notifyKeyLostFocus(MGUI_Widget* _new);
		void _notifyKeySetFocus(MGUI_Widget* _old);
		void _notifyKeyPressed(MGUI_KeyCode _key, MGUI_Char _char);
		void _notifyKeyReleased(MGUI_KeyCode _key);

	protected:
		virtual void OnMouseLostFocus(MGUI_Widget* _new) {}
		virtual void OnMouseSetFocus(MGUI_Widget* _old) {}
		virtual void OnMouseDrag(int _x, int _y) {}
		virtual void OnMouseMove(int _x, int _y) {}
		virtual void OnMouseWheel(int _dz) {}
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id) {}
		virtual void OnMouseReleased(int _x, int _y, MGUI_MouseButton _id) {}
		virtual void OnMouseClick() {}
		virtual void OnMouseDoubleClick() {}
		virtual void OnKeyLostFocus(MGUI_Widget* _new) {}
		virtual void OnKeySetFocus(MGUI_Widget* _old) {}
		virtual void OnKeyPressed(MGUI_KeyCode _key, MGUI_Char _char) {}
		virtual void OnKeyReleased(MGUI_KeyCode _key) {}

	protected:
		void _notifyAttach(MGUI_Widget * _child);
		void _notifyDetach(MGUI_Widget * _child);

	protected:
		TString128 mName;

		const MGUI_LookFeel * mLookFeel;
		MGUI_Widget * mParent;
		MGUI_Widget * mRoot;
		Array<MGUI_Widget *> mChildren;

		MGUI_Rect mRect;
		MGUI_Rect mAbsRect;
		MGUI_Rect mClientRect;
		MGUI_Rect mAbsClientRect;
		MGUI_Rect mClipRect;

		MGUI_Align mAlign;
		Color4 mColor;

		int mOrder;
		int mAbsOrder;

		bool mEnable;
		bool mVisible;

		int mState;
	};


	// inline
	inline void MGUI_Widget::_notifyMouseLostFocus(MGUI_Widget* _new)
	{
		OnMouseLostFocus(_new);
		eventMouseSetFocus(_new);
	}

	inline void MGUI_Widget::_notifyMouseSetFocus(MGUI_Widget* _old)
	{
		OnMouseSetFocus(_old);
		eventMouseSetFocus(_old);
	}

	inline void MGUI_Widget::_notifyMouseDrag(int _x, int _y)
	{
		OnMouseDrag(_x, _y);
		eventMouseDrag(_x, _y);
	}

	inline void MGUI_Widget::_notifyMouseMove(int _x, int _y)
	{
		OnMouseMove(_x, _y);
		eventMouseMove(_x, _y);
	}

	inline void MGUI_Widget::_notifyMouseWheel(int _z)
	{
		OnMouseWheel(_z);
		eventMouseWheel(_z);
	}

	inline void MGUI_Widget::_notifyMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		OnMousePressed(_x, _y, _id);
		eventMousePressed(_x, _y, _id);
	}

	inline void MGUI_Widget::_notifyMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		OnMouseReleased(_x, _y, _id);
		eventMouseReleased(_x, _y, _id);
	}

	inline void MGUI_Widget::_notifyMouseClick()
	{
		OnMouseClick();
		eventMouseClick();
	}

	inline void MGUI_Widget::_notifyMouseDoubleClick()
	{
		OnMouseDoubleClick();
		eventMouseDoubleClick();
	}

	inline void MGUI_Widget::_notifyKeyLostFocus(MGUI_Widget* _new)
	{
		OnKeyLostFocus(_new);
		eventKeyLostFocus(_new);
	}

	inline void MGUI_Widget::_notifyKeySetFocus(MGUI_Widget* _old)
	{
		OnKeySetFocus(_old);
		eventKeySetFocus(_old);
	}

	inline void MGUI_Widget::_notifyKeyPressed(MGUI_KeyCode _key, MGUI_Char _char)
	{
		OnKeyPressed(_key, _char);
		eventKeyPressed(_key, _char);
	}

	inline void MGUI_Widget::_notifyKeyReleased(MGUI_KeyCode _key)
	{
		OnKeyReleased(_key);
		eventKeyReleased(_key);
	}

}