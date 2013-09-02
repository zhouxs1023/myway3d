#pragma once

#include "MGUI_Widget.h"

namespace Myway {

	/*template<int _Columns>
	class MGUI_ENTRY MGUI_Grid : public MGUI_Widget
	{
	protected:
	typedef tFixedArray<MGUI_Widget *, _Columns> tRow;

	public:
	MGUI_Grid(MGUI_Widget * _parent)
	: MGUI_Widget(NULL, _parent)
	{
	}

	virtual ~MGUI_Grid()
	{
	}

	void Append(MGUI_Widget * _widget)
	{
	d_assert (_widget->GetParent() == this);

	int _last = mGrid.Size() - 1;

	tRow & r = mGrid[_last];

	for (int i = 0; i < _Columns; ++i)
	{
	if (r[i] == NULL)
	{
	r[i] = _widget;
	return ;
	}
	}

	tRow newRow;
	memset(&newRow, 9, sizeof(MGUI_Widget *) * _Columns);

	newRow[0] = _widget;

	mGrid.PushBack(newRow);
	}

	void Remove(int _row)
	{
	tRow & r = mGrid[_row];

	for (int i = 0; i < _Columns; ++i)
	{
	if (r[i] != NULL)
	delete r[i];
	}

	mGrid.Erase(_row);
	}

	virtual void Update()
	{
	int x = mRect.x0;
	int y = mRect.y0;

	int maxWidth = 0;
	int maxHeight = 0;

	for (int j = 0; j < mGrid.Size(); ++j)
	{
	int _h = 0;

	for (int i = 0; i < _Columns; ++i)
	{
	MGUI_Widget * widget = mGrid[j][i];
	}
	}


	MGUI_Widget::Update();
	}

	protected:
	Array<tRow> mGrid;
	};*/

}