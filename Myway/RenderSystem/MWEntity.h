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
    SubEntity(Entity * parent, SubMesh * sm);
    virtual ~SubEntity();

	virtual const Aabb & GetWorldAabb();
    virtual void GetWorldPosition(Vec3 * pos);
    virtual void GetWorldTransform(Mat4 * form);
    virtual int GetBlendMatrix(Mat4 * forms);

protected:
    Entity *        mParent;
	SubMesh *		mSubMesh;
};

class MW_ENTRY Entity : public Mover
{
    friend class SceneManager;

public:
    Entity(const TString128 & name);
    virtual ~Entity();

    void                SetMesh(const TString128 & source);
    void                SetMesh(MeshPtr mesh);
    MeshPtr             GetMesh();

    virtual void        NotifyCamera(Camera * cam);
    virtual void        AddRenderQueue(RenderQueue * rq);

    SubEntity *         GetSubEntity(int index);
    int                 GetSubEntityCount();
    SkeletonInstance *  GetSkeletonInstance();
    bool                HasSkeletion();

    void                SetUserData(void * data);
    void *              GetUserData() const;

	void				LoadAnimation(const char * name, const char * source);
	void				PlayAnimation(const char * name, const MotionBlendInfo & mbi = MotionBlendInfo::Default);
	void				UpdateAnimation(float dtime);
   
protected:
    void				_Init();
	void				_Shutdown();

protected:
    MeshPtr             mMesh;

    SkeletonInstance *  mSkeleton;
    Array<SubEntity*>   mEntitys;

	AnimationSet *		mAnimationSet;
};


}
