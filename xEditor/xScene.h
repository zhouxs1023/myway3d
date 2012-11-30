#pragma once

#include "xEditor.h"

class X_ENTRY xScene
{
	DECLARE_SINGLETON (xScene)

public:
	xScene();
	~xScene();

	bool New(const char * filename, const char * floder);
	void Load(const char * filename);
	void Save();

	void Export();

	bool IsInited() { return mFilename != ""; }

	void DirtSave() { mDirt = true; }
	bool IsDirtSave() { return IsInited() &&& mDirt; }

protected:
	TString128 mFloder;
	TString128 mFilename;
	bool mDirt;
};