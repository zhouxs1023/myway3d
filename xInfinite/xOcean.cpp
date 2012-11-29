#include "stdafx.h"
#include "xOcean.h"


xOcean::xOcean()
	: xObj("Ocean")
{
	mOcean = WaterManager::Instance()->CreateOcean();
}

xOcean::~xOcean()
{
}

bool xOcean::OnPropertyChanged(const Property * p)
{
	return false;
}





xObj * xOceanFactory::Create(const char * name)
{
	if (WaterManager::Instance()->GetOcean())
	{
		MessageBox(NULL, "Terrain has created!", "Error", MB_OK);

		return NULL;
	}

	return new xOcean();
}




xOceanFactoryListener gListener;