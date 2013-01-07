#include "stdafx.h"
#include "xForest.h"
#include "xEvent.h"
#include "xSerializer.h"


xForest gForest;

xForest::xForest()
	: OnNewScene(&xEvent::OnNewScene, this, &xForest::_OnNewScene)
	, OnLoadScene(&xEvent::OnLoadScene, this, &xForest::_OnLoadScene)
	, OnUnloadScene(&xEvent::OnUnloadScene, this, &xForest::_OnUnloadScene)
	, OnSerialize(&xEvent::OnSerialize, this, &xForest::_OnSerialize)
{
}

xForest::~xForest()
{
}

void xForest::_OnNewScene(void * param0, void * param1)
{
}

void xForest::_OnLoadScene(void * param0, void * param1)
{
}

void xForest::_OnUnloadScene(void * param0, void * param1)
{
}

void xForest::_OnSerialize(void * param0, void * param1)
{
}
