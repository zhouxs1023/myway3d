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

		class tShape
		{
			DECLARE_ALLOC();

		public:
			enum eType
			{
				eUnknown,
				eBox,			// 盒子
				ePlane,			// 平面
				eEllipsoid,		// 椭球
				eCone,			// 圆锥
				eCapsule,		// 胶囊
				eCylinder,		// 圆柱
				eMesh,			// 网格
				eConvexHull,	// 凸集
				eHightFields,	// 高度场
			};

		public:
			tShape(tWorld * world, int type);
			virtual ~tShape();

			void SetUserId(int userId);
			int GetUserId() const;

			int GetType() { return mType; }

			NewtonCollision * _getNewtonCollision() { return mCollision; }
			tWorld * _getWorld() { return mWorld; }

		protected:
			tWorld * mWorld;
			int mType;
			NewtonCollision * mCollision;
		};



		class tBox : public tShape
		{
		public:
			tBox(tWorld * world, const Vec3 & size, const Mat4 & worldTm = Mat4::Identity);
			~tBox() {}
		};



		class tPlane : public tShape
		{
		public:
			tPlane(tWorld * world, const Plane & plane, float width, float height, const Mat4 & worldTm = Mat4::Identity);
			~tPlane() {}
		};



		class tCone : public tShape
		{
		public:
			tCone(tWorld * world, float radius, float height, const Mat4 & worldTm = Mat4::Identity);
			~tCone() {}
		};



		class tCapsule : public tShape
		{
		public:
			tCapsule(tWorld * world, float radius, float height, const Mat4 & worldTm = Mat4::Identity);
			~tCapsule() {}
		};



		class tCylinder : public tShape
		{
		public:
			tCylinder(tWorld * world, float radius, float height, const Mat4 & worldTm = Mat4::Identity);
			~tCylinder() {}
		};



		class tEllipsoid : public tShape
		{
		public:
			tEllipsoid(tWorld * world, const Vec3 & radius, const Mat4 & worldTm = Mat4::Identity);
			~tEllipsoid() {}
		};



		class tMesh : public tShape
		{
		public:
			tMesh(tWorld * world, const Vec3 * verts, const int * faces, int numFaces, const Mat4 & worldTm = Mat4::Identity);
			~tMesh() {}
		};



		class tConvexHull : public tShape
		{
		public:
			tConvexHull(tWorld * world, const Vec3 * verts, int numVerts, const Mat4 & worldTm = Mat4::Identity);
			~tConvexHull() {}
		};



		class tHightField : public tShape
		{
		public:
			tHightField(tWorld * world, const short * data, float width, float height);
			~tHightField() {}
		};
	};
}

