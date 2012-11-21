#pragma once


class xTerrainBrush
{
public:
	xTerrainBrush();

protected:
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