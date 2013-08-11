#pragma once

#include "Shape.h"
#include "Editor.h"

namespace Infinite {

	class xSound : public Shape
	{
		DECLARE_PROPERTY(Shape);

		DeclareRTTI();

	public:
		TString128 SoundFile;
		Vec3 Position;
		float MinDist;
		float MaxDist;
		int Volume;

	public:
		xSound(const TString128 & name);
		virtual ~xSound();

		virtual TString128 GetTypeName() { return "Sound"; }

		virtual void SetName(const TString128 & name);
		virtual void SetSoundFile(const TString128 & meshFile);

		virtual Shape * Clone();
		virtual bool IsSceneNode(SceneNode * node);

		virtual void SetPosition(const Vec3 & p);
		virtual void SetMinDist(float r);
		virtual void SetMaxDist(float r);
		virtual void SetVolume(int v);

		virtual Vec3 GetPosition() { return Position; }

		virtual void Serialize(xSerializer & serializer);

		virtual Aabb GetBound();

		virtual bool OnPropertyChanged(const Property * p);

	protected:
		SceneNode * mNode;
		Light * mLight;
		Billboard * mBillboard;
		Sound * mSound;
	};

	class xSoundFactory : public ShapeFactory
	{
	public:
		xSoundFactory() : OnDragFile(xEvent::OnDragFile, this, &xSoundFactory::_OnDragFile) {};
		virtual ~xSoundFactory() {};

		virtual xSound * Create(const char * name) { return new xSound(name); }

		virtual const char * GetGroupName()		{ return "Entity"; }
		virtual const char * GetTypeName()		{ return "Sound"; }
		virtual const char * GetExternName()	{ return "wav"; }

	protected:
		void _OnDragFile(Event * sender);

	protected:
		tEventListener<xSoundFactory> OnDragFile;
	};
}