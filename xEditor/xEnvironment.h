#pragma once


class xEnvironment
{
public:
	xEnvironment();
	~xEnvironment();

	virtual void Init() {}
	virtual void Shutdown() {}

protected:
	void _OnNewScene(void * param0, void * param1);
	void _OnLoadScene(void * param0, void * param1);
	void _OnUnloadScene(void * param0, void * param1);

protected:
	tEventListener<xEnvironment> OnNewScene;
	tEventListener<xEnvironment> OnUnloadScene;
	tEventListener<xEnvironment> OnLoadScene;
};

