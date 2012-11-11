#pragma once

#include "PropertyDlg.h"

class UiManager
{
	DECLARE_SINGLETON(UiManager);

public:
	UiManager(void);
	~UiManager(void);

public:
	int Init(CFrameWndEx * pParent);

private:
	PropertyDlg	mPropertyDlg;
};