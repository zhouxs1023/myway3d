#include "stdafx.h"

#include "GSound.h"

namespace game {

	GSound::GSound(const char * name, const char * source)
	{
		mSound = AudioSystem::Instance()->CreateSound(name, source);
		mNode->Attach(mSound);
	}

	GSound::~GSound()
	{
		AudioSystem::Instance()->DestroySound(mSound);
	}

	void GSound::SetVolume(int v)
	{
		mSound->SetVolume(v);
	}

	int GSound::GetVolume() const
	{
		return mSound->GetVolume();
	}

	void GSound::SetDistance(float minDist, float maxDist)
	{
		mSound->SetDistance(minDist, maxDist);
	}

	void GSound::GetDistance(float & minDist, float & maxDist) const
	{
		minDist = mSound->GetMinDistance();
		maxDist = mSound->GetMaxDistance();
	}

}