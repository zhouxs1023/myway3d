#pragma once

#include "xBaseOperator.h"

class xTerrainTuQiOp : public xBaseOp
{
public:
	static const int eOp_TuQi;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_TuQi);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\TerrainTuQi.ico";
	}
};

class xTerrainAoXiaOp : public xBaseOp
{
public:
	static const int eOp_AoXia;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_AoXia);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\TerrainAoXiao.ico";
	}
};

class xTerrainBlendOp : public xBaseOp
{
public:
	static const int eOp_Blend;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_Blend);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\TerrainBlend.ico";
	}
};

class xEditTerrain
{
	struct Brush
	{
		TexturePtr texture;

		float size;
		float density;
	};

	DECLARE_SINGLETON(xEditTerrain);

public:
	xEditTerrain();
	~xEditTerrain();

	void Create(const Terrain::Config & config);
	void AddLayer(const Terrain::Layer & layer);

	void SetBrush(const TString128 & tex);

	void SetSize(float size) { mBrush.size = size; }
	void SetDensity(float density) { mBrush.density = density; }

	float GetSize() { return mBrush.size; }
	float GetDensity() { return mBrush.density; }

protected:
	void _Update(void *);
	void _Shutdown(void *);
	void _Render(void *);

protected:
	Brush mBrush;
};