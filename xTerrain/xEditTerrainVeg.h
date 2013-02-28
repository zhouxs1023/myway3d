#pragma once

class xEditTerrainVeg
{
	struct Brush
	{
		float size;
		Vec3 position;
	};

public:
	xEditTerrainVeg();
	~xEditTerrainVeg();

	void SetBrushSize(float size) { mBrushSize = size; }
	void SetBrushDensity(int density) { mBrushDensity = density; }
	void SetBrushOp(bool bAdd) { mBrushOp_Add = bAdd; }

	void _Init(Event * sender);
	void _Shutdown(Event * sender);
	void _Update(Event * sender);
	void _Render(Event * sender);

protected:
	float mBrushSize;
	int mBrushDensity;
	bool mBrushOp_Add;

	Brush mBrush;
	Technique * mTech_Brush;
};