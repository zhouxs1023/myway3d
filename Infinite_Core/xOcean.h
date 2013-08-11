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

#include "Shape.h"
#include "Editor.h"

namespace Infinite {

class xOcean : public Shape
{
	DECLARE_PROPERTY(Shape);

	DeclareRTTI();

protected:
	float Height;

public:
	xOcean();
	virtual ~xOcean();

	void SetHeight(float h);

	virtual TString128 GetTypeName() { return "Ocean"; }
	virtual void Serialize(xSerializer & Serializer);

	virtual bool OnPropertyChanged(const Property * p);

protected:
	Ocean * mOcean;
};

class xOceanFactory : public ShapeFactory
{
public:
	xOceanFactory() {};
	virtual ~xOceanFactory() {};

	virtual Shape * Create(const char * name);

	virtual const char * GetGroupName() { return "Water"; }
	virtual const char * GetTypeName() { return "Ocean"; }
};

}
