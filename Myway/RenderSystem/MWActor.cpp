#include "MWActor.h"
#include "MWAnimationLoader.h"
#include "MWResourceManager.h"
#include "Engine.h"

namespace Myway {

	Actor::Actor(const TString128 & name)
		: Entity(name)
		, mAnimationSet(NULL)
	{
	}

	Actor::~Actor()
	{
		safe_delete (mAnimationSet);
	}

	void Actor::_Init()
	{
		Entity::_Init();

		mAnimationSet = new AnimationSet;

		/*for (int i = 0; i < mMesh->GetAnimationCount(); ++i)
		{
			Animation * anim = mMesh->GetAnimation(i);

			mAnimationSet->CreateState(anim);
		}*/
	}

	void Actor::_DeInit()
	{
		Entity::_DeInit();

		safe_delete(mAnimationSet);
	}

	void Actor::LoadAnimation(const char * name, const char * source)
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

	void Actor::PlayAnimation(const char * name, const MotionBlendInfo & mbi)
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

	void Actor::UpdateGeometry()
	{
		if (mAnimationSet == NULL || mSkeleton == NULL)
			return ;

		mSkeleton->ResetBone();

		float frameTime = Engine::Instance()->GetFrameTime();

		mAnimationSet->UpdateAnimation(frameTime, mSkeleton);

		mSkeleton->UpdateBoneMatrix();
	}

}