#include "stdafx.h"

#include "xPropertyGridColorPick.h"


xPropertyGridColorPick::xPropertyGridColorPick(const CString& strName, const COLORREF& color)
	: CMFCPropertyGridColorProperty(strName, color)
{
}

xPropertyGridColorPick::~xPropertyGridColorPick()
{
}

void xPropertyGridColorPick::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	m_bButtonIsDown = TRUE;
	Redraw();

	CColorDialog dlg(GetColor());

	if (dlg.DoModal() == IDOK)
	{
		COLORREF clr = dlg.m_cc.rgbResult;

		if (clr != GetColor())
		{
			SetColor(clr);
		}
	}

	m_bButtonIsDown = FALSE;
	Redraw();
}