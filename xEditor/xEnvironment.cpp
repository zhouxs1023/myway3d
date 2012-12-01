#include "stdafx.h"
#include "xEnvironment.h"
#include "xEvent.h"

xEnvironment::xEnvironment()
	: OnNewScene(&xEvent::OnNewScene, this, &xEnvironment::_OnNewScene)
	, OnLoadScene(&xEvent::OnLoadScene, this, &xEnvironment::_OnLoadScene)
	, OnUnloadScene(&xEvent::OnUnloadScene, this, &xEnvironment::_OnUnloadScene)
{
	int i = 0;
}

xEnvironment::~xEnvironment()
{
}

void xEnvironment::_OnNewScene(void * param0, void * param1)
{
	Environment::Instance()->InitEv();
}

void xEnvironment::_OnLoadScene(void * param0, void * param1)
{
	Environment::Instance()->InitEv();
}

void xEnvironment::_OnUnloadScene(void * param0, void * param1)
{
	Environment::Instance()->DeInitEv();
}

