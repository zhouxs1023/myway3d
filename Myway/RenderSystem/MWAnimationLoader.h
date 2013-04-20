#pragma once

#include "MWRenderer.h"

namespace Myway {

	class Animation;

	class MW_ENTRY AnimationLoader
	{
	public:
		static const int K_Magic = 'ANIM';
		static const int K_Version = 0;

		static const int K_SkinAnim = 0x01;
		static const int K_SkinAnimVersion = 0;

		static const int K_MorphAnim = 0x02;
		static const int K_MorphAnimVersion = 0;

	public:
		static void Load(Animation * anim, DataStreamPtr stream);

	protected:
		static void ReadSkinAnim(Animation * anim, DataStreamPtr stream);
	};

}