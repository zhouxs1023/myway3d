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
			mEntities[i] = NULL;
		}
	}

	GActor::~GActor()
	{
		for (int i = 0; i < PT_Max; ++i)
		{
			if (mEntities[i])
				World::Instance()->DestroyEntity(mEntities[i]);
		}
	}

	void GActor::SetPart(PartType type, const char * mesh)
	{
		if (mEntities[type])
		{
			World::Instance()->DestroyEntity(mEntities[type]);
			mEntities[type] = NULL;
		}

		if (strcmp(mesh, "") == 0)
			return ;

		TString128 uname = mName + "_" + _ActorPartName[type];

		mEntities[type] = World::Instance()->CreateEntity(uname, mesh);

		mNode->Attach(mEntities[type]);
	}
}