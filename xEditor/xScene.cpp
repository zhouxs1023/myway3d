#include "stdafx.h"
#include "xScene.h"
#include "xEvent.h"

xScene gScene;

IMP_SLN(xScene);


static const int Magic = 'XSC0';

xScene::xScene()
{
	mDirt = false;

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
		xEvent::OnUnloadScene(NULL);
	}

	mFilename = filename;
	mFloder = floder;

	xEvent::OnNewScene(NULL);

	ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

	Save();

	Archive * ar = rg->GetArchive(mFloder);

	d_assert (ar != NULL);

	ar->Unload();
	ar->Load();

	return true;
}

void xScene::Load(const char * filename)
{
	if (IsInited())
	{
		xEvent::OnUnloadScene(NULL);
	}
	
	// load

	mDirt = false;
}

void xScene::Save()
{
	/*TString128 filename = mFloder + "\\" + mFilename;

	File file;

	file.Open(filename.c_str(), OM_WRITE);

	OnSaveScene(NULL);*/

	mDirt = false;
}

void xScene::Export()
{
}