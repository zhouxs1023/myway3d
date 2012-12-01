#include "stdafx.h"
#include "xScene.h"
#include "xEvent.h"
#include "xSerializer.h"

xScene gScene;

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

void xScene::Load(const char * filename, const char * floder)
{
	if (IsInited())
	{
		xEvent::OnUnloadScene(NULL, NULL);
	}

	mIsLoading = true;

	xEvent::OnLoadScene(NULL, NULL);

	mFilename = filename;
	mFloder = floder;

	TString128 sceneFile = mFloder + "\\" + mFilename;
	xSerializer Serializer(sceneFile.c_str(), false);

	int magic = 0;

	Serializer >> magic;

	d_assert (magic == Magic);

	while (!Serializer.eof())
	{
		int chunkId = -1;

		Serializer >> chunkId;

		xEvent::OnSerialize(&chunkId, &Serializer);
	}

	mDirt = false;
	mIsLoading = false;
}

void xScene::Save()
{
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