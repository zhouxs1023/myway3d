#include "NewtonWorld.h"

namespace Myway {

	namespace Newton {

		tWorld::tWorld()
			: mNtWorld(NULL)
		{
			mSimulateTime = 1 / 30.0f;
		}

		tWorld::~tWorld()
		{
		}

		void tWorld::Init()
		{
			NewtonSetMemorySystem(NULL, NULL);

			mNtWorld = NewtonCreate();

			// use the standard x87 floating point model  
			NewtonSetPlatformArchitecture (mNtWorld, 0);

			NewtonSetSolverModel (mNtWorld, 1);

			mInteralTime = Engine::Instance()->GetTime();

			SetWorldSize(Vec3(-10000.0f, -10000.0f, -10000.0f), Vec3(10000.0f, 10000.0f, 10000.0f));
		}

		void tWorld::Shutdown()
		{
			NewtonDestroyAllBodies(mNtWorld);
			NewtonDestroy(mNtWorld);
		}

		void tWorld::SetWorldSize(const Vec3 & vMin, const Vec3 & vMax)
		{
			NewtonSetWorldSize (mNtWorld, &vMin[0], &vMax[0]); 
		}

		void tWorld::Update()
		{
			float time = Engine::Instance()->GetTime();

			while (time - mInteralTime > mSimulateTime)
			{
				NewtonUpdate(mNtWorld, mSimulateTime * 1000);

				mInteralTime += mSimulateTime;
			}
		}

	}

}