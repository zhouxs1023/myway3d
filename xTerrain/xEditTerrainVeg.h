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

	void _Init(void * param0, void * param1);
	void _Shutdown(void * param0, void * param1);
	void _Update(void * param0, void * param1);
	void _Render(void * param0, void * param1);

protected:
	float mBrushSize;
	int mBrushDensity;
	bool mBrushOp_Add;

	Brush mBrush;
	Technique * mTech_Brush;
};