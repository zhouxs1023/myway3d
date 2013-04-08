#include "stdafx.h"
#include "xSound.h"
#include "Editor.h"

namespace Infinite {

	DF_PROPERTY_BEGIN(xSound)
		DF_PROPERTY(xSound, SoundFile, "General", "File", PT_TString, 128)
		DF_PROPERTY(xSound, Position, "Transform", "Position", PT_Vec3, 12)
		DF_PROPERTY(xSound, MinDist, "General", "MinDist", PT_Float, 4)
		DF_PROPERTY(xSound, MaxDist, "General", "MaxDist", PT_Float, 4)
		DF_PROPERTY(xSound, Volume, "General", "Volume", PT_Int, 4)
	DF_PROPERTY_END();

	xSound::xSound(const TString128 & name)
		: Shape(name)
		, mSound(NULL)
	{
		SoundFile = "";
		Position = Vec3::Zero;
		MinDist = 50;
		MaxDist = 100;
		Volume = 1000;

		mNode = World::Instance()->CreateSceneNode();

		Technique * tech = Editor::Instance()->GetHelperShaderLib()->GetTechnique("Billboard");
		mBillboard = BillboardManager::Instance()->Create(tech);

		mBillboard->SetWidth(20);
		mBillboard->SetHeight(20);
		mBillboard->SetBounds(Aabb::Identiy * 20, Sphere::Identity * 20);

		Material * mat = mBillboard->GetMaterial();

		mat->SetDepthWrite(false);
		mat->SetBlendMode(BM_ALPHA_BLEND);
		mat->SetDiffuseMap("Editor\\sound.dds");

		mNode->Attach(mBillboard);

		mNode->GetFlag().SetFlags(PICK_Flag);
	}

	xSound::~xSound()
	{
		if (mSound)
			AudioSystem::Instance()->DestroySound(mSound);
		
		BillboardManager::Instance()->Destroy(mBillboard);
		World::Instance()->DestroySceneNode(mNode);
	}

	Aabb xSound::GetBound()
	{
		Aabb bound;

		bound.minimum = mNode->GetPosition() - MaxDist / 2;
		bound.maximum = mNode->GetPosition() + MaxDist / 2;
		
		return bound;
	}

	void xSound::Serialize(xSerializer & Serializer)
	{
		Shape::Serialize(Serializer);

		int version = 0;

		if (Serializer.IsSave())
		{
			Serializer << version;
			Serializer << SoundFile;
			Serializer << Position;
			Serializer << MinDist;
			Serializer << MaxDist;
			Serializer << Volume;
		}
		else
		{
			Serializer >> version;
			Serializer >> SoundFile;
			Serializer >> Position;
			Serializer >> MinDist;
			Serializer >> MaxDist;
			Serializer >> Volume;

			SetSoundFile(SoundFile);
			SetPosition(Position);
			SetMinDist(MinDist);
			SetMaxDist(MaxDist);
			SetVolume(Volume);
		}
	}

	bool xSound::OnPropertyChanged(const Property * p)
	{
		bool r = false;

		if (p->name == "Name")
		{
			SetName(Name);
			r = true;
		}
		else if (p->name == "SoundFile")
		{
			SetSoundFile(SoundFile);
			r = true;
		}
		else if (p->name == "Position")
		{
			SetPosition(Position);
			r = true;
		}
		else if (p->name == "MinDist")
		{
			SetMinDist(MinDist);
			r = true;
		}
		else if (p->name == "MaxDist")
		{
			SetMaxDist(MaxDist);
			r = true;
		}
		else if (p->name == "Volume")
		{
			SetVolume(Volume);
			r = true;
		}

		return r;
	}

	void xSound::SetName(const TString128 & name)
	{
		Name = name;
		
		if (mSound)
			AudioSystem::Instance()->RenameSound(name.c_str(), mSound->GetName());
	}

	void xSound::SetSoundFile(const TString128 & file)
	{
		if (SoundFile == file)
			return ;

		SoundFile = file;

		if (mSound)
			AudioSystem::Instance()->DestroySound(mSound);

		mSound = NULL;

		if (file == "")
			return ;

		mSound = AudioSystem::Instance()->CreateSound(Name.c_str(), SoundFile.c_str());

		mSound->SetPosition(mNode->GetPosition());
		mSound->SetDistance(MinDist, MaxDist);
		mSound->SetVolume(int(Volume));

		mSound->Play();
	}

	Shape * xSound::Clone()
	{
		xSound * sound = (xSound *)ShapeManager::Instance()->Create(GetTypeName().c_str());

		sound->SetPosition(Position);
		sound->SetMinDist(MinDist);
		sound->SetMaxDist(MaxDist);
		sound->SetVolume(Volume);
		sound->SetSoundFile(SoundFile);

		return sound;
	}

	bool xSound::IsSceneNode(SceneNode * node)
	{
		return node == mNode;
	}

	void xSound::SetPosition(const Vec3 & position)
	{
		Position = position;
		mNode->SetPosition(position);

		if (mSound)
			mSound->SetPosition(mNode->GetPosition());
	}

	void xSound::SetMinDist(float r)
	{
		MinDist = r;

		if (mSound)
			mSound->SetDistance(MinDist, MaxDist);
	}

	void xSound::SetMaxDist(float r)
	{
		MaxDist = r;

		if (mSound)
			mSound->SetDistance(MinDist, MaxDist);
	}

	void xSound::SetVolume(int v)
	{
		Volume = v;

		if (mSound)
			mSound->SetVolume(Volume);
	}















	void xSoundFactory::_OnDragFile(Event * sender)
	{
		Point2f pt = *(Point2f*)sender->GetParam(0);
		TString128 file = (const char *)sender->GetParam(1);

		file.Lower();
		file.Replace('/', '\\');

		TString128 externName;

		externName = File::GetExternName(file);

		if (externName != "wav")
			return ;

		const char * testFile = file.c_str();
		int length = file.Length();

		bool exist = false;

		while (length > 0)
		{
			if (testFile[length - 1] == '\\')
			{
				const char * tfile = testFile + length;
				exist = ResourceManager::Instance()->Exist(tfile);

				if (exist)
					break;
			}

			--length;
		}

		if (!exist)
			return ;

		testFile += length;

		xSound * obj = (xSound *)ShapeManager::Instance()->Create("Sound");

		d_assert (obj);

		Vec3 pos = Editor::Instance()->GetHitPosition(pt.x, pt.y);

		obj->SetPosition(pos);
		obj->SetSoundFile(testFile);

		Editor::Instance()->SetSelectedShape(obj);
	}
}