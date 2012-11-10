#pragma once

#include "resource.h"       // Ö÷·ûºÅ

class MyApp : public CWinApp
{
    DECLARE_MESSAGE_MAP()

public:
	MyApp();

// ÖØÐ´
public:
	virtual BOOL InitInstance();
	virtual int Run();
	virtual void OnIdle();

};

extern MyApp theApp;