#include "stdafx.h"

#include "xLight.h"

DF_PROPERTY_BEGIN(xPointLight)
	DF_PROPERTY(xPointLight, Position, "Generic", "Position", PT_Vec3, 12)
	DF_PROPERTY(xPointLight, Diffuse, "Generic", "Diffuse", PT_Color, 16)
	DF_PROPERTY(xPointLight, Specular, "Generic", "Specular", PT_Color, 16)
	DF_PROPERTY(xPointLight, Range, "Generic", "Range", PT_Float, 4)
DF_PROPERTY_END();

xPointLight::xPointLight(const TString128 & name)
	: xObj(name)
{
	mLight = World::Instance()->CreateLight(name);
	mNode = World::Instance()->CreateSceneNode();

	mNode->Attach(mLight);

	mNode->GetFlag().SetFlags(PICK_Flag);

	mLight->SetBounds(xObj::K_DefaultBound, xObj::K_DefaultSphere);
}

xPointLight::~xPointLight()
{
}


void xPointLight::SetName(const TString128 & name)
{
}


xObj * xPointLight::Clone()
{
	xPointLight * light = (xPointLight *)xObjManager::Instance()->Create(GetTypeName().c_str());

	light->SetPosition(Position);
	light->SetDiffuse(Diffuse);
	light->SetSpecular(Specular);
	light->SetRange(Range);

	return light;
}

bool xPointLight::IsSceneNode(SceneNode * node)
{
	return mNode == node;
}

void xPointLight::SetPosition(const Vec3 & p)
{
	Position = p;
	mNode->SetPosition(p);
}

void xPointLight::SetDiffuse(const Color4 & clr)
{
	Diffuse = clr;
	mLight->SetDiffuse(clr);
}

void xPointLight::SetSpecular(const Color4 & clr)
{
	Specular = clr;
	mLight->SetSpecular(clr);
}

void xPointLight::SetRange(float range)
{
	Range = range;
	mLight->SetRange(range);
}

void xPointLight::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 0;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << Position;
		Serializer << Diffuse;
		Serializer << Specular;
		Serializer << Range;
	}
	else
	{
		Serializer >> version;
		if (version == 0)
		{
			Serializer >> Position;
			Serializer >> Diffuse;
			Serializer >> Specular;
			Serializer >> Range;
		}

		SetPosition(Position);
		SetDiffuse(Diffuse);
		SetSpecular(Specular);
		SetRange(Range);
	}
}

Aabb xPointLight::GetBound()
{
	return mLight->GetWorldAabb();
}

bool xPointLight::OnPropertyChanged(const Property * p)
{
	if (p->name == "Name")
	{
		SetName(Name);
	}
	else if (p->name == "Position")
	{
		SetPosition(Position);
	}
	else if (p->name == "Diffuse")
	{
		SetDiffuse(Diffuse);
	}
	else if (p->name == "Specular")
	{
		SetSpecular(Specular);
	}
	else if (p->name == "Range")
	{
		SetRange(Range);
	}

	return true;
}

xLightFactoryListener gLightFactoryListener;