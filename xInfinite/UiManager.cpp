#include "stdafx.h"
#include "UiManager.h"
#include "resource.h"

UiManager g_UiMgr;

IMP_SLN(UiManager);

UiManager::UiManager(void)
{
	INIT_SLN;
}

UiManager::~UiManager(void)
{
	SHUT_SLN;
}


int UiManager::Init(CFrameWndEx * pParent)
{
	/*int iWidth = GetSystemMetrics(SM_CXSCREEN);
	int iHeight = GetSystemMetrics(SM_CXSCREEN);
	pParent->MoveWindow(0, 0, iWidth, iHeight);

	mPropertyDlg.Create("Property", pParent, CRect(0,0,400, 600), TRUE, IDD_Property,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);

	mPropertyDlg.EnableDocking(CBRS_ALIGN_ANY);
	pParent->EnableDocking(CBRS_ALIGN_ANY);
	pParent->DockPane(&mPropertyDlg);

	mPropertyDlg.Init();*/

	return 0;
}

