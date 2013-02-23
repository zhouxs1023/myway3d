#include "MOpcodeColObj.h"
#include "MOpcodeMath.h"
#include "MOpcodeScene.h"

namespace Myway {

	MOpcodeColObj::MOpcodeColObj(void * uid, float scale)
		: IColObj(uid, scale) 
		, mCollisionModel(NULL)
	{
	}

	MOpcodeColObj::~MOpcodeColObj()
	{
	}

	bool MOpcodeColObj::RayCheck(HitInfoSet & info, const Ray & ray, const Mat4 & worldTm, unsigned int * cache)
	{
		if (!mCollisionModel)
			return false;

		Opcode::CollisionFaces * queryResult = MOpcodeScene::_getCollisionFaces();

		Opcode::RayCollider collider;

		IceMaths::Ray _ray;
		_ray.mDir.x = ray.direction.x;
		_ray.mDir.y = ray.direction.y;
		_ray.mDir.z = ray.direction.z;

		_ray.mOrig.x = ray.origin.x;
		_ray.mOrig.y = ray.origin.y;
		_ray.mOrig.z = ray.origin.z;

		collider.SetClosestHit(true);
		collider.SetCulling(true);
		collider.SetDestination(queryResult);

		if (collider.Collide(_ray, *mCollisionModel, (const IceMaths::Matrix4x4 *)&worldTm, cache))
		{
			int bnFaces = queryResult->GetNbFaces();
			const Opcode::CollisionFace * colFaces = queryResult->GetFaces();

			if( bnFaces == 0 )
				return false;

			int colIndex = 0;
			for (int i = 1; i < bnFaces; ++i)
			{
				if (colFaces[i].mDistance < colFaces[colIndex].mDistance)
					colIndex = i;
			}

			info.r_distance = colFaces[colIndex].mDistance;
			info.r_that_faceId = colFaces[colIndex].mFaceID;

			Vec3 v0, v1, v2;
			_getTriVertex(info.r_that_faceId, v0, v1, v2);
			v0 *= worldTm;
			v1 *= worldTm;
			v2 *= worldTm;

			info.r_that_normal = (v1 - v0).CrossN(v2 - v1);
			
			float u = colFaces[colIndex].mU;
			float v = colFaces[colIndex].mV;
			float w = 1.0f - u - v;

			info.r_that_position = v0 * w + v1 * u + v2 * v;

			info.r_that_faceId = colFaces[colIndex].mFaceID;

			return true;
		}

		return false;
	}

	//bool MOpcodeColObj::SweptSphereCheck(PhyHitInfo & info, const SweptSphere & sph, const mat4 & worldTm)
	//{
	//	Opcode::LSSCollider & collider =  MOpcodeScene::Instance()->opcSweptSphereCollider;
	//	Opcode::LSSCache& cache = CollisionManager::getSingletonPtr()->opcSweptSphereCache;

	//	collider.SetFirstContact(false);

	//	IceMaths::Matrix4x4 opcMatrix = MOpcodeUtil::ToIceMat4(worldTm);

	//	d_assert( collider.ValidateSettings() == 0 );

	//	IceMaths::Point startPoint = MOpcodeUtil::ToIcePoint(sph.position);
	//	IceMaths::Point endPoint = MOpcodeUtil::ToIcePoint(sph.position + sph.movement);

	//	IceMaths::Segment opcSegment(startPoint, endPoint);
	//	IceMaths::LSS opcLSS(opcSegment, radius);

	//	// perform collision
	//	if( !collider.Collide(cache, opcLSS, opcModel, NULL, (const IceMaths::Matrix4x4 *)&worldTm) )
	//		return false;

	//	info.Distance = sph.movement.Length();

	//	CollisionPair cp;

	//	if (collider.GetContactStatus())
	//	{
	//		const udword* collFaces = collider.GetTouchedPrimitives();
	//		int numFaces = collider.GetNbTouchedPrimitives();

	//		if (numFaces > 0)
	//		{
	//			for(int i = 0; i < numFaces; i++)
	//			{
	//				// build triangle from from faceIndex
	//				Vector3 v0,v1,v2;
	//				_getTriVertex(collFaces[i], v0, v1, v2);

	//				v0 = v0 * worldTm;		// transform to world space
	//				v1 = v1 * worldTm;
	//				v2 = v2 * worldTm;

	//				_SweptSphereIntersection(&cp, sph, v0, v1, v2);
	//			}

	//			return true;
	//		}
	//	}

	//	return false;
	//}

	//bool MOpcodeColObj::_SweptSphereIntersection(CollisionPair * cp, const SweptSphere & sph,
	//											 const Vec3 & v0, const Vec3 & v1, const Vec3 & v2)
	//{
	//	if(sph.movement == Vec3::Zero)
	//		return false;

	//	float scale_fact = 1.0 / radius;
	//	v0 *= scale_fact;
	//	v1 *= scale_fact;
	//	v2 *= scale_fact;

	//	Vec3 position = sph.position * scale_fact;
	//	Vec3 movementVector = sph.movement * scale_fact;

	//	triangle triangle(v0, v1, v2);

	//	OP_Math
	//	Plane trianglePlane = Plane(v0, v1, v2)

	//	// only check front facing polygons
	//	if (trianglePlane.AtSide(position) != Plane::POSITIVE)
	//		return false;

	//	// get interval of plane intersection
	//	float t1, t0;
	//	bool embeddedInPlane = false;

	//	// calculate signed distance from sphere position to triangle plane
	//	float signedDistToTrianglePlane = trianglePlane.Distance(position);

	//	float normalDotVelocity = trianglePlane.normal.Dot(movementVector);

	//	if (normalDotVelocity == 0.0f)
	//	{
	//		// sphere is traveling paralell to plane
	//		if (fabs(signedDistToTrianglePlane) >= 1.0f)
	//			return false; // no collision possible
	//		else
	//		{
	//			// sphere is embedded in plane
	//			embeddedInPlane = true;
	//			t0 = 0.0;
	//			t1 = 1.0;
	//		}
	//	}
	//	else
	//	{
	//		// N.D is not 0. Calculate intersection interval
	//		t0 = (-1.0-signedDistToTrianglePlane)/normalDotVelocity;
	//		t1 = (1.0-signedDistToTrianglePlane)/normalDotVelocity;

	//		// Swap so t0 < t1
	//		if (t0 > t1) { float tmp = t1; t1 = t0; t0 = tmp;	}

	//		// check if at least one value is within the range
	//		if (t0 > 1.0f || t1 < 0.0f)
	//			return false; // both t values are outside 1 and 0, no collision possible

	//		// clamp to 0 and 1
	//		if (t0 < 0.0) t0 = 0.0;
	//		if (t1 < 0.0) t1 = 0.0;
	//		if (t0 > 1.0) t0 = 1.0;
	//		if (t1 > 1.0) t1 = 1.0;
	//	}

	//	// at this point we have t0 and t1, if there is any intersection, it
	//	// is between this interval

	//	Vec3 collisionPoint = Vec3::Zero;
	//	bool foundCollision = false;
	//	float t = 1.0f;

	//	// first check the easy case: Collision within the triangle;
	//	// if this happens, it must be at t0 and this is when the sphere
	//	// rests on the front side of the triangle plane. This can only happen
	//	// if the sphere is not embedded in the triangle plane.
	//	if (!embeddedInPlane)
	//	{
	//		Vec3 planeIntersectionPoint =
	//			(position - trianglePlane.normal)
	//			+ (movementVector * t0);


	//		if (triangle.isPointInsideFast(planeIntersectionPoint))
	//		{
	//			foundCollision = true;
	//			t = t0;
	//			collisionPoint = planeIntersectionPoint;
	//		}
	//	}

	//	// if we havent found a collision already we will have to sweep
	//	// the sphere against points and edges of the triangle. Note: A
	//	// collision inside the triangle will always happen before a
	//	// vertex or edge collision.

	//	// DAVE: is checking against points really necessary if we are checking against edges?
	//	// Shouldn't the edges take care of that?

	//	if (!foundCollision)
	//	{
	//		Ogre::Vector3 velocity = movementVector;
	//		Ogre::Vector3 base = position;

	//		Ogre::Real velocitySqaredLength = (Ogre::Real)velocity.squaredLength();
	//		Ogre::Real a,b,c;
	//		Ogre::Real newT;

	//		// for each edge or vertex a quadratic equation has to be solved:
	//		// a*t^2 + b*t + c = 0. We calculate a,b, and c for each test.

	//		// check against points
	//		a = velocitySqaredLength;

	//		// FIXME: turn these 3 into 1 function
	//		// p1
	//		b = 2.0f * (velocity.dotProduct(base - triangle.point(0)));
	//		c = (Ogre::Real)((triangle.point(0)-base).squaredLength() - 1.0);
	//		if (getLowestRoot(a,b,c,t, &newT))
	//		{
	//			t = newT;
	//			foundCollision = true;
	//			collisionPoint = triangle.point(0);
	//		}

	//		// p2
	//		if (!foundCollision)
	//		{
	//			b = 2.0f * (velocity.dotProduct(base - triangle.point(1)));
	//			c = (Ogre::Real)((triangle.point(1)-base).squaredLength() - 1.0);
	//			if (getLowestRoot(a,b,c,t, &newT))
	//			{
	//				t = newT;
	//				foundCollision = true;
	//				collisionPoint = triangle.point(1);
	//			}
	//		}

	//		// p3
	//		if (!foundCollision)
	//		{
	//			b = 2.0f * (velocity.dotProduct(base - triangle.point(2)));
	//			c = (Ogre::Real)((triangle.point(2)-base).squaredLength() - 1.0);
	//			if (getLowestRoot(a,b,c,t, &newT))
	//			{
	//				t = newT;
	//				foundCollision = true;
	//				collisionPoint = triangle.point(2);
	//			}
	//		}

	//		// check against edges:

	//		Ogre::Vector3 edge;
	//		Ogre::Vector3 baseToVertex;

	//		edge = triangle.point(1) - triangle.point(0);
	//		baseToVertex = triangle.point(0) - position;

	//		Vector3 point = triangle.point(0);
	//		sphereEdgeCheck(velocity, edge, baseToVertex, t, foundCollision, collisionPoint, point);

	//		edge = triangle.point(2) - triangle.point(1);
	//		baseToVertex = triangle.point(1) - position;

	//		point = triangle.point(1);
	//		sphereEdgeCheck(velocity, edge, baseToVertex, t, foundCollision, collisionPoint, point);

	//		edge = triangle.point(0) - triangle.point(2);
	//		baseToVertex = triangle.point(2) - position;

	//		point = triangle.point(2);
	//		sphereEdgeCheck(velocity, edge, baseToVertex, t, foundCollision, collisionPoint, point);

	//	}// end no collision found


	//	// set result:
	//	if (foundCollision)
	//	{
	//		// Time to scale everything back..

	//		// distance to collision is t
	//		Ogre::Real distToCollision = (Ogre::Real)(t*movementVector.length() * radius);

	//		// does this triangle qualify for closest hit?
	//		if (distToCollision	<= cp->distance)
	//		{
	//			cp->distance = distToCollision;
	//			cp->contact = collisionPoint * radius;
	//			cp->other_contact = cp->contact;
	//			cp->this_contact = (position + (movementVector*t))*radius;

	//			cp->other_normal = trianglePlane.normal;
	//			Ogre::Vector3 vec_to_col = cp->contact - cp->this_contact;
	//			cp->this_normal = vec_to_col.normalisedCopy();
	//		}

	//		return true;
	//	}// end found collision
	//}
}