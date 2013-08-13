#pragma once

#include "GameCoreEntry.h"

struct GCORE_ENTRY GmHelper {

	static const TString32 S_Anim_Idle0;
	static const TString32 S_Anim_Idle1;
	static const TString32 S_Anim_Walk;
	static const TString32 S_Anim_Run;
	
	static Vec3 GetRayCheckPosition();
};

class GmActor;

namespace GmEngine {

	struct GCORE_ENTRY RayCheckInfo {
		bool bPicked;
		Vec3 vPosition;
		GmActor * pObject;

		RayCheckInfo()
		{
			bPicked = false;
			vPosition = Vec3(0, 0, 0);
			pObject = NULL;
		}
	};

	GCORE_ENTRY RayCheckInfo RayCheck(Ray ray, float dist);
}