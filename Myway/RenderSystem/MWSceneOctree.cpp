#include "MWSceneOctree.h"

using namespace Myway;

OctreeNode::OctreeNode()
: m_bound(Aabb::Zero),
  m_creator(NULL),
  m_parent(NULL),
  m_head(NULL),
  m_size(0)
{
    ZeroMemory(m_child, sizeof(OctreeNode *) * 8);
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::SetBound(const Aabb & bound)
{
    m_bound = bound;
}

void OctreeNode::_Create(OctreeScene * creator)
{
    m_creator = creator;
}

void OctreeNode::_SetChild(int index, OctreeNode * node)
{
    d_assert (index < 8 && !m_child[index]);
    m_child[index] = node;
    node->m_parent = this;
}

OctreeNode * OctreeNode::_GetChild(int index)
{
    d_assert (index < 8);
    return m_child[index];
}

OctreeNode * OctreeNode::_GetParent()
{
    return m_parent;
}

bool OctreeNode::_IsRoot()
{
    return m_parent == NULL;
}

bool OctreeNode::_IsNil()
{
    return !_IsRoot() && GetNodeSize() == 0 &&
        m_child[0] == NULL && m_child[1] == NULL &&
        m_child[2] == NULL && m_child[3] == NULL &&
        m_child[4] == NULL && m_child[5] == NULL &&
        m_child[6] == NULL && m_child[7] == NULL;
}

void OctreeNode::_NotifyErase(OctreeNode * child)
{
    if (m_child[0] == child)
        m_child[0] = NULL;
    else if (m_child[1] == child)
        m_child[1] = NULL;
    else if (m_child[2] == child)
        m_child[2] = NULL;
    else if (m_child[3] == child)
        m_child[3] = NULL;
    else if (m_child[4] == child)
        m_child[4] = NULL;
    else if (m_child[5] == child)
        m_child[5] = NULL;
    else if (m_child[6] == child)
        m_child[6] = NULL;
    else if (m_child[7] == child)
        m_child[7] = NULL;
    else
        d_assert(0);
}

const Aabb & OctreeNode::GetBound() const
{
    return m_bound;
}

bool OctreeNode::IsInside(const Aabb & bound)
{
    return Math::AABBInside(m_bound, bound);
}

void OctreeNode::AddNode(SceneNode * node)
{
    _dirt dirt;
    dirt.node = this;
    node->_NotifyAttach(m_creator, &dirt, sizeof(_dirt));

    _node * mynode = m_creator->AllocScNode();
    mynode->node = node;
    mynode->next = m_head;
    m_head = mynode;
    ++m_size;
}

void OctreeNode::RemoveNode(SceneNode * node)
{
    _node * earseNode = m_head;
    while (earseNode && earseNode->node != node)
    {
        earseNode = earseNode->next;
    }

    d_assert (earseNode);

    earseNode->node = m_head->node;
    earseNode = m_head;
    m_head = m_head->next;
    node->_NotifyDetach(m_creator);
    m_creator->FreeScNode(earseNode);
    --m_size;
}

void OctreeNode::RemoveAllNode()
{
    _node * earseNode = m_head;

    while (earseNode)
    {
        earseNode->node->_NotifyDetach(m_creator);
        m_creator->FreeScNode(earseNode);
        earseNode = earseNode->next;
    }

    m_head = NULL;
    m_size = 0;
}

void OctreeNode::Clear()
{
    RemoveAllNode();

    for (int i = 0; i < 8; ++i)
    {
        if (m_child[i])
            m_child[i]->Clear();
    }
}

int OctreeNode::GetNodeSize()
{
    return m_size;
}

void OctreeNode::GetVisibleSceneNodes(SceneNodeList & list, Camera * cam)
{
    _node * mynode = m_head;

    while (mynode)
    {
        SceneNode * node = mynode->node;
        const Aabb & bound = node->GetWorldAabb();

        if (node->IsVisible() && cam->IsVisible(bound))
        {
            list.PushBack(node);
        }

        mynode = mynode->next;
    }
}

void OctreeNode::GetSceneNodesInBound(SceneNodeList & list, const Aabb & bound)
{
    _node * head = m_head;

    while (head)
    {
        SceneNode * node = head->node;
        const Aabb & aabb = node->GetWorldAabb();

        if (Math::AABBIntersection(aabb, bound) == 0)
        {
            list.PushBack(node);
        }

        head = head->next;
    }
}

void OctreeNode::GetSceneNodesInSphere(SceneNodeList & list, const Sphere & sph)
{
    _node * head = m_head;

    while (head)
    {
        SceneNode * node = head->node;
        const Aabb & aabb = node->GetWorldAabb();

        if (Math::AABBIntersection(aabb, sph) == 0)
        {
            list.PushBack(node);
        }

        head = head->next;
    }
}

void OctreeNode::RayTracing(const Ray & ray, float dist, Array<Scene::TraceInfo> & nodes, int flag)
{
    _node * head = m_head;

    while (head)
    {
        SceneNode * node = head->node;

        if (node->GetFlag().TestAny(flag))
        {
            RayIntersectionInfo info;

            const Aabb & aabb = node->GetWorldAabb();
            Math::RayIntersection(info, ray, aabb);
            if (info.iterscetion == TRUE && info.distance >= 0 && info.distance <= dist)
            {
				Scene::TraceInfo tinfo = { node, info.distance };
                nodes.PushBack(tinfo);
            }
        }

        head = head->next;
    }
}

void OctreeNode::_GetAllNodes(SceneNodeList & list)
{
    _node * head = m_head;

    while (head)
    {
        list.PushBack(head->node);

        head = head->next;
    }

    for (int i = 0; i < 8; ++i)
    {
        if (m_child[i])
            m_child[i]->_GetAllNodes(list);
    }
}









#define _MAKE_POSITION(x, y, z) ((z << 2) | (y << 1) | (x))

OctreeScene::OctreeScene(const Aabb & bound)
: Scene(bound)
{
    m_root = NULL;
    Resize(bound);
}

OctreeScene::~OctreeScene()
{
    //////////////////////////////////////////////////////////////////////////
    // Bug: 2011 - 1 - 03
    //
    //  没用清除节点的SceneNode而导致析构出错。
    //
    m_root->Clear();
}

void OctreeScene::Resize(const Aabb & bound)
{
    SceneNodeList list;

    // remove all scene node.
    if (m_root)
        m_root->_GetAllNodes(list);

    SceneNodeList::Iterator whr = list.Begin();
    SceneNodeList::Iterator end = list.End();

    while (whr != end)
    {
        RemoveSceneNode(*whr);

        ++whr;
    }

    m_root = NULL;
    m_OctAlloc.Shutdown();
    m_ScAlloc.Shutdown();

    m_root = AllocOctNode();
    m_root->SetBound(bound);

    // add all scene node.
    whr = list.Begin();

    while (whr != end)
    {
        AddSceneNode(*whr);

        ++whr;
    }
}

void OctreeScene::UpdateSceneNode(SceneNode * node)
{
    d_assert (node->_GetScene() == this);

    // check dirt
    OctreeNode::_dirt dirt = *(OctreeNode::_dirt*)node->_GetSceneDirtData();

    if (!dirt.node->IsInside(node->GetWorldAabb()))
    {
        // readd the scene node.
        RemoveSceneNode(node);
        AddSceneNode(node);
    }
}

void OctreeScene::AddSceneNode(SceneNode * node)
{
    const Aabb & bound = node->GetWorldAabb();

    // check in this scene
    Scene * scene = node->_GetScene();
    if (scene == this)
        return;
    else if (scene)
        scene->RemoveSceneNode(node);

    // if not in root, force in root.
    if (!Math::AABBInside(m_root->GetBound(), bound))
    {
        m_root->AddNode(node);
    }
    else
    {
        OctreeNode * ocnode = _InsideNode(m_root, 0, bound);
        ocnode->AddNode(node);
    }
}

OctreeNode * OctreeScene::_InsideChild(OctreeNode * node, const Aabb & bound)
{
    const Aabb & ndbound = node->GetBound();

    Vec3 _half = ndbound.GetHalfSize();
    Vec3 _min = bound.minimum - ndbound.minimum;
    Vec3 _max = bound.maximum - ndbound.minimum;

    int ix0 = 0, iy0 = 0, iz0 = 0;
    int ix1 = 0, iy1 = 0, iz1 = 0;
    Aabb clbound(ndbound.minimum, ndbound.minimum);

    if (_min.x >= _half.x)
    {
        ix0 = 1;
        clbound.minimum.x += _half.x;
    }
    if (_min.y >= _half.y)
    {
        iy0 = 1;
        clbound.minimum.y += _half.y;
    }
    if (_min.z >= _half.z)
    {
        iz0 = 1;
        clbound.minimum.z += _half.z;
    }

    if (_max.x >= _half.x)
        ix1 = 1;
    if (_max.y >= _half.y)
        iy1 = 1;
    if (_max.z >= _half.z)
        iz1 = 1;

    clbound.maximum = clbound.minimum + _half;

    if (ix0 != ix1 || iy0 != iy1 || iz0 != iz1)
        return NULL;

    d_assert (Math::AABBInside(clbound, bound));

    int index = _MAKE_POSITION(ix0, iy1, iz1);

    OctreeNode * child = node->_GetChild(index);
    if (child == NULL)
    {
        child = AllocOctNode();
        child->SetBound(clbound);
        node->_SetChild(index, child);
    }

    return child;
}

OctreeNode * OctreeScene::_InsideNode(OctreeNode * node, int level, const Aabb & bound)
{
    if (level == MAX_OCTREE_LEVEL - 1)
        return node;

    OctreeNode * child = _InsideChild(node, bound);

    if (child)
    {
        return _InsideNode(child, level + 1, bound);
    }

    return node;
}


void OctreeScene::RemoveSceneNode(SceneNode * node)
{
   OctreeNode::_dirt dirt = *(OctreeNode::_dirt*)node->_GetSceneDirtData();
   dirt.node->RemoveNode(node);

   OctreeNode * eraseNode = dirt.node;
   if (eraseNode->_IsNil())
   {
       OctreeNode * parent = dirt.node->_GetParent();
       parent->_NotifyErase(eraseNode);
       FreeOctNode(eraseNode);
       eraseNode = parent;
   }
}

bool OctreeScene::HasSceneNode(SceneNode * node)
{
    return node->_GetScene() == this;
}

void OctreeScene::_GetVisibleSceneNodes(OctreeNode * node, SceneNodeList & list, Camera * cam)
{
    if (cam->IsVisible(node->GetBound()))
    {
        node->GetVisibleSceneNodes(list, cam);

        for (int i = 0; i < 8; ++i)
        {
            OctreeNode * child = node->_GetChild(i);
            if (child)
                _GetVisibleSceneNodes(child, list, cam);
        }
    }
}

void OctreeScene::GetVisibleSceneNodes(SceneNodeList & list, Camera * cam)
{
    _GetVisibleSceneNodes(m_root, list, cam);
}

void OctreeScene::GetSceneNodesInBound(SceneNodeList & list, const Aabb & bound)
{
}

void OctreeScene::GetSceneNodesInSphere(SceneNodeList & list, const Sphere & sph)
{
}


void OctreeScene::RayTracing(const Ray & ray, float dist,
                              Array<Scene::TraceInfo> & nodes,
                              int mark)
{
    _RayTracing(m_root, ray, dist, nodes, mark);
}

void OctreeScene::_RayTracing(OctreeNode * node, const Ray & ray, float dist,
                              Array<Scene::TraceInfo> & nodes, int mark)
{
    const Aabb & bound = node->GetBound();

    RayIntersectionInfo info;
    Math::RayIntersection(info, ray, bound);
    if (info.iterscetion == TRUE && info.distance >= -Math::EPSILON_E4 && info.distance <= dist)
    {
        node->RayTracing(ray, dist, nodes, mark);

        for (int i = 0; i < 8; ++i)
        {
            OctreeNode * child = node->_GetChild(i);
            if (child)
            {
                _RayTracing(child, ray, dist, nodes, mark);
            }
        }
    }
}

OctreeNode * OctreeScene::AllocOctNode()
{
    OctreeNode * node = m_OctAlloc.Alloc();
    node->_Create(this);
    return node;
}

void OctreeScene::FreeOctNode(OctreeNode * node)
{
    m_OctAlloc.Free(node);
}

OctreeNode::_node * OctreeScene::AllocScNode()
{
    OctreeNode::_node * node = m_ScAlloc.Alloc();
    node->next = NULL;
    return node;
}

void OctreeScene::FreeScNode(OctreeNode::_node * node)
{
    m_ScAlloc.Free(node);
}

SceneType OctreeScene::GetType() const
{
    return ST_OCTREE;
}