#include "NewtonBody.h"
#include "NewtonWorld.h"
#include "NewtonShape.h"

#define GRAVITY	-1000.0f

namespace Myway {

	namespace Newton {

		tBody::tBody(tWorld * world, tShape * shape, int type)
			: mWorld(world)
			, mShape(shape)
			, mType(type)
		{
		}

		tBody::~tBody()
		{
			NewtonDestroyBody(mWorld->_getNewtonWorld(), mBody);
		}

		void tBody::SetBodyMatrix(const Mat4 & worldTm)
		{
			NewtonBodySetMatrix(mBody, &worldTm[0][0]);
		}










		void DestroyBodyCallback (const NewtonBody* body)
		{
			// nothing
		}

		// Transform callback to set the matrix of a the visual entity
		void SetTransformCallback (const NewtonBody* body, const dFloat* matrix, int threadIndex)
		{
			Vec3 posit (matrix[12], matrix[13], matrix[14]);
			Quat rotation;

			// we will ignore the Rotation part of matrix and use the quaternion rotation stored in the body
			NewtonBodyGetRotation(body, (float*)&rotation);

			tBody * me = (tBody *)NewtonBodyGetUserData(body);

			SceneNode * node = me->GetSceneNode();

			if (node)
			{
				node->SetPosition(posit);
				node->SetOrientation(rotation);
			}

			Mat4 worldTm = *(Mat4 *)matrix;

			me->OnTransform(me, &worldTm);
		}


		// callback to apply external forces to body
		void ApplyForceAndTorqueCallback (const NewtonBody* body, dFloat timestep, int threadIndex)
		{
			dFloat Ixx;
			dFloat Iyy;
			dFloat Izz;
			dFloat mass;

			// for this tutorial the only external force in the Gravity
			NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);

			Vec4 gravityForce(0.0f, mass * GRAVITY, 0.0f, 1);

			NewtonBodySetForce(body, &gravityForce[0]);
		}







		tRigidBody::tRigidBody(tWorld * world, tShape * shape, SceneNode * node, float mass)
			: tBody(world, shape, tBody::eRigidBody)
		{
			mNode = node;
			Vec4 origin, inertia;

			Mat4 worldTm = Mat4::Identity;

			if (mNode)
			{
				worldTm.MakeTransform(node->GetWorldPosition(), node->GetWorldOrientation(), Vec3::Unit);
			}

			mBody = NewtonCreateBody(world->_getNewtonWorld(), shape->_getNewtonCollision(), worldTm[0]);

			NewtonBodySetDestructorCallback (mBody, DestroyBodyCallback);
			NewtonBodySetUserData(mBody, this);

			// we need to set the proper center of mass and inertia matrix for this body
			// the inertia matrix calculated by this function does not include the mass.
			// therefore it needs to be multiplied by the mass of the body before it is used.
			NewtonConvexCollisionCalculateInertialMatrix(shape->_getNewtonCollision(), &inertia[0], &origin[0]);	

			// set the body mass matrix
			NewtonBodySetMassMatrix(mBody, mass, mass * inertia.x, mass * inertia.y, mass * inertia.z);

			// set the body origin
			NewtonBodySetCentreOfMass(mBody, &origin[0]);

			// set the function callback to apply the external forces and torque to the body
			// the most common force is Gravity
			NewtonBodySetForceAndTorqueCallback(mBody, ApplyForceAndTorqueCallback);

			// set the function callback to set the transformation state of the graphic entity associated with this body 
			// each time the body change position and orientation in the physics world
			NewtonBodySetTransformCallback(mBody, SetTransformCallback);
		}
	}
}