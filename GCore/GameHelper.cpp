#include "stdafx.h"

#include "GameHelper.h"

const TString32 GameHelper::S_Anim_Idle0 = "Idle0";
const TString32 GameHelper::S_Anim_Idle1 = "Idle1";
const TString32 GameHelper::S_Anim_Walk = "Walk";
const TString32 GameHelper::S_Anim_Run = "Run";

namespace GameEngine {

	RayCheckInfo RayCheck(Ray ray, float dist)
	{
		RayCheckInfo info;

		Terrain * terriain = Environment::Instance()->GetTerrain();

		Vec3 pos = terriain->GetPosition(ray);

		if (pos != Vec3::Zero && pos.DistanceSq(ray.origin) < dist * dist)
		{
			info.bPicked = true;
			info.vPosition = pos;
		}

		return info;
	}
}