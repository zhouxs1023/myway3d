#pragma once

class xObjBound
{
public:
	xObjBound();
	virtual ~xObjBound();

	void Init(Event * sender);
	void Shutdown(Event * sender);
	void Render(Event * sender);

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