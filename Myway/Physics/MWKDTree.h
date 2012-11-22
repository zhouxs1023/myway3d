#pragma once

#include "MWMath.h"

namespace Myway {

	template <class T>
	class KDTree
	{
		DECLARE_ALLOC();

	public:
		enum Axis
		{
			nAxis = 0,
			xAxis,
			yAxis,
			zAxis
		};

		struct Node
		{
			Aabb aabb;
			Axis axis;
			List<T> elems;

			Node * child[2];

			Node()
			{
				aabb = Aabb::Zero;
				axis = nAxis;
				child[0] = NULL;
				child[1] = NULL;
			}
		};

	public:
		KDTree()
		{
		}

		~KDTree()
		{
			mAlloc.Shutdown();
			mRoot = NULL;
		}

		void Build(const Aabb & aabb, int depth)
		{
			mAlloc.Shutdown();
			mRoot = NULL;
			mAlloc = aabb;

			mRoot = mAlloc.Alloc();

			mRoot->aabb = aabb;
			mRoot->axis = nAxis;
			mRoot->child[0] = NULL;
			mRoot->child[1] = NULL;

			_build(mRoot, depth);
		}

		Node * GetRoot()
		{
			return mRoot;
		}

	protected:
		void _init(Node * node)
		{
			node->aabb = Aabb::Zero;
			node->axis = nAxis;

			node->child[0] = NULL;
			node->child[1] = NULL;

			node->elems.Clear();
		}

		void _build(Node * parent, int depth)
		{
			if (depth < 1)
				return ;

			const Aabb & aabb = parent->aabb;
			Vec3 size = aabb->GetSize();
			Vec3 split = Vec3::Unit;

			if (size.x >= size.y && size.x >= size.z)
			{
				parent->axis = xAxis;
				split.x = 0.5f;
			}
			else if (size.y >= size.x && size.y >= size.z)
			{
				parent->axis = yAxis;
				split.y = 0.5f;
			}
			else if (size.z >= size.x && size.z >= size.y)
			{
				parent->axis = zAxis;
				split.z = 0.5f;
			}

			d_assert (parent->axis != nAxis);

			parent->child[0] = mAlloc.Alloc();
			parent->child[1] = mAlloc.Alloc();

			_init(parent->child[0]);
			_init(parent->child[1]);

			parent->child[0]->aabb = Aabb(aabb.minimum, aabb.minimum + size * Vec3(0.5f, 1, 1));
			parent->child[1]->aabb = Aabb(aabb.minimum + size * Vec3(0.5f, 1, 1), aabb.maximum);

			_build(parent->child[0], depth - 1);
			_build(parent->child[1], depth - 1);
		}

	protected:
		Aabb mAabb;
		Node * mRoot;

		Allocator<Node, 64> mAlloc;
	};

}