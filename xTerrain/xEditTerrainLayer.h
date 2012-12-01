#pragma once


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

	void _Init(void * param0, void * param1);
	void _Shutdown(void * param0, void * param1);
	void _Update(void * param0, void * param1);
	void _Render(void * param0, void * param1);

	void _RenderSectionLayer();

protected:
	void _UpdateWeightMap();

protected:
	Brush mBrush;
	Technique * mTech_Brush;
	Technique * mTech_Layer;
	int mLayer;
};