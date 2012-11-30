#pragma once

#include "xEditor.h"

class X_ENTRY xPlugin
{
public:
	xPlugin() {}
	virtual ~xPlugin() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};

class X_ENTRY xPluginManager
{
	DECLARE_SINGLETON (xPluginManager);

public:
	xPluginManager();
	~xPluginManager();

	void Add(xPlugin * plugin);

	void Init();
	void Shutdown();

protected:
	Array<xPlugin *> mPlugins;
};