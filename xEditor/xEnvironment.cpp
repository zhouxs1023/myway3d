#include "stdafx.h"
#include "xEnvironment.h"
#include "xEvent.h"

xEnvironment::xEnvironment()
	: OnNewScene(&xEvent::OnNewScene, this, &xEnvironment::_OnNewScene)
	, OnUnloadScene(&xEvent::OnUnloadScene, this, &xEnvironment::_OnUnloadScene)
{
	int i = 0;
}

xEnvironment::~xEnvironment()
{
}

void xEnvironment::_OnNewScene(void *)
{
	Environment::Instance()->InitEv();
}

void xEnvironment::_OnUnloadScene(void *)
{
	Environment::Instance()->DeInitEv();
}

