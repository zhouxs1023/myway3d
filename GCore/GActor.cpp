#include "stdafx.h"

#include "GActor.h"

namespace game {

	TString128 _ActorPartName[] = {
		TString128("Main"),
		TString128("Weapon"),
		TString128("Helmet"),
		TString128("Shoulder"),
		TString128("Clothes"),
		TString128("Shoes")
	};

	GActor::GActor(const char * name)
	{
		mName = name;

		for (int i = 0; i < PT_Max; ++i)
		{
			mActor[i] = NULL;
		}
	}

	GActor::~GActor()
	{
		for (int i = 0; i < PT_Max; ++i)
		{
			if (mActor[i])
				World::Instance()->DestroyActor(mActor[i]);
		}
	}

	void GActor::SetPart(PartType type, const char * mesh)
	{
		if (mActor[type])
		{
			World::Instance()->DestroyActor(mActor[type]);
			mActor[type] = NULL;
		}

		if (strcmp(mesh, "") == 0)
			return ;

		TString128 uname = mName + "_" + _ActorPartName[type];

		mActor[type] = World::Instance()->CreateActor(uname, mesh);

		mNode->Attach(mActor[type]);
	}
}