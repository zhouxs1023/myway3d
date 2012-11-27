#pragma once


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

	void SetSize(float size) { mBrush.size = size; }
	void SetDensity(float density) { mBrush.density = density; }

	float GetSize() { return mBrush.size; }
	float GetDensity() { return mBrush.density; }

	void SetOperator(Op op) { mOp = op; }
	Op GetOperator() { return mOp; }

	void _Init(void *);
	void _Shutdown(void *);
	void _Update(void *);
	void _Render(void *);

protected:
	
	void _UpdateGeometry();

protected:
	Op mOp;
	Brush mBrush;
	Technique * mTech_Brush;
};
