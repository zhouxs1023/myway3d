#pragma once

#include "NewtonEntry.h"

namespace Myway {

	namespace Newton {

		class MNEWTON_ENTRY tMaterialId
		{
			DECLARE_ALLOC();

		public:
			tMaterialId();
			~tMaterialId();

			void Query();

			int GetId() const { return mId; }

			bool operator == (const tMaterialId & rk) const
			{
				return mId == rk.GetId();
			}

		protected:
			int mId;
		};


		class MNEWTON_ENTRY tContact
		{
			DECLARE_ALLOC();

			static int  _newtonAABBOverlap( const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex);
			static void _newtonContactProcess(const NewtonJoint * contact, dFloat timeStep, int threadIndex);

		public:
			Event OnAABBOverlap;
			Event OnContactProcess;

		public:
			tContact();
			~tContact();

			void Set(const tMaterialId * id0, const tMaterialId * id1);
			void Reset();

			void SetSoftness(float softness);
			void SetElasticity(float elasticity);
			void SetFriction(float static_, float kinetic_);

			void SetCollidable(int state);

			void SetContinuousCollisionMode(int state);

		protected:
			const tMaterialId * mId0;
			const tMaterialId * mId1;
		};

	}

}