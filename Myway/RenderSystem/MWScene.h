#pragma once

#include "MWSceneNode.h"
#include "MWCamera.h"

namespace Myway
{

enum SceneType
{
    ST_UNIFORM,
    ST_QUADTREE,
    ST_OCTREE,
};


class MW_ENTRY Scene : public AllocObj
{
public:
	struct TraceInfo
	{
		struct SortOp
		{
			bool operator ()(const Scene::TraceInfo & a, const Scene::TraceInfo & b) const
			{
				return a.dist < b.dist;
			};
		};

		SceneNode * node;
		float dist;
	};

public:
    Scene(const Aabb & bound) : m_abBound(bound){}
    virtual ~Scene() {}

    virtual void Resize(const Aabb & bound) = 0;

    virtual void UpdateSceneNode(SceneNode * node) = 0;
    virtual void AddSceneNode(SceneNode * node) = 0;
    virtual void RemoveSceneNode(SceneNode * node) = 0;
    virtual bool HasSceneNode(SceneNode * node) = 0;
    virtual void GetVisibleSceneNodes(SceneNodeList & list, Camera * cam) = 0;
    virtual void GetSceneNodesInBound(SceneNodeList & list, const Aabb & bound) = 0;
    virtual void GetSceneNodesInSphere(SceneNodeList & list, const Sphere & sph) = 0;
    virtual void RayTracing(const Ray & ray,  float dist,
                            Array<TraceInfo> & nodes,
                            int flags) = 0;

    virtual SceneType GetType() const = 0;

    const Aabb & GetBound() const { return m_abBound; }

protected:
    //scene bound. all nodes must be in this bound.
    Aabb            m_abBound;
};

}