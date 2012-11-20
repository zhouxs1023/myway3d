#include "stdafx.h"
#include "xToolBar.h"
#include "xApp.h"
#include "resource.h"

Event xToolBar::OnCommand;

IMP_SLN(xToolBar);

xToolBar::xToolBar()
	: mOnInitUI(xApp::OnInitUI, this, &xToolBar::Init)
{
	INIT_SLN;
	mNumButtons = 0;
}

xToolBar::~xToolBar()
{
	SHUT_SLN;
}

void xToolBar::Add(const char * icon, UINT btn)
{
	d_assert (mNumButtons < MAX_BUTTONS);

	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), icon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	d_assert (hIcon != NULL);

	int idx = mImageList.AddIcon(hIcon);

	d_assert (idx >= 0);

	mButtons[mNumButtons] = btn;
	++mNumButtons;
}

void xToolBar::Init(void *)
{
	SetUserImages(&mImageList);

	for (int i = 0; i < mNumButtons; ++i)
	{
		CMFCToolBarButton * btn = new CMFCToolBarButton(mButtons[0], i, NULL, TRUE, TRUE);

		InsertButton(btn);
	}

	ShowPane(TRUE,FALSE,TRUE);
}

BOOL xToolBar::PreTranslateMessage(MSG * msg)
{
	if (msg->message == WM_COMMAND)
	{
		WORD id = LOWORD(msg->wParam);

		for (int i = 0; i < mNumButtons; ++i)
		{
			if (mButtons[i] == id)
				OnCommand.Call(&mButtons[i]);
		}
	}

	return CMFCToolBar::PreTranslateMessage(msg);
}