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
		return "TuQi.jpg";
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
		return "AoXia.jpg";
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
		return "blend.jpg";
	}
};

class xEditTerrain
{
	struct Brush
	{
		TexturePtr texture;
		ImagePtr image;

		float size;
		float density;

		Vec3 position;
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
	void _Init(void *);
	void _Update(void *);
	void _Shutdown(void *);
	void _Render(void *);

	void _UpdateGeometry();
	void _UpdateWeightMap();

protected:
	Brush mBrush;
	Array<TString128> mBrushImages;

	Technique * mTech_Brush;
	Terrain * mTerrain;

	tEventListener<xEditTerrain> OnInit;
	tEventListener<xEditTerrain> OnUpdate;
	tEventListener<xEditTerrain> OnShutdown;
	tEventListener<xEditTerrain> OnRender;
};