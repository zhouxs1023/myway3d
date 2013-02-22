#include "World.h"

namespace Myway {

	namespace Newton {

		IMP_SLN(World);

		World::World()
			: mNtWorld(NULL)
		{
			INIT_SLN;
			mSimulateTime = 1 / 30.0f;
		}

		World::~World()
		{
			SHUT_SLN;
		}

		void World::Init()
		{
			NewtonSetMemorySystem(NULL, NULL);

			mNtWorld = NewtonCreate();

			// use the standard x87 floating point model  
			NewtonSetPlatformArchitecture (mNtWorld, 0);

			// set a fix world size
			Vec3 minSize (-1000.0f, -1000.0f, -1000.0f);
			Vec3 maxSize ( 1000.0f,  1000.0f,  1000.0f);

			NewtonSetWorldSize (mNtWorld, &minSize[0], &maxSize[0]); 

			NewtonSetSolverModel (mNtWorld, 1);

			mInteralTime = Engine::Instance()->GetTime();
		}

		void World::Shutdown()
		{
			NewtonDestroyAllBodies(mNtWorld);
			NewtonDestroy(mNtWorld);
		}

		void World::Update()
		{
			float time = Engine::Instance()->GetTime();

			while (time - mInteralTime > mSimulateTime)
			{
				NewtonUpdate(mNtWorld, mSimulateTime);

				mInteralTime += mSimulateTime;
			}
		}

	}

}