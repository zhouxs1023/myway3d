#include "NewtonWorld.h"

#define _PHY_DEFAULT_FPS 60.0f
#define _PHY_DEFAULT_GRAVITY -1000.0f

namespace Myway {

	namespace Newton {

		IMP_SLN(tWorld);

		tWorld::tWorld()
			: mNtWorld(NULL)
		{
			mSimulateTime = 1 / _PHY_DEFAULT_FPS;
			mGravity = _PHY_DEFAULT_GRAVITY;

			INIT_SLN;
		}

		tWorld::~tWorld()
		{
			SHUT_SLN;
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