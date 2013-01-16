#pragma once

#include "MWRenderer.h"
#include "MWMover.h"
#include "MWRenderQueue.h"

namespace Myway
{

class MW_ENTRY Billboard : public Mover, public Renderer
{
	DECLARE_ALLOC();

public:
    Billboard(Technique * tech);
    virtual ~Billboard();

	void SetTexcoord(const RectF & rc);
	void SetColor(const Color4 & color);

	void SetWidth(float width);
	void SetHeight(float height);

	float GetWidth();
	float GetHeight();
	const RectF & GetTexcoord();

	virtual const Aabb & GetWorldAabb();
	virtual void GetWorldPosition(Vec3 * pos);
	virtual void GetWorldTransform(Mat4 * form);

	virtual void UpdateGeometry();
	virtual void AddRenderQueue(RenderQueue * RQ);

	virtual Technique *	GetTechnique(eRenderTechType::enum_t type);

protected:
	void _initGeometry();

protected:
	RectF				mTexcoord;
    Color4              mColor;
    float               mWidth;
    float               mHeight;
	bool				mNeedUpdate;
	Technique *			mTechnique;
};

class MW_ENTRY BillboardManager
{
	DECLARE_SINGLETON(BillboardManager);

public:
	BillboardManager();
	~BillboardManager();

	Billboard * Create(Technique * tech);
	void Destroy(Billboard * billboard);
};

}