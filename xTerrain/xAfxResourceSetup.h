#pragma once


extern HANDLE ghModule;

class AfxResourceReseter
{
public:
	AfxResourceReseter()
	{
		hInstance = AfxGetResourceHandle();
		AfxSetResourceHandle((HINSTANCE)ghModule);
	}

	~AfxResourceReseter()
	{
		AfxSetResourceHandle(hInstance);
	}

protected:
	HINSTANCE hInstance;
};


#define afx_resource_setup() AfxResourceReseter ___arr