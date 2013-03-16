#include "stdafx.h"
#include "PropertyFieldEditBox.h"

namespace Infinite
{
	PropertyFieldInt::PropertyFieldInt(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldEditBox.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		d_assert (prop->type == PT_Int && prop->enumObj == NULL);

		mText->setCaption(prop->displayName.c_wstr());
		setTarget(obj);

		mField->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldInt::notifyTextChanged);
		//mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldEditBox::notifyForceApplyProperties);
	}

	PropertyFieldInt::~PropertyFieldInt()
	{	
		mField->eventEditTextChange -= MyGUI::newDelegate (this, &PropertyFieldInt::notifyTextChanged);
	}

	void PropertyFieldInt::setTarget(IPropertyObj * obj)
	{
		mObject = obj;

		int val = mProperty->AsInt(mObject->GetPropertyData(mProperty));

		TString128 sVal(val);

		mField->setCaption(sVal.c_str());
	}

	void PropertyFieldInt::notifyTextChanged(MyGUI::EditBox* _sender)
	{
		const char * sval = mField->getCaption().asUTF8_c_str();

		int ival = atoi(sval);

		mObject->SetPropertyData(mProperty, &ival);

		mObject->OnPropertyChanged(mProperty);
	}

	MyGUI::IntSize PropertyFieldInt::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldInt::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}














	PropertyFieldFloat::PropertyFieldFloat(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldEditBox.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		d_assert (prop->type == PT_Float);

		mText->setCaption(prop->displayName.c_str());
		setTarget(obj);

		mField->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldFloat::notifyTextChanged);
		//mField->eventEditSelectAccept += newDelegate (this, &PropertyFieldEditBox::notifyForceApplyProperties);
	}

	PropertyFieldFloat::~PropertyFieldFloat()
	{
		mField->eventEditTextChange -= MyGUI::newDelegate (this, &PropertyFieldFloat::notifyTextChanged);
	}

	void PropertyFieldFloat::setTarget(IPropertyObj * obj)
	{
		mObject = obj;

		float val = mProperty->AsFloat(mObject->GetPropertyData(mProperty));

		char buf[256];

		sprintf_s(buf, 256, "%.3f", val);

		mField->setCaption(buf);
	}

	void PropertyFieldFloat::notifyTextChanged(MyGUI::EditBox* _sender)
	{
		const char * sval = mField->getCaption().asUTF8_c_str();

		float fval = (float)atof(sval);

		mObject->SetPropertyData(mProperty, &fval);

		mObject->OnPropertyChanged(mProperty);
	}

	MyGUI::IntSize PropertyFieldFloat::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldFloat::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}













	PropertyFieldString::PropertyFieldString(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldEditBox.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		d_assert (prop->type == PT_TString && prop->size == 128);

		mText->setCaption(prop->displayName.c_str());
		setTarget(obj);

		mField->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldString::notifyTextChanged);
	}

	PropertyFieldString::~PropertyFieldString()
	{
		mField->eventEditTextChange -= MyGUI::newDelegate (this, &PropertyFieldString::notifyTextChanged);
	}

	void PropertyFieldString::setTarget(IPropertyObj * obj)
	{
		mObject = obj;

		TString128 val = mProperty->AsTString<128>(mObject->GetPropertyData(mProperty));

		wchar_t buf[1024];
		CharSet::AnsiToUnicode(buf, 1024, val.c_str());

		mField->setCaption(buf);
	}

	void PropertyFieldString::notifyTextChanged(MyGUI::EditBox* _sender)
	{
		TString128 val = mField->getCaption().asUTF8_c_str();

		mObject->SetPropertyData(mProperty, &val);

		mObject->OnPropertyChanged(mProperty);
	}

	MyGUI::IntSize PropertyFieldString::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldString::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}
} 
