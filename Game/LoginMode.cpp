#include "stdafx.h"

#include "LoginMode.h"


cLoginMode::cLoginMode()
{
}

cLoginMode::~cLoginMode()
{
}

void cLoginMode::Init()
{
	mLoginDlg = new cLoginDlg;
}

void cLoginMode::Shutdown()
{
	delete mLoginDlg;
}