#include "stdafx.h"

#include "IDockPane.h"
#include "xApp.h"

IDockPane::IDockPane(const TString128 & name)
	: mName(name)
{
	xApp::Instance()->AddPane(this);
}

