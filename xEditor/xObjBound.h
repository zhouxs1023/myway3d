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
	void RenderColMesh();
	
protected:
	Technique * mTech;
	RenderOp * mRenderAABB;
	RenderOp * mRenderColMesh;

	tEventListener<xObjBound> OnInit;
	tEventListener<xObjBound> OnShutdown;
	tEventListener<xObjBound> OnRender;
};