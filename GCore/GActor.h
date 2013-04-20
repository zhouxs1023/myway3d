#pragma once

#include "GEntity.h"

namespace game {

	class GCORE_ENTRY GActor : public GEntity
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

		virtual int GetType() const = 0;

		void SetPart(PartType type, const char * mesh);

	protected:
		TString128 mName;
		Actor * mActor[PT_Max];
	};

	
}