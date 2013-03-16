#pragma once

#include "Infinite.h"

class INFI_ENTRY xScene
{
	DECLARE_SINGLETON (xScene)

public:
	xScene();
	~xScene();

	bool New(const char * filename, const char * floder);
	bool Load(const char * filename, const char * floder);
	void Save();

	void Export();

	bool IsInited() { return mFilename != ""; }

	void DirtSave();
	bool IsDirtSave();

	const TString128 & GetFloder() { return mFloder; }
	const TString128 & GetFileName() { return mFilename; }

	bool IsLoading() { return mIsLoading; }

	void DirtLoadChunk() { mDirtLoadChunk = true; }

protected:
	TString128 mFloder;
	TString128 mFilename;
	bool mDirt;
	bool mIsLoading;
	bool mDirtLoadChunk;
};