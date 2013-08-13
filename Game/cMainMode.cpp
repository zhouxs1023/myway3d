#include "stdafx.h"

#include "cMainMode.h"

namespace Game {

	cMainMode::cMainMode()
	{
	}

	cMainMode::~cMainMode()
	{
	}

	void cMainMode::Init()
	{
		mMap.Load("test003.gmap");

		AddController(new GmCameraControllerTest);
	}

	void cMainMode::Shutdown()
	{
	}
}


