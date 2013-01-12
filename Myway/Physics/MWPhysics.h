#pragma once

#include "MWMath.h"
#include "MWEntity.h"

namespace Myway {

	class MW_ENTRY PhysicsSystem
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON(PhysicsSystem);

	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();

		virtual void OnStaticObjectCreate(Entity * entity) = 0;
		virtual void OnStaticObjectDestroy(Entity * entity) = 0;
		virtual void OnStaticObjectTransform(Entity * entity) = 0;

		virtual void OnDynamicObjectCreate(Entity * entity) = 0;
		virtual void OnDynamicObjectDestroy(Entity * entity) = 0;
		virtual void OnDynamicObjectTransform(Entity * entity) = 0;
	};
}