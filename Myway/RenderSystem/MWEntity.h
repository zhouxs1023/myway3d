#pragma once

#include "MWRenderer.h"
#include "MWMover.h"
#include "MWMesh.h"
#include "MWSkeletonInstance.h"

namespace Myway
{

class Entity;

class MW_ENTRY SubEntity : public Renderer
{
    friend class Entity;
    friend class SceneManager;

    struct skve
    {
        const VertexElement * position;
        const VertexElement * normal;
        const VertexElement * tangent;
        const VertexElement * indeces;
        const VertexElement * weights;
        VertexBufferPtr       srcbuffer[MAX_VERTEX_STREAM];
        VertexBufferPtr       destbuffer[MAX_VERTEX_STREAM];
        bool                  needLocked[MAX_VERTEX_STREAM];

        skve()
        {
            position = NULL;
            normal = NULL;
            tangent = NULL;
            indeces = NULL;
            weights = NULL;

            for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
            {
                srcbuffer[i] = NULL;
                destbuffer[i] = NULL;
                needLocked[i] = FALSE;
            }
        }
    };

public:
    SubEntity(Entity * parent);
    ~SubEntity();

    virtual void GetWorldPosition(Vec3 * pos);
    virtual void GetWorldTransform(Mat4 * form);
    virtual void GetBlendMatrix(Mat4 * forms, int * count);

    virtual VertexStream * GetVertexStream();

protected:
    void NotifySoftSkined();

protected:
    Entity *        mParent;
    bool            mSoftSkined;
    VertexStream    mSkStream;
    skve            mSkElement;
};

class MW_ENTRY Entity : public Mover
{
    friend class SceneManager;

public:
    Entity(const TString128 & name);
    Entity(const TString128 & name, MeshPtr mesh);
    ~Entity();

    void                SetMesh(const TString128 & source);
    void                SetMesh(MeshPtr mesh);
    MeshPtr             GetMesh();

    virtual void        NotifyCamera(Camera * cam);
    virtual void        UpdateMover();
    virtual void        AddRenderQueue(RenderQueue * rq);

    void                UpdateAnimation();

    SubEntity *         GetSubEntity(int index);
    int                 GetSubEntityCount();
    SkeletonInstance *  GetSkeletonInstance();
    AnimationSet *      GetAnimationSet();
    bool                HasSkeletion();

    void                SetUserData(void * data);
    void *              GetUserData() const;
   
protected:
    void                _Init();
    void                _DeInit();

protected:
    MeshPtr             mMesh;

    SkeletonInstance *  mSkeleton;
    AnimationSet *      mAnims;

    Array<SubEntity*>   mEntitys;
};


}
