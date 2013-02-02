#pragma once

#include "MWCore.h"

namespace Myway
{
    class ResourceLoader;

    class MW_ENTRY Resource
    {
    public:
        enum State
        {
            LOADED,
            UNLOADED
        };

    public:
        Resource();
        virtual ~Resource();

        State GetLoadState() { return mLoadState; }

        virtual void Load() {};
        virtual void Reload() {};
        virtual void Unload() {};

        bool CanLoad() { return mSourceName != ""; }

        virtual void LoadImp(DataStreamPtr stream) {};

        void SetSourceName(const TString128 & source) { mSourceName = source; }
        const TString128 & GetSourceName() const { return mSourceName; }

		void SetPriority(float priority) { mPriority = priority; }
		float GetPriority() const { return mPriority; }

	public:
		Event OnResourceLoaded;
		Event OnResourceUnloaded;

    protected:
        TString128 mSourceName;
        State mLoadState;
		float mPriority;

    };



    class MW_ENTRY ResourceLoader
    {
        DECLARE_ALLOC();

    public:
        ResourceLoader();
        virtual ~ResourceLoader();

        virtual void Load(Resource * res);
        virtual void ForceLoad(Resource * res);

    protected:
        Array<Resource *> mResources;
    };
}