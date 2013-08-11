#pragma once

namespace UIEditor {

class SkinRenderer
{
	DECLARE_SINGLETON(SkinRenderer);

public:
	SkinRenderer();
	~SkinRenderer();

	void SetEnable(bool _enable);
	void SetSkinTexture(TexturePtr _texture);

	void Render(Event * _sender);

protected:
	bool mEnable;
	Technique * mTech;
	RenderOp * mRenderOp;

	TexturePtr mBackTexture;
	TexturePtr mSkinTexture;

	tEventListener<SkinRenderer> OnRender;
};

}