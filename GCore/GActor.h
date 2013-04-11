#pragma once

#include "GEntity.h"

namespace game {

	class GActor : public GEntity
	{
		DECLARE_ALLOC();

	public:
		enum PartType
		{
			Main,

			Weapon,
			Helmet,
			Shoulder,
			Clothes,
			Shoes,

			PT_Max,
		};

		struct ActorPart
		{
			int id;
			char mesh[32];
		};

	public:
		GActor(const char * name);
		virtual ~GActor();

		void SetPart(PartType type, const char * mesh);

	protected:
		TString128 mName;
		Entity * mEntities[PT_Max];
	};

	
}