//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

namespace Infinite {


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

	void _Init();
	void _Shutdown();
	void _Update();
	void _Render();

protected:
	float mBrushSize;
	int mBrushDensity;
	bool mBrushOp_Add;

	Brush mBrush;
	Technique * mTech_Brush;
};

}
