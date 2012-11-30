#include "stdafx.h"
#include "xOcean.h"


DF_PROPERTY_BEGIN(xOcean)
	DF_PROPERTY(xOcean, Height, "Transform", "Height", PT_Float, 4)
DF_PROPERTY_END()

xOcean::xOcean()
	: xObj("Ocean")
{
	Height = 10;
	mOcean = WaterManager::Instance()->CreateOcean();
}

xOcean::~xOcean()
{
}

bool xOcean::OnPropertyChanged(const Property * p)
{
	if (p->name == "Height")
	{
		_setHeight(Height);
		return true;
	}

	return false;
}

void xOcean::_setHeight(float h)
{
	mOcean->SetHeight(h);
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