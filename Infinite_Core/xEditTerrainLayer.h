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

class xEditTerrainLayer
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
	xEditTerrainLayer();
	~xEditTerrainLayer();

	void SetBrush(const TString128 & tex);

	void SetLayer(int layer) { mLayer = layer; };

	void SetBrushSize(float size) { mBrush.size = size; }
	void SetBrushDensity(float density) { mBrush.density = density; }

	float GetBrushSize() { return mBrush.size; }
	float GetBrushDensity() { return mBrush.density; }

	void _Init();
	void _Shutdown();
	void _Update();
	void _Render();

	void _RenderSectionLayer();

protected:
	void _UpdateWeightMap();

protected:
	Brush mBrush;
	Technique * mTech_Brush;
	Technique * mTech_Layer;
	int mLayer;
};

}