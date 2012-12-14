#pragma once

namespace MaxPlugin{

	struct xKeyFrame
	{
		float time;

		Point3 position;
		::Quat orientation;
		float scale;
	};

	struct xBone
	{
		short parent;

		TString128 name;

		Point3 position;
		::Quat orientation;
		Point3 scale;

		Array<xKeyFrame> keyFrames;
	};

	class xSkeleton
	{
	public:
		xSkeleton();
		~xSkeleton();

		void Extrat(IGameNode * node);

	protected:
		void _dumpBone(IGameNode * node);
		int _getBoneId(const char * name);

		void Push(IGameNode * node);

	protected:
		Array<IGameNode *> mMaxBones;

		Array<xBone*> mBones;
	};
}