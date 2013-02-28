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

		class MNEWTON_ENTRY tShape
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
			tShape(int type);
			virtual ~tShape();

			void SetUserId(int userId);
			int GetUserId() const;

			int GetType() { return mType; }

			NewtonCollision * _getNewtonCollision() { return mCollision; }

		protected:
			int mType;
			NewtonCollision * mCollision;
		};



		class MNEWTON_ENTRY tBox : public tShape
		{
		public:
			tBox(const Vec3 & size, const Mat4 & worldTm = Mat4::Identity);
			~tBox() {}
		};



		class MNEWTON_ENTRY tPlane : public tShape
		{
		public:
			tPlane(const Plane & plane, float width, float height, const Mat4 & worldTm = Mat4::Identity);
			~tPlane() {}
		};



		class MNEWTON_ENTRY tCone : public tShape
		{
		public:
			tCone(float radius, float height, const Mat4 & worldTm = Mat4::Identity);
			~tCone() {}
		};



		class MNEWTON_ENTRY tCapsule : public tShape
		{
		public:
			tCapsule(float radius, float height, const Mat4 & worldTm = Mat4::Identity);
			~tCapsule() {}
		};



		class MNEWTON_ENTRY tCylinder : public tShape
		{
		public:
			tCylinder(float radius, float height, const Mat4 & worldTm = Mat4::Identity);
			~tCylinder() {}
		};



		class MNEWTON_ENTRY tEllipsoid : public tShape
		{
		public:
			tEllipsoid(const Vec3 & radius, const Mat4 & worldTm = Mat4::Identity);
			~tEllipsoid() {}
		};



		class MNEWTON_ENTRY tMesh : public tShape
		{
		public:
			tMesh(const Vec3 * verts, const int * faces, int numFaces, const Mat4 & worldTm = Mat4::Identity);
			~tMesh() {}
		};



		class MNEWTON_ENTRY tConvexHull : public tShape
		{
		public:
			tConvexHull(const Vec3 * verts, int numVerts, const Mat4 & worldTm = Mat4::Identity);
			~tConvexHull() {}
		};



		class MNEWTON_ENTRY tHightField : public tShape
		{
		public:
			tHightField(const short * data, float width, float height);
			~tHightField() {}
		};
	};
}

