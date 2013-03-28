#include "stdafx.h"

#include "xScene.h"
#include "xEvent.h"
#include "xSerializer.h"
#include "Editor.h"

namespace Infinite {

	static const int Magic = 'XSC0';

	IMP_SLN(xScene);

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

		xEvent::OnAfterLoadScene(NULL, NULL);

		Save();

		return true;
	}

	void xScene::Import(const char * filename, const char * floder)
	{
		mImportFilename = filename;
		mImportFloder = floder;

		FileDialog * dlg = Editor::Instance()->getFileDialog();

		dlg->clearFileExt();
		dlg->eventEndDialog = MyGUI::newDelegate(this, &xScene::notifyImport);

		for (int i = 0; i < Editor::Instance()->GetPluginCount(); ++i)
		{
			iPlugin * plugin = Editor::Instance()->GetPlugin(i);

			if (!(plugin->GetUsages() | iPlugin::eImport))
				continue ;

			TString128 ext = TString128(".") + plugin->GetImpExtern();

			dlg->addFileExt(ext.c_wstr());
		}

		dlg->DoModal();
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

	void xScene::notifyImport(FileDialog * _sender, bool _ok)
	{
		_sender->eventEndDialog = NULL;

		if (!_ok)
			return ;

		TString128 ext = File::GetExternName(TString128::FromUtf8(_sender->getFileMask().asUTF8_c_str()));

		iPlugin * plugin = NULL;

		for (int i = 0; i < Editor::Instance()->GetPluginCount(); ++i)
		{
			iPlugin * _p = Editor::Instance()->GetPlugin(i);

			if (!(_p->GetUsages() | iPlugin::eImport))
				continue ;

			if (ext == _p->GetImpExtern())
			{
				plugin = _p;
				break;
			}
		}

		if (plugin == NULL)
			return ;

		// import...
		if (IsInited())
		{
			xEvent::OnUnloadScene(NULL, NULL);
		}

		mFilename = mImportFilename;
		mFloder = mImportFloder;

		mIsLoading = true;

		MyGUI::UString _filename = _sender->getCurrentFolder() + "\\" + _sender->getFileName();

		plugin->Import(TString128::FromUtf8(_filename.asUTF8_c_str()).c_str());

		xEvent::OnNewScene(NULL, NULL);

		xEvent::OnAfterLoadScene(NULL, NULL);
		mIsLoading = false;
	}

}