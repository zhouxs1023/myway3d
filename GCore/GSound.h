#pragma once

#include "GEntity.h"

namespace game {

	class GSound : public GEntity
	{
	public:
		GSound(const char * name, const char * source);
		~GSound();

		void SetVolume(int v);
		int GetVolume() const;

		void SetDistance(float minDist, float maxDist);
		void GetDistance(float & minDist, float & maxDist) const;

	protected:
		Sound * mSound;
	};

}