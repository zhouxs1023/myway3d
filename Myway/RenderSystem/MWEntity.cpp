#include "MWEntity.h"
#include "MWRenderQueue.h"
#include "MWMeshManager.h"
#include "MWSkeletonManager.h"
#include "MWMaterialManager.h"
#include "MWResourceManager.h"


using namespace Myway;


SubEntity::SubEntity(Entity * parent, SubMesh * sm)
: mParent(parent)
, mSubMesh(sm)
{
}

SubEntity::~SubEntity()
{
}

const Aabb & SubEntity::GetWorldAabb()
{
	return mParent->GetWorldAabb();
}

void SubEntity::GetWorldPosition(Vec3 * pos)
{
    *pos = mParent->GetAttachNode()->GetWorldPosition();
}

void SubEntity::GetWorldTransform(Mat4 * forms)
{
    *forms = mParent->GetAttachNode()->GetWorldMatrix();
}

int SubEntity::GetBlendMatrix(Mat4 * forms)
{
	int count = 0;

    if (mParent->GetAttachNode()->_NeedUpdate())
        mParent->GetAttachNode()->_UpdateTransform();

	const Array<short> & boneIdMap = mSubMesh->GetBoneIdMap();

    if (mParent->HasSkeletion() && boneIdMap.Size() > 0)
    {
        SkeletonInstance * skel = mParent->GetSkeletonInstance();

		if (forms == NULL)
			return skel->GetBoneCount();

		count = boneIdMap.Size();

		for (int i = 0; i < count; ++i)
			forms[i] = skel->GetBone(boneIdMap[i])->GetBoneMatrix();

		return count;
    }
    else
    {
        return 0;
    }
}


/* :) 
--------------------------------------------------------------
    entity
--------------------------------------------------------------
*/
Entity::Entity(const TString128 & name)
: Mover(name)
, mMesh(NULL)
, mSkeleton(NULL)
, mAnimationSet(NULL)
{
}

Entity::~Entity(void)
{
    _Shutdown();
}

void Entity::_Init()
{
    if (mMesh == NULL)
        return ;

    for (int i = 0; i < mMesh->GetSubMeshCount(); ++i)
    {
        SubEntity * se              = new SubEntity(this, mMesh->GetSubMesh(i));
        SubMesh * sm                = mMesh->GetSubMesh(i);
        VertexStream * vs0          = se->GetVertexStream();
        VertexStream * vs1          = sm->GetVertexStream();
        IndexStream * is0           = se->GetIndexStream();
        IndexStream * is1           = sm->GetIndexStream();
        Material * mtl              = sm->GetMaterial();

        *vs0 = *vs1;
        *is0 = *is1;

        se->SetMaterial(mtl);
        se->SetPrimitiveCount(sm->GetPrimitiveCount());
        se->SetPrimitiveType(sm->GetPrimitiveType());

        mEntitys.PushBack(se);
    };

    if (mMesh->GetSkeleton()->GetJointCount() > 0)
    {
        Skeleton * skel = mMesh->GetSkeleton();

		mSkeleton = new SkeletonInstance(skel);
		mAnimationSet = new AnimationSet;
    }

    SetBounds(mMesh->GetAabb(), mMesh->GetBoundingSphere());
}

void Entity::_Shutdown()
{
    Array<SubEntity*>::Iterator iter;
    Array<SubEntity*>::Iterator end;

    iter = mEntitys.Begin();
    end = mEntitys.End();

    while (iter != end)
    {
        delete (*iter);

        ++iter;
    }

    mEntitys.Clear();

	safe_delete(mAnimationSet);
    safe_delete(mSkeleton);

    mMesh = NULL;
}

void Entity::SetMesh(const TString128 & source)
{
	if (ResourceManager::Instance()->Exist(source))
	{
		MeshPtr mesh = MeshManager::Instance()->Load(source, source);
		SetMesh(mesh);
	}
	else
	{
		SetMesh(MeshPtr(NULL));
	}
}

void Entity::SetMesh(MeshPtr mesh)
{
    if (mesh == mMesh)
        return ;

    _Shutdown();

    mMesh = mesh;

    _Init();

	if (mNode)
		mNode->_NotifyUpdate();
}

MeshPtr Entity::GetMesh()
{
    return mMesh;
}

void Entity::NotifyCamera(Camera * cam)
{
    Mover::NotifyCamera(cam);
}

void Entity::AddRenderQueue(RenderQueue * rq)
{
    for (int i = 0; i < mEntitys.Size(); ++i)
    {
        rq->AddRenderer(mEntitys[i]);
    }
}

//void Entity::UpdateAnimation()
//{
//	if (!mSkeleton || !mAnims)
//		return;
//
//	mSkeleton->ResetBone();
//
//	AnimationSet::ActiveStateVisitor v = mAnims->GetActiveStates();
//
//	while (!v.Endof())
//	{
//		AnimationState * state = *v.Cursor();
//		state->_UpdateAnimation(mSkeleton);
//
//		++v;
//	}
//
//	mSkeleton->UpdateBoneMatrix();
//}


SubEntity * Entity::GetSubEntity(int index)
{
    return mEntitys[index];
}

int Entity::GetSubEntityCount()
{
    return mEntitys.Size();
}


SkeletonInstance * Entity::GetSkeletonInstance()
{
    return mSkeleton;
}

bool Entity::HasSkeletion()
{
    return mSkeleton != NULL;
}

void Entity::LoadAnimation(const char * name, const char * source)
{
	d_assert (mMesh != NULL);

	if (mSkeleton == NULL || mMesh->GetAnimation(name) != NULL)
		return ;

	DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

	d_assert (stream != NULL);

	Animation * anim = mMesh->CreateAnimation(name);

	/*anim->_convertSkinAnim(mSkeleton);

	mAnimationSet->CreateState(anim);*/

	AnimationLoader::Load(anim, stream);
}

void Entity::PlayAnimation(const char * name, const MotionBlendInfo & mbi)
{
	if (mAnimationSet == NULL)
		return ;

	AnimationController * controller = NULL;

	if (!mAnimationSet->IsPlay(name))
	{
		Animation * anim = mMesh->GetAnimation(name);

		if (anim)
		{
			controller = new AnimationController(anim);

			controller->SetBlendInfo(mbi);
				
			mAnimationSet->Play(controller);
		}
	}
}

void Entity::UpdateAnimation(float dtime)
{
	if (mAnimationSet == NULL || mSkeleton == NULL)
		return ;

	mAnimationSet->UpdateAnimation(dtime, mSkeleton);

	mSkeleton->UpdateBoneMatrix();
}