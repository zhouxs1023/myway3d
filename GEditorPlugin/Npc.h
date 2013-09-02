#pragma once


class Npc : public Shape
{
	DECLARE_PROPERTY(Shape);

public:
	Vec3 Position;
	Quat Orientation;
	float Scale;

public:
	Npc(const TString128 & name);
	virtual ~Npc();

	virtual TString128 GetTypeName() { return "Npc"; }

	virtual void SetName(const TString128 & name);

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

	virtual bool OnPropertyChanged(const Property * p);

protected:
	GmNpc * mNpc;
};


class NpcFactory : public ShapeFactory
{
public:
	NpcFactory() {};
	virtual ~NpcFactory() {};

	virtual Shape * Create(const char * name) { return new Npc(name); }

	virtual const char * GetGroupName()		{ return "Game"; }
	virtual const char * GetTypeName()		{ return "Npc"; }
	virtual const char * GetExternName()	{ return "npc"; }
};

