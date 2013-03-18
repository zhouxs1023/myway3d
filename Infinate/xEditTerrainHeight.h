#pragma once


namespace Infinite {

class xEditTerrainHeight
{
	struct Brush
	{
		TexturePtr texture;
		ImagePtr image;

		float size;
		float density;

		Vec3 position;
	};

public:
	enum Op {
		eNone,
		eUp,
		eDown,
		eSmooth
	};

public:
	xEditTerrainHeight();
	~xEditTerrainHeight();

	void SetBrush(const TString128 & tex);

	void SetBrushSize(float size) { mBrush.size = size; }
	void SetBrushDensity(float density) { mBrush.density = density; }

	float GetBrushSize() { return mBrush.size; }
	float GetBrushDensity() { return mBrush.density; }

	void SetOperator(Op op) { mOp = op; }
	Op GetOperator() { return mOp; }

	void _Init();
	void _Shutdown();
	void _Update();
	void _Render();

protected:
	
	void _UpdateGeometry();

protected:
	Op mOp;
	Brush mBrush;
	Technique * mTech_Brush;
};

}
