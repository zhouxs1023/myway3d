#pragma once


class xGizmo
{
public:
	xGizmo();
	~xGizmo();

	void Init(void * data);
	void Shutdown(void * data);
	void Render(void * data);
	void Update(void * data);

protected:
	void _initGeo();
	void _mouseMoved();

protected:
	Technique * mTech;
	RenderDesc * mRender;

	int mNumVertex_Move;
	int mNumIndex_Move;
	Vec3 * mVertex_Move;
	short * mIndex_Move;
	int mPickedAxis;
	bool mNeedMove;

	tEventListener<xGizmo> OnInit;
	tEventListener<xGizmo> OnShutdown;
	tEventListener<xGizmo> OnRender;
	tEventListener<xGizmo> OnUpdate;
};