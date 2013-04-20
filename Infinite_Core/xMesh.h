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

	class xMesh : public Shape
	{
		DECLARE_PROPERTY(Shape);

	public:
		TString128 MeshFile;
		Vec3 Position;
		Quat Orientation;
		float Scale;

	public:
		xMesh(const TString128 & name);
		virtual ~xMesh();

		virtual TString128 GetTypeName() { return "Mesh"; }

		virtual void SetName(const TString128 & name);
		virtual void SetMeshFile(const TString128 & meshFile);

		virtual Shape * Clone();
		virtual bool IsSceneNode(SceneNode * node);

		virtual void SetPosition(const Vec3 & p);
		virtual void SetOrientation(const Quat & q);
		virtual void SetScale(float s);

		virtual Vec3 GetPosition() { return Position; }
		virtual Quat GetOrientation() { return Orientation; }
		virtual float GetScale() { return Scale; }

		virtual void Serialize(xSerializer & serializer);

		virtual Aabb GetBound();
		virtual ColMesh * GetColMesh();

		virtual bool OnPropertyChanged(const Property * p);

	protected:
		SceneNode * mNode;
		Entity * mEntity;
	};

	class xMeshFactory : public ShapeFactory
	{
	public:
		xMeshFactory() : OnDragFile(xEvent::OnDragFile, this, &xMeshFactory::_OnDragFile) {};
		virtual ~xMeshFactory() {};

		virtual Shape * Create(const char * name) { return new xMesh(name); }

		virtual const char * GetGroupName()		{ return "Entity"; }
		virtual const char * GetTypeName()		{ return "Mesh"; }
		virtual const char * GetExternName()	{ return "mesh"; }

	protected:
		void _OnDragFile(Event * sender);

	protected:
		tEventListener<xMeshFactory> OnDragFile;
	};

}
