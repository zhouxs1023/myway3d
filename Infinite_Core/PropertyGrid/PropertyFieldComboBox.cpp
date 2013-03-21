#include "stdafx.h"

#include "PropertyFieldComboBox.h"

namespace Infinite
{
	PropertyFieldEnum::PropertyFieldEnum(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldComboBox.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		d_assert (prop->enumObj != NULL && prop->type == PT_Int);

		mText->setCaption(prop->displayName.c_wstr());

		for (int i = 0; i < prop->enumObj->GetEnumCount(); ++i)
		{
			mField->insertItemAt(i, prop->enumObj->GetEnum(i)->name.c_str());
		}

		setTarget(obj);

		mField->eventComboAccept += newDelegate (this, &PropertyFieldEnum::notifyComboAccept);
	}

	PropertyFieldEnum::~PropertyFieldEnum()
	{
		mField->eventComboAccept -= newDelegate (this, &PropertyFieldEnum::notifyComboAccept);
	}

	void PropertyFieldEnum::setTarget(IPropertyObj * obj)
	{
		mObject = obj;

		int val = mProperty->AsInt(mObject->GetPropertyData(mProperty));

		for (int i = 0; i < mProperty->enumObj->GetEnumCount(); ++i)
		{
			if (mProperty->enumObj->GetEnum(i)->val == val)
			{
				mField->setIndexSelected(i);
				return ;
			}
		}

		d_assert (0);
	}

	void PropertyFieldEnum::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		const char * ename = _sender->getItemNameAt(_index).asUTF8_c_str();
		const cEnum * en = mProperty->enumObj->GetEnum(ename);

		d_assert (en != NULL);

		mObject->SetPropertyData(mProperty, &en->val);
		mObject->OnPropertyChanged(mProperty);
	}

	MyGUI::IntSize PropertyFieldEnum::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldEnum::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

















	PropertyFieldBool::PropertyFieldBool(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldComboBox.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		d_assert (prop->type == PT_Bool);

		mText->setCaption(prop->displayName.c_str());

		mField->insertItemAt(0, "true");
		mField->insertItemAt(1, "false");

		setTarget(obj);

		//mField->setEnabled(false);

		mField->eventComboAccept += newDelegate (this, &PropertyFieldBool::notifyComboAccept);
	}

	PropertyFieldBool::~PropertyFieldBool()
	{
		mField->eventComboAccept -= newDelegate (this, &PropertyFieldBool::notifyComboAccept);
	}

	void PropertyFieldBool::setTarget(IPropertyObj * obj)
	{
		mObject = obj;

		bool val = mProperty->AsBool(mObject->GetPropertyData(mProperty));

		if (val)
			mField->setIndexSelected(0);
		else
			mField->setIndexSelected(1);
	}

	void PropertyFieldBool::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		bool val = _index == 0;

		mObject->SetPropertyData(mProperty, &val);
		mObject->OnPropertyChanged(mProperty);
	}

	MyGUI::IntSize PropertyFieldBool::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldBool::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}
}
