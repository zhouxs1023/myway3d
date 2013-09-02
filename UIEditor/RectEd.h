#pragma once

namespace UIEditor {

	class RectEd : public MGUI_Widget
	{
	public:
		RectEd(MGUI_Widget * _parent);
		~RectEd();

		void SetEditRect(const MGUI_Rect & _rect);
		const MGUI_Rect & GetEditRect();

	protected:
		virtual void OnMouseLostFocus(MGUI_Widget* _new);
		virtual void OnMouseSetFocus(MGUI_Widget* _old);
		virtual void OnMouseDrag(int _x, int _y);
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id);
		virtual void OnMouseReleased(int _x, int _y, MGUI_MouseButton _id);

	protected:

		MGUI_Widget * mLeft;
		MGUI_Widget * mRight;
		MGUI_Widget * mTop;
		MGUI_Widget * mBottom;

		int mX, mY;
	};

}