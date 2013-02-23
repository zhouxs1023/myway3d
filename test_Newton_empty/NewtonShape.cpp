#include "NewtonShape.h"
#include "NewtonWorld.h"

namespace Myway {

	namespace Newton {

		tShape::tShape(tWorld * world, int type)
			: mCollision(NULL)
			, mWorld(world)
			, mType(type)
		{
		}

		tShape::~tShape()
		{
		}

		void tShape::SetUserId(int userId)
		{
			NewtonCollisionSetUserID(mCollision, userId);
		}

		int tShape::GetUserId() const
		{
			return NewtonCollisionGetUserID(mCollision);
		}









		tBox::tBox(tWorld * world, const Vec3 & size, const Mat4 & worldTm)
			: tShape(world, tShape::eBox)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision = NewtonCreateBox(ntWorld, size.x, size.y, size.z, -1, worldTm[0]);
		}





		tPlane::tPlane(tWorld * world, const Plane & plane, float width, float height, const Mat4 & worldTm)
			: tShape(world, tShape::ePlane)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			Vec3 a(-width * 0.5f, 0.0f, height * 0.5f);
			Vec3 b(width * 0.5f, 0.0f, height * 0.5f);
			Vec3 c(-width * 0.5f, 0.0f, -height * 0.5f);
			Vec3 d(width * 0.5f, 0.0f, -height * 0.5f);

			Vec3 normal = Vec3(plane.a, plane.b, plane.c);
			Vec3 trans = normal * -plane.d;
			Vec3 axis;

			float dot = Math::VecDot(Vec3::UnitY, normal);

			if (Math::Abs(dot) < (1 - DEFAULT_EPSILON))
			{
				Math::VecCross(axis, Vec3::UnitY, normal);

				float rads = Math::ACos(Math::VecDot(Vec3::UnitY, normal));

				Quat quat;
				Math::QuatFromAxis(quat, axis, rads);

				Math::QuatTranslate(a, quat, a);
				Math::QuatTranslate(b, quat, b);
				Math::QuatTranslate(c, quat, c);
				Math::QuatTranslate(d, quat, d);
			}

			a += trans;
			b += trans;
			c += trans;
			d += trans;

			mCollision = NewtonCreateTreeCollision(ntWorld, -1);

			NewtonTreeCollisionBeginBuild(mCollision);

			Vec3 tri0[3] = {a, b, c};
			Vec3 tri1[3] = {c, b, d};

			for (int i = 0; i < 3; ++i)
			{
				tri0[i] = tri0[i] * worldTm;
				tri1[i] = tri1[i] * worldTm;
			}

			NewtonTreeCollisionAddFace(mCollision, 3, (const float *)tri0, 3 * sizeof(float), 0);
			NewtonTreeCollisionAddFace(mCollision, 3, (const float *)tri1, 3 * sizeof(float), 1);

			NewtonTreeCollisionEndBuild(mCollision, 0);
		}
		




		tCone::tCone(tWorld * world, float radius, float height, const Mat4 & worldTm /* = Mat4::Identity */)
			: tShape(world, tShape::eCone)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision = NewtonCreateCone(ntWorld, radius, height, -1, worldTm[0]);
		}





		tCapsule::tCapsule(tWorld * world, float radius, float height, const Mat4 & worldTm /* = Mat4::Identity */)
			: tShape(world, tShape::eCapsule)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision = NewtonCreateCapsule(ntWorld, radius, height, -1, worldTm[0]);
		}

		

		


		tCylinder::tCylinder(tWorld * world, float radius, float height, const Mat4 & worldTm /* = Mat4::Identity */)
			: tShape(world, tShape::eCylinder)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision = NewtonCreateCylinder(ntWorld, radius, height, -1, worldTm[0]);
		}






		tEllipsoid::tEllipsoid(tWorld * world, const Vec3 & radius, const Mat4 & worldTm)
			: tShape(world, tShape::eEllipsoid)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision = NewtonCreateSphere(ntWorld, radius.x, radius.y, radius.z, -1, worldTm[0]);
		}


		tMesh::tMesh(tWorld * world, const Vec3 * verts, const int * faces, int numFaces, const Mat4 & worldTm /* = Mat4::Identity */)
			: tShape(world, tShape::eMesh)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision = NewtonCreateTreeCollision(ntWorld, -1);

			NewtonTreeCollisionBeginBuild (mCollision);

			if (worldTm != Mat4::Identity)
			{
				for (int i = 0; i < numFaces; ++i)
				{
					Vec3 tri[3];

					int a = faces[i * 3 + 0];
					int b = faces[i * 3 + 1];
					int c = faces[i * 3 + 2];

					tri[0] = verts[a] * worldTm;
					tri[1] = verts[b] * worldTm;
					tri[2] = verts[c] * worldTm;

					NewtonTreeCollisionAddFace(mCollision, 3, (const float *)tri, 3 * sizeof(float), i);
				}
			}
			else
			{
				for (int i = 0; i < numFaces; ++i)
				{
					Vec3 tri[3];

					int a = faces[i * 3 + 0];
					int b = faces[i * 3 + 1];
					int c = faces[i * 3 + 2];

					tri[0] = verts[a];
					tri[1] = verts[b];
					tri[2] = verts[c];

					NewtonTreeCollisionAddFace(mCollision, 3, (const float *)tri, 3 * sizeof(float), i);
				}
			}

			NewtonTreeCollisionEndBuild(mCollision, 1);
		}

		

		

		tConvexHull::tConvexHull(tWorld * world, const Vec3 * verts, int numVerts, const Mat4 & worldTm /* = Mat4::Identity */)
			: tShape(world, tShape::eConvexHull)
		{
			NewtonWorld * ntWorld = world->_getNewtonWorld();

			mCollision =  NewtonCreateConvexHull(ntWorld, numVerts, (const float *)verts, sizeof(Vec3), 0.1f, -1, worldTm[0]);
		}

		




		tHightField::tHightField(tWorld * world, const short * data, float width, float height)
			: tShape(world, tShape::eHightFields)
		{
		}

	}
}