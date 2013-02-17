#include "MActor.h"
#include "MActorManager.h"
#include "Engine.h"
#include <EMotionFX.h>

namespace Myway {

	MActor::MActor(const TString128 & name)
		: Mover(name)
		, OnActorLoaded(this, &MActor::_OnActorLoaded)
		, mMainActor(NULL)
		, mRotationMatrix(Mat4::Identity)
	{
	}

	MActor::~MActor()
	{
		safe_delete(mMainActor);
	}

	void MActor::SetPart(int part, MActorResPtr pActor)
	{
		d_assert (part >= 0  && part < K_MAX_PART);

		if (mActorParts[part] == pActor)
			return ;

		OnActorLoaded.Detach();

		mActorParts[part] = pActor;

		if (part != 0)
			return;

		safe_delete (mMainActor);

		if (pActor == NULL)
			return ;

		OnActorLoaded.Attach(&pActor->OnResourceLoaded);

		if (pActor->GetLoadState() == Resource::LOADED)
		{
			_OnActorLoaded(NULL, NULL);
		}
	}

	MActorResPtr MActor::GetPart(int part)
	{
		d_assert (part >= 0 && part < K_MAX_PART);
		return mActorParts[part];
	}

	void MActor::UpdateGeometry()
	{
		_OnUpdateAnimtion();
	}

	void MActor::AddRenderQueue(RenderQueue * rq)
	{
		MActorManager::Instance()->_addVisibleActor(this);
	}

	void MActor::_OnActorLoaded(void *, void *)
	{
		const Aabb & aabb = mActorParts[0]->GetAabb();
		const Sphere & sph = mActorParts[0]->GetSphere();

		SetBounds(aabb, sph);

		_OnMainPartBind();
	}

	void MActor::_OnMainPartBind()
	{
		mMainActor = mActorParts[0]->GetEMotionActor()->Clone();
		mMainActor->SetLOD(0);
	}

	void MActor::_OnUpdateAnimtion()
	{
		float timePassed = Engine::Instance()->GetFrameTime();

		if (mMainActor)
		{
			Mat4 worldTm = mRotationMatrix * mNode->GetWorldMatrix();
			mMainActor->Update(timePassed, true, (MCore::Matrix *)&worldTm);
		}
	}

	bool MActor::PlayMotion(const TString128 & motionName, const MMotion::PlayInfo & info)
	{
		if (mActorParts[0] == NULL)
			return false;

		MMotion * motion = mActorParts[0]->GetMotion(motionName.LowerR());

		if (!motion)
			return false;

		EMotionFX::Motion * emotion = motion->GetEmotionMotion();

		if (!emotion)
			return false;

		if (mMainActor->GetMotionSystem()->IsPlayingMotion(emotion))
			return false;

		EMotionFX::PlayBackInfo pbi;

		pbi.mBlendInTime = info.BlendInTime;
		pbi.mBlendOutTime = info.BlendOutTime;
		pbi.mBlendMode = info.Overwrite ? EMotionFX::BLENDMODE_OVERWRITE : EMotionFX::BLENDMODE_ADDITIVE;
		pbi.mPlaySpeed = info.Speed;
		pbi.mNumLoops = info.Loops;
		pbi.mPlayMask = info.UseScale ? EMotionFX::PLAYMASK_POSROTSCALE : EMotionFX::PLAYMASK_POSROT;
		pbi.mMix = info.Mixed;

		mMainActor->PlayMotion(emotion, &pbi);

		return true;
	}
}