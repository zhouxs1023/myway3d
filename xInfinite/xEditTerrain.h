#pragma once


class xTerrainBrush
{
public:
	xTerrainBrush();
	~xTerrainBrush();

protected:
	ImagePtr mImage;
	float mSize;
	float mDensity;
	Vec3 mPosition;
};

class xEditTerrain
{
public:
	xEditTerrain();
	~xEditTerrain();

	void Create(const Terrain::Config & config);
	void AddLayer(const Terrain::Layer & layer);

protected:
};