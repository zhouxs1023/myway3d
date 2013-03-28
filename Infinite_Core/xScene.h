//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "Infinite.h"

namespace Infinite {

class FileDialog;

class INFI_ENTRY xScene
{
	DECLARE_SINGLETON (xScene)

public:
	xScene();
	~xScene();

	bool New(const char * filename, const char * floder);
	bool Load(const char * filename, const char * floder);
	void Save();

	void Import(const char * filename, const char * floder);

	void Export();

	bool IsInited() { return mFilename != ""; }

	void DirtSave();
	bool IsDirtSave();

	const TString128 & GetFloder() { return mFloder; }
	const TString128 & GetFileName() { return mFilename; }

	bool IsLoading() { return mIsLoading; }

	void DirtLoadChunk() { mDirtLoadChunk = true; }

protected:
	void notifyImport(FileDialog * _sender, bool _ok);

protected:
	TString128 mFloder;
	TString128 mFilename;
	bool mDirt;
	bool mIsLoading;
	bool mDirtLoadChunk;


	TString128 mImportFilename;
	TString128 mImportFloder;
};

}
