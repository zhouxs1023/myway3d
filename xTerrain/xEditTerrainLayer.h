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

	void _Init(Event * sender);
	void _Shutdown(Event * sender);
	void _Update(Event * sender);
	void _Render(Event * sender);

	void _RenderSectionLayer();

protected:
	void _UpdateWeightMap();

protected:
	Brush mBrush;
	Technique * mTech_Brush;
	Technique * mTech_Layer;
	int mLayer;
};