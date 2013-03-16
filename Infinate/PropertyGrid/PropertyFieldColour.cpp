#include "stdafx.h"

#include "PropertyFieldColour.h"
#include "Editor.h"

namespace Infinite
{
	PropertyFieldColour::PropertyFieldColour(IPropertyObj * obj, const Property * prop, PropertyGroup * _parent)
		: BaseLayout("PropertyFieldColour.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mField(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mField, "Field");

		mText->setCaption(prop->name.c_wstr());

		setTarget(obj);

		mField->eventMouseButtonPressed += MyGUI::newDelegate(this, &PropertyFieldColour::notifyMouseButtonPressed);
	}

	PropertyFieldColour::~PropertyFieldColour()
	{
	}

	void PropertyFieldColour::setTarget(IPropertyObj * obj)
	{
		mObject = obj;

		d_assert (mObject);

		Color4 val = mProperty->AsColor(obj->GetPropertyData(mProperty));

		MyGUI::Colour color(val.r, val.g, val.b, 1);

		mField->setColour(color);
	}

	MyGUI::IntSize PropertyFieldColour::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldColour::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldColour::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		ColourPanel::Instance()->eventPreviewColour = MyGUI::newDelegate(this, &PropertyFieldColour::notifyPreviewColour);
		ColourPanel::Instance()->eventEndDialog = MyGUI::newDelegate(this, &PropertyFieldColour::notifyEndDialog);

		Color4 val = mProperty->AsColor(mObject->GetPropertyData(mProperty));

		mOldColour = val;

		ColourPanel::Instance()->DoModal(Utils::Color4To(val));
	}

	void PropertyFieldColour::notifyPreviewColour(const MyGUI::Colour & _clr)
	{
		Color4 val = Utils::ToColor4(_clr);

		mObject->SetPropertyData(mProperty, &val);

		mObject->OnPropertyChanged(mProperty);
	}

	void PropertyFieldColour::notifyEndDialog(ColourPanel * _sender, bool _ok)
	{
		Color4 val = mOldColour;
		
		if (_ok)
			val = Utils::ToColor4(_sender->getColour());

		mObject->SetPropertyData(mProperty, &val);

		mObject->OnPropertyChanged(mProperty);

		setTarget(mObject);
	}

	/*void PropertyFieldColour::showColourDialog()
	{
	mGoodData = onCheckValue();
	if (mGoodData)
	mPreviewColour = getColour();
	else
	mPreviewColour = MyGUI::Colour::White;

	mColourPanel->setColour(mPreviewColour);
	mColourPanel->setAlphaSupport(false);
	mColourPanel->doModal();
	}*/

	

}
