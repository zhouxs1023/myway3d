#pragma once

class xObjBound
{
public:
	xObjBound();
	virtual ~xObjBound();

	void Init(void * param0, void * param1);
	void Shutdown(void * param0, void * param1);
	void Render(void * param0, void * param1);

protected:
	Technique * mTech;
	RenderOp * mRender;

	tEventListener<xObjBound> OnInit;
	tEventListener<xObjBound> OnShutdown;
	tEventListener<xObjBound> OnRender;
};