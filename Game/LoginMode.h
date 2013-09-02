#pragma once

#include "LoginDlg.h"

class cLoginMode : public GmMode
{
public:
	cLoginMode();
	virtual ~cLoginMode();

	virtual void Init();
	virtual void Shutdown();

protected:
	cLoginDlg * mLoginDlg;
};