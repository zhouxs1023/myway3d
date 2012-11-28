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

	void SetSize(float size) { mBrush.size = size; }
	void SetDensity(float density) { mBrush.density = density; }

	float GetSize() { return mBrush.size; }
	float GetDensity() { return mBrush.density; }

	void _Init(void *);
	void _Shutdown(void *);
	void _Update(void *);
	void _Render(void *);

protected:
	void _UpdateWeightMap();

protected:
	Brush mBrush;
	Technique * mTech_Brush;
	int mLayer;
};