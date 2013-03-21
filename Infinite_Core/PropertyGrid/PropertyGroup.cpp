#include "stdafx.h"

#include "PropertyGroup.h"

namespace Infinite {

	PropertyGroup::PropertyGroup(const MyGUI::UString & groupName)
		: BasePanelViewItem("PropertyGroup.layout")
		, mName(groupName)
	{
		mHeight = 0;
	}

	void PropertyGroup::initialise()
	{
		mPanelCell->setCaption(mName);
	}

	void PropertyGroup::shutdown()
	{
		for (int i = 0; i < mProperties.Size(); ++i)
		{
			delete mProperties[i];
		}

		mProperties.Clear();
	}
	

}