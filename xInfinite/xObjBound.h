#pragma once

class xObjBound
{
public:
	xObjBound();
	virtual ~xObjBound();

	void Init(void * data);
	void Shutdown(void * data);
	void Render(void * data);

protected:
	Technique * mTech;
	RenderDesc * mRender;

	tEventListener<xObjBound> OnInit;
	tEventListener<xObjBound> OnShutdown;
	tEventListener<xObjBound> OnRender;
};