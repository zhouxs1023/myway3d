#pragma once

#include "MWScene.h"

namespace Myway
{

#define MAX_OCTREE_LEVEL    8

class MW_ENTRY OctreeNode : public AllocObj
{
    friend class OctreeScene;

    struct _dirt
    {
        OctreeNode * node;
    };

    struct _node
    {
        _node * next;
        SceneNode * node;
    };

public:
    OctreeNode();
    ~OctreeNode();
    
    void SetBound(const Aabb & bound);
    const Aabb & GetBound() const;

    bool IsInside(const Aabb & bound);
    void AddNode(SceneNode * node);
    void RemoveNode(SceneNode * node);
    void RemoveAllNode();
    int GetNodeSize();

    void Clear();

    void GetVisibleSceneNodes(SceneNodeList & list, Camera * cam);
    void GetSceneNodesInBound(SceneNodeList & list, const Aabb & bound);
    void GetSceneNodesInSphere(SceneNodeList & list, const Sphere & sph);

    void RayTracing(const Ray & ray, List<SceneNode *> & nodes, int flags);
    void RayTracing(const Ray & ray, List<Mover *> & geoms, int flags);

protected:
    void _Create(OctreeScene * creator);

    void _SetChild(int index, OctreeNode * node);
    OctreeNode * _GetChild(int index);
    OctreeNode * _GetParent();

    bool _IsRoot();
    bool _IsNil();
    void _NotifyErase(OctreeNode * child);

    void _GetAllNodes(SceneNodeList & list);

protected:
    Aabb      m_bound;

    OctreeScene *       m_creator;
    OctreeNode *        m_parent;
    OctreeNode *        m_child[8];

    _node *             m_head;
    int              m_size;
};



class MW_ENTRY OctreeScene : public Scene
{
    friend class OctreeNode;

public:
    OctreeScene(const Aabb & bound);
    virtual ~OctreeScene();

    virtual void Resize(const Aabb & bound);

    virtual void UpdateSceneNode(SceneNode * node);
    virtual void AddSceneNode(SceneNode * node);
    virtual void RemoveSceneNode(SceneNode * node);
    virtual bool HasSceneNode(SceneNode * node);
    virtual void GetVisibleSceneNodes(SceneNodeList & list, Camera * cam);
    virtual void GetSceneNodesInBound(SceneNodeList & list, const Aabb & bound);
    virtual void GetSceneNodesInSphere(SceneNodeList & list, const Sphere & sph);

    virtual void RayTracing(const Ray & ray, 
                            List<SceneNode *> & nodes,
                            int flag);

    virtual void RayTracing(const Ray & ray,
                            List<Mover *> & geoms,
                            int flag);

    virtual SceneType GetType() const;

protected:
    OctreeNode *  _InsideChild(OctreeNode * node, const Aabb & bound);
    OctreeNode * _InsideNode(OctreeNode * node, int level, const Aabb & bound);

    void _GetVisibleSceneNodes(OctreeNode * ocnode, SceneNodeList & list, Camera * cam);

    void _RayTracing(OctreeNode * node, const Ray & ray, 
                     List<SceneNode *> & nodes, int mark);
    void _RayTracing(OctreeNode * node, const Ray & ray,
                     List<Mover *> & geoms,
                     int mark);

    OctreeNode * AllocOctNode();
    void FreeOctNode(OctreeNode * node);

    OctreeNode::_node * AllocScNode();
    void FreeScNode(OctreeNode::_node * node);
    

protected:
    OctreeNode *                        m_root;
    Allocator<OctreeNode, 1024>         m_OctAlloc;
    Allocator<OctreeNode::_node, 1024>  m_ScAlloc;
};


}