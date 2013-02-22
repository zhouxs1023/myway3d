#include "Shape.h"
#include "World.h"

namespace Myway {

	namespace Newton {

		static dFloat UserMeshCollisionCallback (const NewtonBody* const body, const NewtonCollision* const collisionTree, dFloat interception, dFloat* normal, int faceId, void* usedData)
		{
			return 1.0f;
		}


		ntShape * CreateBox(int shapeId, const Vec3 & vMin, const Vec3 & vMax, const Mat4 & worldTm)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			Vec3 size = vMax - vMin;

			return NewtonCreateBox(world, size.x, size.y, size.z, shapeId, (const float *)&worldTm);
		}

		ntShape * CreateSphere(int shapeId, float radius, const Mat4 & worldTm)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			return NewtonCreateSphere(world, radius, radius, radius, shapeId, (const float *)&worldTm);
		}

		ntShape * CreateCone(int shapeId, float radius, float height, const Mat4 & worldTm)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			return NewtonCreateCone(world, radius, height, shapeId, (const float *)&worldTm);
		}

		ntShape * CreateCapsule(int shapeId, float radius, float height, const Mat4 & worldTm)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			return NewtonCreateCapsule(world, radius, height, shapeId, (const float *)&worldTm);
		}

		ntShape * CreateCylinder(int shapeId, float radius, float height, const Mat4 & worldTm)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			return NewtonCreateCylinder(world, radius, height, shapeId, (const float *)&worldTm);
		}

		ntShape * CreateMesh(int shapeId, const Vec3 * verts, const int * faces, int numFaces, const Mat4 & worldTm)
		{
			NewtonWorld * world = World::Instance()->_getNewtonWorld();

			ntShape * shape = NewtonCreateTreeCollision(world, shapeId);

			NewtonTreeCollisionBeginBuild (shape);

			for (int i = 0; i < numFaces; ++i)
			{
				Vec3 tri[3];

				int a = faces[i * 3 + 0];
				int b = faces[i * 3 + 1];
				int c = faces[i * 3 + 2];

				tri[0] = verts[a];
				tri[1] = verts[b];
				tri[2] = verts[c];

				NewtonTreeCollisionAddFace(shape, 3, (const float *)tri, 3 * sizeof(Vec3), i);
			}

			NewtonTreeCollisionEndBuild(shape, 1);

			NewtonTreeCollisionSetUserRayCastCallback(shape, UserMeshCollisionCallback);

			return shape;
		}

		ntShape * CreateTerrain(int shapeId, float * hightFields, float width, float height)
		{
			//NewtonCreateHeightFieldCollision()

			return NULL;
		}
	}
}