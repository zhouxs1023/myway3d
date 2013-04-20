#pragma once

#include "MWEntity.h"

namespace Myway {

	class MW_ENTRY Actor : public Entity
	{
		DECLARE_ALLOC();

	public:
		Actor(const TString128 & name);
		virtual ~Actor();

		virtual void UpdateGeometry();

		void LoadAnimation(const char * name, const char * source);
		void PlayAnimation(const char * name);


	protected:
		virtual void _Init();
		virtual void _DeInit();

	protected:
		AnimationSet * mAnimationSet;
	};

}