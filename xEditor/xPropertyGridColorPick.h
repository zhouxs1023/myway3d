#pragma once

#include "xEditor.h"

class X_ENTRY xPropertyGridColorPick : public CMFCPropertyGridColorProperty
{
public:
	xPropertyGridColorPick(const CString& strName, const COLORREF& color);
	virtual ~xPropertyGridColorPick();

	virtual void OnClickButton(CPoint point);

};