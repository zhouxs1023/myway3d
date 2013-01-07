#pragma once

class xForest
{
public:
	xForest();
	~xForest();

protected:
	void _OnNewScene(void * param0, void * param1);
	void _OnLoadScene(void * param0, void * param1);
	void _OnUnloadScene(void * param0, void * param1);
	void _OnSerialize(void * param0, void * param1);

protected:
	tEventListener<xForest> OnNewScene;
	tEventListener<xForest> OnUnloadScene;
	tEventListener<xForest> OnLoadScene;
	tEventListener<xForest> OnSerialize;
};