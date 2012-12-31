#include "MWEntity.h"
#include "MWRenderQueue.h"
#include "MWMeshManager.h"
#include "MWSkeletonManager.h"
#include "MWMaterialManager.h"
#include "MWResourceManager.h"


using namespace Myway;


SubEntity::SubEntity(Entity * parent)
: mParent(parent),
  mSoftSkined(FALSE)
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

    if (mParent->HasSkeletion())
    {
        SkeletonInstance * skel = mParent->GetSkeletonInstance();

		if (forms == NULL)
			return skel->GetBoneCount();

		count = skel->GetBoneMatrix(forms);

        if (mSoftSkined)
        {
            char * src[MAX_VERTEX_STREAM] = { NULL };
            char * dest[MAX_VERTEX_STREAM] = { NULL };
            int stride[MAX_VERTEX_STREAM] = { 0 };

            // implement soft skined.
            for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
            {
                if (mSkElement.needLocked[i])
                {
                    src[i] = (char *)mSkElement.srcbuffer[i]->Lock(0, 0, LOCK_READONLY);
                    dest[i] = (char *)mSkElement.destbuffer[i]->Lock(0, 0, LOCK_DISCARD);
                    stride[i] = mVertexStream.GetStreamStride(i);
                }
            }

            for (int i = 0; i < mSkStream.GetCount(); ++i)
            {
                Vec3 * dpos = NULL, * dnormal = NULL, * dtangent = NULL;
                const Vec3 * spos = NULL, * snormal = NULL, * stangent = NULL;
                SkinData infl;

                spos = (Vec3 *)(src[mSkElement.position->Stream] + mSkElement.position->Offset);
                dpos = (Vec3 *)(dest[mSkElement.position->Stream] + mSkElement.position->Offset);

                if (mSkElement.normal)
                {
                    snormal = (Vec3 *)(src[mSkElement.normal->Stream] + mSkElement.normal->Offset);
                    dnormal = (Vec3 *)(dest[mSkElement.normal->Stream] + mSkElement.normal->Offset);
                }

                if (mSkElement.tangent)
                {
                    stangent = (Vec3 *)(src[mSkElement.tangent->Stream] + mSkElement.normal->Offset);
                    dtangent = (Vec3 *)(dest[mSkElement.tangent->Stream] + mSkElement.normal->Offset);
                }

                for (int bi = 0; bi < 4; ++bi)
                {
                    infl.indices[bi] = *((unsigned char*)(src[mSkElement.indeces->Stream] + mSkElement.indeces->Offset) + bi);
                    infl.weights[bi] = *((float*)(src[mSkElement.weights->Stream] + mSkElement.weights->Offset) + bi);
                }

                Math::UpdateSkinned(dpos, dnormal, dtangent, spos, snormal, stangent, &infl, forms, 1);

                for (int m = 0; m < MAX_VERTEX_STREAM; ++m)
                {
                    src[m] += stride[m];
                    dest[m] += stride[m];
                }
            }

            for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
            {
                if (mSkElement.needLocked[i])
                {
                    mSkElement.srcbuffer[i]->Unlock();
                    mSkElement.destbuffer[i]->Unlock();
                }
            }

            return 0;
        }
        else
        {
			return count;
        }
    }
    else
    {
        return 0;
    }
}

VertexStream * SubEntity::GetVertexStream()
{
    if (mSoftSkined)
    {
        return &mSkStream;
    }
    else
    {
        return Renderer::GetVertexStream();
    }
}

void SubEntity::NotifySoftSkined()
{
    SkeletonInstance * skel = mParent->GetSkeletonInstance();

    VertexDeclarationPtr decl = mVertexStream.GetDeclaration();
    const VertexElement * veps = decl->GetElementByUsage(DU_POSITION);
    const VertexElement * venm = decl->GetElementByUsage(DU_NORMAL);
    const VertexElement * vetg = decl->GetElementByUsage(DU_TANGENT);
    const VertexElement * vebi = decl->GetElementByUsage(DU_BLENDINDICES);
    const VertexElement * vewt = decl->GetElementByUsage(DU_BLENDWEIGHT);

    if (veps->Type != DT_FLOAT3 &&
        venm->Type != DT_FLOAT3 &&
        vetg->Type != DT_FLOAT3 &&
        vebi->Type != DT_UBYTE4 &&
        vewt->Type != DT_FLOAT4)
    {
        return ;
    }

    mSkStream.SetDeclaration(decl);
    mSkStream.SetCount(mVertexStream.GetCount());

    for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
    {
        VertexBufferPtr buffer = mVertexStream.GetStream(i);

        if (buffer == NULL)
            continue;

        int stide = mVertexStream.GetStreamStride(i);
        int size = buffer->GetSize();

        if ((veps && veps->Stream != i) &&
            (venm && venm->Stream != i) &&
            (vetg && vetg->Stream != i) &&
            (vebi && vebi->Stream != i) &&
            (vewt && vewt->Stream != i))
        {
            mSkStream.Bind(i, buffer, stide);
        }
        else
        {
            VertexBufferPtr nbuffer = VideoBufferManager::Instance()->CreateVertexBuffer(
                size, USAGE_DYNAMIC);

            void * src = buffer->Lock(0, 0, LOCK_READONLY);
            void * dest = nbuffer->Lock(0, 0, LOCK_DISCARD);

            Memcpy(dest, src, size);

            nbuffer->Unlock();
            buffer->Unlock();

            mSkStream.Bind(i, nbuffer, stide);

            mSkElement.srcbuffer[i] = buffer;
            mSkElement.destbuffer[i] = nbuffer;
            mSkElement.needLocked[i] = TRUE;
        }
    }

    mSkElement.position = veps;
    mSkElement.normal = venm;
    mSkElement.tangent = vetg;
    mSkElement.indeces = vebi;
    mSkElement.weights = vewt;

    d_assert(mSkElement.position);

    // set default animation data
    char * dest[MAX_VERTEX_STREAM] = { NULL };
    int stride[MAX_VERTEX_STREAM] = { 0 };

    for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
    {
        if (mSkElement.indeces->Stream == i ||
            mSkElement.weights->Stream == i)
        {
            dest[i] = (char *)mSkElement.destbuffer[i]->Lock(0, 0, LOCK_DISCARD);
            stride[i] = mSkStream.GetStreamStride(i);
        }
    }

    for (int i = 0; i < mSkStream.GetCount(); ++i)
    {
        unsigned char * indeces = (unsigned char*)(dest[mSkElement.indeces->Stream] + mSkElement.indeces->Offset);
        float * weights = (float*)(dest[mSkElement.weights->Stream] + mSkElement.weights->Offset);

        indeces[0] = 0, indeces[1] = 0, indeces[2] = 0, indeces[3] = 0;
        weights[0] = 1, weights[1] = 0, weights[2] = 0, weights[3] = 0;

        for (int m = 0; m < MAX_VERTEX_STREAM; ++m)
        {
            dest[m] += stride[m];
        }
    }

    for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
    {
        if (mSkElement.indeces->Stream == i ||
            mSkElement.weights->Stream == i)
        {
            mSkElement.destbuffer[i]->Unlock();
        }
    }

    mSoftSkined = TRUE;
}



/* :) 
--------------------------------------------------------------
    entity
--------------------------------------------------------------
*/
Entity::Entity(const TString128 & name)
: Mover(name),
  mMesh(NULL),
  mSkeleton(NULL),
  mAnims(NULL)
{
}

Entity::Entity(const TString128 & name, MeshPtr mesh)
: Mover(name),
  mMesh(mesh),
  mSkeleton(NULL),
  mAnims(NULL)
{
    _Init();
}

Entity::~Entity(void)
{
    _DeInit();
}

void Entity::_Init()
{
    if (mMesh == NULL)
        return ;

    for (int i = 0; i < mMesh->GetSubMeshCount(); ++i)
    {
        SubEntity * se              = new SubEntity(this);
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
		mAnims = new AnimationSet();

		for (int i = 0; i < mSkeleton->GetAnimationCount(); ++i)
		{
			Animation * anim = mSkeleton->GetAnimation(i);
			mAnims->CreateState(anim);
		}

		if (skel->GetJointCount() > MAX_BLEND_MATRIX_VS)
		{
			for (int i = 0; i < mEntitys.Size(); ++i)
			{
				mEntitys[i]->NotifySoftSkined();
			}
		}
    }

    SetBounds(mMesh->GetAabb(), mMesh->GetBoundingSphere());
}

void Entity::_DeInit()
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

    safe_delete(mAnims);
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

    _DeInit();

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

void Entity::UpdateMover()
{
    UpdateAnimation();
}

void Entity::AddRenderQueue(RenderQueue * rq)
{
    for (int i = 0; i < mEntitys.Size(); ++i)
    {
        rq->AddRenderer(mEntitys[i]);
    }
}

void Entity::UpdateAnimation()
{
    if (!mSkeleton || !mAnims)
        return;

    mSkeleton->ResetBone();

    AnimationSet::ActiveStateVisitor v = mAnims->GetActiveStates();

    while (!v.Endof())
    {
        AnimationState * state = *v.Cursor();
        state->_UpdateAnimation(mSkeleton);

        ++v;
    }

    mSkeleton->UpdateBoneMatrix();
}


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

AnimationSet * Entity::GetAnimationSet()
{
    return mAnims;
}

bool Entity::HasSkeletion()
{
    return mSkeleton != NULL;
}