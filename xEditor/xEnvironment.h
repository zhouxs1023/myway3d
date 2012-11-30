#pragma once


class xEnvironment
{
public:
	xEnvironment();
	~xEnvironment();

	virtual void Init() {}
	virtual void Shutdown() {}

protected:
	void _OnNewScene(void *);
	void _OnUnloadScene(void *);

protected:
	tEventListener<xEnvironment> OnNewScene;
	tEventListener<xEnvironment> OnUnloadScene;
};

