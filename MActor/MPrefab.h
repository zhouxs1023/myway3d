#pragma once

#include "MActorEntry.h"
#include "MActor.h"
#include "MWWorld.h"

namespace Myway {

	class MACTOR_ENTRY MPrefab
	{
		DECLARE_ALLOC();

	public:
		MPrefab(const TString128 & name);
		~MPrefab();

		void SetName(const TString128 & name) { mName = mName; }
		const TString128 & GetName() const { return mName; }

		void Load(const TString128 & source);
		void Unload();

		SceneNode * GetRootNode() { return mRoot; }

		TString128 GetSourceName() const { return mSource; }

	protected:
		TString128 mName;
		TString128 mSource;

		Array<MActor *> mModels;
		Array<SceneNode *> mNodes;
		SceneNode * mRoot;
	};

}