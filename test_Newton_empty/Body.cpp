#include "Body.h"
#include "World.h"

#define GRAVITY	-1000.0f

namespace Myway {

	namespace Newton {

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

			Node * node = (Node*) NewtonBodyGetUserData(body);

			node->SetPosition(posit);
			node->SetOrientation(rotation);
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


		ntBody * CreateRigidBody(SceneNode * node, ntShape * shape, float mass)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			Vec4 origin, inertia;
			NewtonBody* body;

			Mat4 worldTm;
			worldTm.MakeTransform(node->GetWorldPosition(), node->GetWorldOrientation(), Vec3::Unit);

			// Now with the collision Shape we can crate a rigid body
			body = NewtonCreateBody (world, shape, &worldTm[0][0]);

			// bodies can have a destructor. 
			NewtonBodySetDestructorCallback (body, DestroyBodyCallback);

			// save the entity as the user data for this body
			NewtonBodySetUserData(body, node);

			// we need to set the proper center of mass and inertia matrix for this body
			// the inertia matrix calculated by this function does not include the mass.
			// therefore it needs to be multiplied by the mass of the body before it is used.
			NewtonConvexCollisionCalculateInertialMatrix(shape, &inertia[0], &origin[0]);	

			// set the body mass matrix
			NewtonBodySetMassMatrix(body, mass, mass * inertia.x, mass * inertia.y, mass * inertia.z);

			// set the body origin
			NewtonBodySetCentreOfMass(body, &origin[0]);

			// set the function callback to apply the external forces and torque to the body
			// the most common force is Gravity
			NewtonBodySetForceAndTorqueCallback(body, ApplyForceAndTorqueCallback);

			// set the function callback to set the transformation state of the graphic entity associated with this body 
			// each time the body change position and orientation in the physics world
			NewtonBodySetTransformCallback(body, SetTransformCallback);

			return body;
		}

		void DestroyRigidBody(ntBody * body)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			NewtonDestroyBody(world, body);
		}
	}
}