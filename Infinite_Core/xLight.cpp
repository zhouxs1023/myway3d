#include "stdafx.h"

#include "xLight.h"
#include "Editor.h"

namespace Infinite {

	DF_PROPERTY_BEGIN(xPointLight)
		DF_PROPERTY(xPointLight, Position, "General", "Position", PT_Vec3, 12)
		DF_PROPERTY(xPointLight, Diffuse, "General", "Diffuse", PT_Color, 16)
		DF_PROPERTY(xPointLight, Specular, "General", "Specular", PT_Color, 16)
		DF_PROPERTY(xPointLight, Range, "General", "Range", PT_Float, 4)
	DF_PROPERTY_END();

	ImplementRTTI(xPointLight, Shape);

	xPointLight::xPointLight(const TString128 & name)
		: Shape(name)
	{
		Position = Vec3::Zero;
		Diffuse = Color4::White;
		Specular = Color4::Black;

		Range = 20;

		mLight = World::Instance()->CreateLight(name);
		mNode = World::Instance()->CreateSceneNode();

		mNode->Attach(mLight);

		mNode->GetFlag().SetFlags(PICK_Flag);

		mLight->SetBounds(Aabb(-1, -1, -1, 1, 1, 1), Sphere(0, 0, 0, 1));
		mLight->SetType(LT_POINT);
	
		Technique * tech = Editor::Instance()->GetHelperShaderLib()->GetTechnique("PointLight");
		mBillboard = BillboardManager::Instance()->Create(tech);

		mBillboard->SetWidth(5);
		mBillboard->SetHeight(5);

		Material * mat = mBillboard->GetMaterial();

		mat->SetDepthWrite(false);
		mat->SetBlendMode(BM_ALPHA_BLEND);
		mat->SetDiffuseMap("Editor\\PointLight.png");

		mNode->Attach(mBillboard);

		SetScale(Range);
		SetDiffuse(Diffuse);
		SetSpecular(Specular);
	}

	xPointLight::~xPointLight()
	{
		BillboardManager::Instance()->Destroy(mBillboard);
		World::Instance()->DestroyLight(mLight);
		World::Instance()->DestroySceneNode(mNode);
	}


	void xPointLight::SetName(const TString128 & name)
	{
		if (World::Instance()->RenameLight(name, mLight))
			Name = name;
	}

	Shape * xPointLight::Clone()
	{
		xPointLight * light = (xPointLight *)ShapeManager::Instance()->Create(GetTypeName().c_str());

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

	void xPointLight::SetScale(float s)
	{
		mNode->SetScale(s);
		SetRange(s);
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
		Shape::Serialize(Serializer);

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
			SetScale(Range);
		}

		return true;
	}
}
