#include "MWAnimationLoader.h"
#include "MWAnimation.h"

namespace Myway {

	void AnimationLoader::Load(Animation * anim, DataStreamPtr stream)
	{
		int magic, version;

		stream->Read(&magic, sizeof(int));
		stream->Read(&version, sizeof(int));

		d_assert (magic == K_Magic && version == K_Version);

		int ckId = 0;

		while (stream->Read(&ckId, sizeof(int)) == 1)
		{
			switch (ckId)
			{
			case K_SkinAnim:
				ReadSkinAnim(anim, stream);
				break;

			case K_MorphAnim:
				break;
			}
		}
	}

	void AnimationLoader::ReadSkinAnim(Animation * anim, DataStreamPtr stream)
	{
		int version = -1;
		stream->Read(&version, sizeof(int));

		d_assert (version == K_SkinAnimVersion);

		int numAnims = 0;
		stream->Read(&numAnims, sizeof(int));

		for (int i = 0; i < numAnims; ++i)
		{
			int boneId, count;

			stream->Read(&boneId, sizeof(int));
			stream->Read(&count, sizeof(int));

			if (count == 0)
				continue;

			SkeletonAnimation * skel_anim;
			skel_anim = anim->CreateSkeletonAnimation(boneId);

			KeyFrame * kf;
			float time;
			Vec3 trans;
			Quat rotate;
			Vec3 scale;

			for (int i = 0; i < count; ++i)
			{
				stream->Read(&time, sizeof(float));
				stream->Read(&trans, sizeof(float) * 3);
				stream->Read(&rotate, sizeof(float) * 4);
				stream->Read(&scale, sizeof(float) * 3);

				kf = skel_anim->CreateKeyFrame();
				kf->SetTime(time);
				kf->SetTranslate(trans);
				kf->SetRotation(rotate);
				kf->SetScale(scale);
			}
		}

		anim->_calcuLength();
	}
}
