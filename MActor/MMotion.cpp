#include "MMotion.h"
#include "MWResourceManager.h"
#include <EMotionFX.h>

namespace Myway {

	MMotion::MMotion(const TString128 & source)
	{
		SetSourceName(source);
		mMotion = NULL;

		mName = File::GetBaseName(source);
		mName = File::RemoveExternName(mName);
		mName.Lower();
	}

	MMotion::~MMotion()
	{
		safe_delete (mMotion);
	}

	const TString128 & MMotion::GetName()
	{
		return mName;
	}

	float MMotion::GetLength()
	{
		if (mMotion)
			return mMotion->GetMaxTime();
		else
			return 0;
	}

	// override Resource
	void MMotion::Load()
	{
		if (CanLoad() && GetLoadState() == Resource::UNLOADED)
		{
			ResourceManager::Instance()->GetResourceLoader()->Load(this);
		}

		mLoadState = Resource::LOADED;
	}

	void MMotion::Reload()
	{
		Unload();
		Load();
	}

	void MMotion::Unload()
	{
		if (CanLoad() && GetLoadState() == Resource::LOADED)
		{
			safe_delete (mMotion);
		}
	}

	void MMotion::LoadImp(DataStreamPtr stream)
	{
		mMotion = EMotionFX::IMPORTER.LoadMotion((unsigned char*)stream->GetData(), stream->Size(), mName.c_str());
		d_assert (mMotion != NULL);
	}
}

