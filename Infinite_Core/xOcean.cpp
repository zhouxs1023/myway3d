#include "stdafx.h"
#include "xOcean.h"


namespace Infinite {

DF_PROPERTY_BEGIN(xOcean)
	DF_PROPERTY(xOcean, Height, "Transform", "Height", PT_Float, 4)
DF_PROPERTY_END()

ImplementRTTI(xOcean, Shape);

xOcean::xOcean()
	: Shape("Ocean")
{
	Height = 10;
	mOcean = WaterManager::Instance()->CreateOcean();
}

xOcean::~xOcean()
{
}

void xOcean::SetHeight(float h)
{
	Height = h;
	mOcean->SetHeight(h);
}

void xOcean::Serialize(xSerializer & Serializer)
{
	Shape::Serialize(Serializer);

	int version = 0;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << Height;
	}
	else
	{
		Serializer >> version;

		if (version == 0)
		{
			Serializer >> Height;
		}

		SetHeight(Height);
	}
}

bool xOcean::OnPropertyChanged(const Property * p)
{
	if (p->name == "Height")
	{
		SetHeight(Height);
		return true;
	}

	return false;
}

Shape * xOceanFactory::Create(const char * name)
{
	if (WaterManager::Instance()->GetOcean())
	{
		HWND hWnd = Engine::Instance()->GetDeviceProperty()->hWnd;

		MessageBox(hWnd, "Ocean has created!", "Error", MB_OK);

		return NULL;
	}

	return new xOcean();
}




}
