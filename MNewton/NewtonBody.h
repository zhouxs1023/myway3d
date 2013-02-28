//////////////////////////////////////////////////////////////////////////
//
// Newton Integration.
//   newton physics engine: http://newtondynamics.com/forum/newton.php
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "NewtonEntry.h"

namespace Myway {

	namespace Newton {

		class tWorld;
		class tShape;
		class tMaterialId;

		class MNEWTON_ENTRY tBody
		{
		public:
			enum eType
			{
				eUnknown,
				eRigidBody
			};

			Event OnTransform;
			Event OnApplyForce;
			Event OnDestroy;

		public:
			tBody(tShape * shape, int type);
			virtual ~tBody();

			void SetBodyMatrix(const Mat4 & worldTm);

			int GetType() const { return mType; }

			tShape * GetShape() const { return mShape; }

			SceneNode * GetSceneNode() const { return mNode; }

			Vec4 GetMassMatrix() const;

			void SetCenterOfMass(Vec3 & center);
			Vec3 GetCenterOfMass() const;

			NewtonBody * _getNewtonBody() const { return mBody; }

			void SetMaterialId(const tMaterialId * matId);

			void SetVelocity(const Vec3 & v);
			Vec3 GetVelocity() const;

		protected:
			tShape * mShape;
			int mType;
			NewtonBody * mBody;

			SceneNode * mNode;
		};






		class MNEWTON_ENTRY tRigidBody : public tBody
		{
		public:
			tRigidBody(tShape * shape, SceneNode * node, float mass = 0);
			~tRigidBody() {}
		};

	}
}