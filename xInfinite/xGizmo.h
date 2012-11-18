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
	void _initGeo_Move();
	void _initGeo_Move_Render();
	void _initGeo_Rotate();
	void _initGeo_Rotate_Render();
	void _initGeo_Scale();
	void _initGeo_Scale_Render();

	void _renderMove();
	void _renderRotate();
	void _renderScale();

	void _update_Move();
	void _mouseMoved_Move();

	void _update_Rotate();
	void _mouseMoved_Rotate();

	void _update_Scale();
	void _mouseMoved_Scale();


protected:
	Technique * mTech;

	RenderDesc * mRender_Move;
	int mNumVertex_Move;
	int mNumIndex_Move;
	Vec3 * mVertex_Move;
	short * mIndex_Move;

	RenderDesc * mRender_Rotate;
	int mNumVertex_Rotate;
	int mNumIndex_Rotate;
	Vec3 * mVertex_Rotate;
	short * mIndex_Rotate;

	RenderDesc * mRender_Scale;
	int mNumVertex_Scale;
	int mNumIndex_Scale;
	Vec3 * mVertex_Scale;
	short * mIndex_Scale;

	int mPickedAxis;
	bool mPicked;

	tEventListener<xGizmo> OnInit;
	tEventListener<xGizmo> OnShutdown;
	tEventListener<xGizmo> OnRender;
	tEventListener<xGizmo> OnUpdate;
};