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
#include "MForest.h"
#include "xEvent.h"

namespace Infinite {

class xTree : public Shape
{
	DECLARE_PROPERTY(Shape);

public:
	TString128 TreeFile;
	Vec3 Position;
	Quat Orientation;
	float Scale;
	Color4 LeafDiffuse;
	Color4 FrondDiffuse;
	Color4 BranchDiffuse;

public:
	xTree(const TString128 & name);
	virtual ~xTree();

	virtual TString128 GetTypeName() { return "Tree"; }

	virtual void SetName(const TString128 & name);
	virtual void SetTreeFile(const TString128 & meshFile);

	virtual Shape * Clone();
	virtual bool IsSceneNode(SceneNode * node);

	virtual void SetPosition(const Vec3 & p);
	virtual void SetOrientation(const Quat & q);
	virtual void SetScale(float s);

	virtual Vec3 GetPosition();
	virtual Quat GetOrientation();
	virtual float GetScale();

			void SetLeafDiffuse(const Color4 & c);
			void SetFrondDiffuse(const Color4 & c);
			void SetBranchDiffuse(const Color4 & c);

	virtual void Serialize(xSerializer & serializer);

	virtual Aabb GetBound();

	virtual ColMesh * GetColMesh();

	virtual bool OnPropertyChanged(const Property * p);

protected:
	SceneNode * mNode;
	MTreeInstance * mTree;
};

class xTreeFactory : public ShapeFactory
{
public:
	xTreeFactory() : OnDragFile(xEvent::OnDragFile, this, &xTreeFactory::_OnDragFile) {};
	virtual ~xTreeFactory() {};

	virtual xTree * Create(const char * name) { return new xTree(name); }

	virtual const char * GetGroupName()		{ return "Entity"; }
	virtual const char * GetTypeName()		{ return "Tree"; }
	virtual const char * GetExternName()	{ return "spt"; }

protected:
	void _OnDragFile(Event * sender);

	tEventListener<xTreeFactory> OnDragFile;
};

}
