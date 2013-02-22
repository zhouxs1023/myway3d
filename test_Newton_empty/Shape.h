#pragma once

#include "Newton.h"
#include "Myway.h"


namespace Myway {

	namespace Newton {

		typedef NewtonCollision ntShape;

		// 创建盒子
		ntShape * CreateBox(int shapeId, const Vec3 & vMin, const Vec3 & vMax, const Mat4 & worldTm = Mat4::Identity);

		// 创建球体
		ntShape * CreateSphere(int shapeId, float radius, const Mat4 & worldTm = Mat4::Identity);

		// 创建圆锥体
		ntShape * CreateCone(int shapeId, float radius, float height, const Mat4 & worldTm = Mat4::Identity);

		// 创建胶囊体
		ntShape * CreateCapsule(int shapeId, float radius, float height, const Mat4 & worldTm = Mat4::Identity);
		
		// 创建圆柱
		ntShape * CreateCylinder(int shapeId, float radius, float height, const Mat4 & worldTm = Mat4::Identity);

		// 创建网格
		ntShape * CreateMesh(int shapeId, const Vec3 * verts, const int * faces, int numFaces, const Mat4 & worldTm = Mat4::Identity);

		// 创建地形
		ntShape * CreateTerrain(int shapeId, float * hightFields, float width, float height);
	};
}

