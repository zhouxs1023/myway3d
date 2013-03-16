#pragma once

#include "BaseLayout.h"
#include "BasePanelViewItem.h"

#include "IPropertyField.h"

namespace Infinite {

	class PropertyGroup : public wraps::BasePanelViewItem
	{
		typedef std::map<std::string, IPropertyField*> MapPropertyField;

	public:
		PropertyGroup(const MyGUI::UString & groupName);

		virtual void initialise();
		virtual void shutdown();

		MyGUI::Widget * getClient() { return mWidgetClient; }

		const MyGUI::UString & getName() const { return mName; }

		void Add(IPropertyField * filed)
		{
			MyGUI::IntSize size = filed->getContentSize();

			filed->setCoord(MyGUI::IntCoord(0, mHeight, mMainWidget->getWidth(), size.height));

			mHeight += size.height;

			mPanelCell->setClientHeight(mHeight);

			mProperties.PushBack(filed);
		}

	private:
		MyGUI::UString mName;
		MapPropertyField mFields;
		int mHeight;
		Array<IPropertyField *> mProperties;
	};

}