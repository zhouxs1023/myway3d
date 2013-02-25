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

#include "Newton.h"
#include "Myway.h"

namespace Myway {

	namespace Newton {

		class tWorld;
		class tShape;

		class tBody
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
			Vec3 GetCenterOfMass() const;

			NewtonBody * _getNewtonBody() const { return mBody; }
			
		protected:
			tShape * mShape;
			int mType;
			NewtonBody * mBody;

			SceneNode * mNode;
		};






		class tRigidBody : public tBody
		{
		public:
			tRigidBody(tShape * shape, SceneNode * node, float mass = 0);
			~tRigidBody() {}
		};

	}
}