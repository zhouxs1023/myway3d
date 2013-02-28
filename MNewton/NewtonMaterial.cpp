#include "NewtonMaterial.h"
#include "NewtonWorld.h"

namespace Myway {

	namespace Newton {

		tMaterialId::tMaterialId()
			: mId(-1)
		{
		}

		tMaterialId::~tMaterialId()
		{
		}

		void tMaterialId::Query()
		{
			d_assert (mId == -1);
			mId = NewtonMaterialCreateGroupID(tWorld::Instance()->_getNewtonWorld());
		}









		int tContact::_newtonAABBOverlap( const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex)
		{
			return 1;
		}

		void tContact::_newtonContactProcess(const NewtonJoint * contactJoint, dFloat timeStep, int threadIndex)
		{
			Vec3 contactPosit, contactNormal;

			float contactBestSpeed = 0.5f;

			tContact * mtl = NULL;
			const NewtonBody * body0 = NewtonJointGetBody0(contactJoint);

			void * contact = NewtonContactJointGetFirstContact (contactJoint);

			while (contact)
			{
				float contactNormalSpeed;
				NewtonMaterial * material;

				// get the material for this contact;
				material = NewtonContactGetMaterial (contact);

				contactNormalSpeed = NewtonMaterialGetContactNormalSpeed (material);
				if (contactNormalSpeed > contactBestSpeed){
					contactBestSpeed = contactNormalSpeed;
					contactBestSpeed = contactNormalSpeed;
					NewtonMaterialGetContactPositionAndNormal(material, &contactPosit.x, &contactNormal[0]);
					mtl = (tContact *)NewtonMaterialGetMaterialPairUserData(material);
				}

				contact = NewtonContactJointGetNextContact (contactJoint, contact);
			}
		
			if (mtl)
				mtl->OnContactProcess(&contactPosit, &contactNormal);
		}

		tContact::tContact()
			: mId0(NULL)
			, mId1(NULL)
		{
		}

		tContact::~tContact()
		{
		}

		void tContact::Set(const tMaterialId * id0, const tMaterialId * id1)
		{
			d_assert(mId0 == NULL && mId1 == NULL);

			mId0 = id0;
			mId1 = id1;

			NewtonMaterialSetCollisionCallback(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(),
				this, tContact::_newtonAABBOverlap, tContact::_newtonContactProcess);
		}

		void tContact::Reset()
		{
			d_assert (mId0 && mId1)
			NewtonMaterialSetCollisionCallback(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(), NULL, NULL, NULL);
		}

		void tContact::SetSoftness(float softness)
		{
			NewtonMaterialSetDefaultSoftness(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(), softness);
		}

		void tContact::SetElasticity(float elasticity)
		{
			NewtonMaterialSetDefaultElasticity(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(), elasticity);
		}

		void tContact::SetFriction(float static_, float kinetic_)
		{
			NewtonMaterialSetDefaultFriction(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(), static_, kinetic_);
		}

		void tContact::SetCollidable(int state)
		{
			NewtonMaterialSetDefaultCollidable(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(), state);
		}

		void tContact::SetContinuousCollisionMode(int state)
		{
			NewtonMaterialSetContinuousCollisionMode(tWorld::Instance()->_getNewtonWorld(), mId0->GetId(), mId1->GetId(), state);
		}
	}

}