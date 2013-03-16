#include "stdafx.h"
#include "xScene.h"
#include "xEvent.h"
#include "xSerializer.h"

IMP_SLN(xScene);

static const int Magic = 'XSC0';

xScene::xScene()
{
	mDirt = false;
	mIsLoading = false;

	INIT_SLN;
}

xScene::~xScene()
{
	SHUT_SLN;
}

bool xScene::New(const char * filename, const char * floder)
{
	if (IsInited())
	{
		xEvent::OnUnloadScene(NULL, NULL);
	}

	mFilename = filename;
	mFloder = floder;

	xEvent::OnNewScene(NULL, NULL);

	Save();

	return true;
}

bool xScene::Load(const char * filename, const char * floder)
{
	if (IsInited())
	{
		xEvent::OnUnloadScene(NULL, NULL);
	}

	mFilename = filename;
	mFloder = floder;

	TString128 sceneFile = mFloder + "\\" + mFilename;
	xSerializer Serializer(sceneFile.c_str(), false);

	if (!Serializer.IsOpen())
	{
		return false;
	}

	mIsLoading = true;

	xEvent::OnLoadScene(NULL, NULL);
	
	int magic = 0;

	Serializer >> magic;

	d_assert (magic == Magic);


	int chunkId = -1;
	Serializer >> chunkId;

	while (!Serializer.eof())
	{
		mDirtLoadChunk = false;

		{
			xEvent::OnSerialize(&chunkId, &Serializer);
			d_assert (mDirtLoadChunk);
		}

		mDirtLoadChunk = false;

		Serializer >> chunkId;
	}

	xEvent::OnAfterLoadScene(NULL, NULL);

	mDirt = false;
	mIsLoading = false;

	return true;
}

void xScene::Save()
{
	if (!IsInited())
		return ;

	TString128 filename = mFloder + "\\" + mFilename;

	xSerializer Serializer(filename.c_str(), true);

	Serializer << Magic;

	int chunkId = 0;
	xEvent::OnSerialize(&chunkId, &Serializer);

	xEvent::OnSaveScene(NULL, NULL);

	// reload archive
	ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

	Archive * ar = rg->GetArchive(mFloder);

	d_assert (ar != NULL);

	ar->Unload();
	ar->Load();

	mDirt = false;
}

void xScene::Export()
{
}

void xScene::DirtSave()
{ 
	mDirt = true;
}

bool xScene::IsDirtSave()
{ 
	return IsInited() && mDirt;
}
