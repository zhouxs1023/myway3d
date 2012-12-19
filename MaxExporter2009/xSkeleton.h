#pragma once

namespace MaxPlugin{

	struct xKeyFrame
	{
		float time;

		Vec3 position;
		MQuat orientation;
		Vec3 scale;

		bool operator == (const xKeyFrame & rk) const
		{
			return position == rk.position &&
				   orientation == rk.orientation &&
				   scale == rk.scale;
		}
	};

	struct xBone
	{
		short parent;

		TString128 name;

		Vec3 position;
		MQuat orientation;
		Vec3 scale;

		Array<xKeyFrame> keyFrames;

		void Optimize()
		{
			while (keyFrames.Size() > 3)
			{
				int size = keyFrames.Size();
				int end_0 = size - 1;
				int end_1 = size - 2;
				int end_2 = size - 3;

				xKeyFrame & e0 = keyFrames[end_0];
				xKeyFrame & e1 = keyFrames[end_1];
				xKeyFrame & e2 = keyFrames[end_2];

				if (e0 == e1 && e0 == e2)
				{
					e1.time = e0.time;

					keyFrames.Erase(end_0);
				}
				else
				{
					break;
				}
			}
		}
	};

	class xSkeleton
	{
		DECLARE_SINGLETON(xSkeleton);

	public:
		xSkeleton();
		~xSkeleton();

		void Extrat(IGameNode * node);

		int GetBoneCount() { return mBones.Size(); }
		xBone * GetBone(int i) { return mBones[i]; }

		int _getBoneId(const char * name);

	protected:
		void _dumpBone(IGameNode * node);

		void Push(IGameNode * node);

	protected:
		Array<IGameNode *> mMaxBones;

		Array<xBone*> mBones;
	};
}