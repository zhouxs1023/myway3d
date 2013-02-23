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

		public:
			tBody(tWorld * world, tShape * shape, int type);
			virtual ~tBody();

			void SetBodyMatrix(const Mat4 & worldTm);

			int GetType() { return mType; }

			SceneNode * GetSceneNode() { return mNode; }
			
		protected:
			SceneNode * mNode;
			tWorld * mWorld;
			NewtonBody * mBody;
			tShape * mShape;
			int mType;
		};






		class tRigidBody : public tBody
		{
		public:
			tRigidBody(tWorld * world, tShape * shape, SceneNode * node, float mass = 0);
			~tRigidBody() {}
		};

	}
}