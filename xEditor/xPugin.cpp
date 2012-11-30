#include "stdafx.h"
#include "xPlugin.h"

xPluginManager gPluginManager;

IMP_SLN(xPluginManager);

xPluginManager::xPluginManager()
{
	INIT_SLN;
}

xPluginManager::~xPluginManager()
{
	SHUT_SLN;
	d_assert (mPlugins.Size() == 0);
}

void xPluginManager::Add(xPlugin * plugin)
{
	mPlugins.PushBack(plugin);
}

void xPluginManager::Init()
{
	for (int i = 0; i < mPlugins.Size(); ++i)
	{
		mPlugins[i]->Init();
	}
}

void xPluginManager::Shutdown()
{
	for (int i = 0; i < mPlugins.Size(); ++i)
	{
		mPlugins[i]->Shutdown();
		delete[] mPlugins[i];
	}

	mPlugins.Clear();
}