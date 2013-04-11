#include "stdafx.h"

#include "GLight.h"

namespace game {

	GLight::GLight(const char * name)
	{
		mLight = World::Instance()->CreateLight(name);
		mNode->Attach(mLight);
	}

	GLight::~GLight()
	{
		World::Instance()->DestroyLight(mLight);
	}

	Light * GLight::getLight()
	{
		return mLight;
	}

}