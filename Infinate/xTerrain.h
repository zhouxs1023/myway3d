#pragma once

#include "Shape.h"
#include "Editor.h"

namespace Infinite {

class xTerrain : public Shape
{
public:
	xTerrain();
	virtual ~xTerrain();

	void _create(const Terrain::Config & config);
	void _load(const char * source);

	virtual void Serialize(xSerializer & Serializer);

	virtual TString128 GetTypeName() { return "Terrain"; }

	virtual bool OnPropertyChanged(const Property * p);

protected:
	Terrain * mTerrain;
};


class xTerrainFactory : public ShapeFactory
{
public:
	xTerrainFactory() {};
	virtual ~xTerrainFactory() {};

	virtual Shape * Create(const char * name);

	virtual const char * GetGroupName() { return "Terrain"; }
	virtual const char * GetTypeName() { return "Terrain"; }
};

}
