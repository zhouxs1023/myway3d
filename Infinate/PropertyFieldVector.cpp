#include "stdafx.h"

#include "PropertyFieldVector.h"

namespace Infinite {


	PropertyFieldVec2::PropertyFieldVec2(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldVec2.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mFieldX(nullptr)
		, mFieldY(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mFieldX, "FieldX");
		assignWidget(mFieldY, "FieldY");

		d_assert (prop->type == PT_Vec3);

		mText->setCaption(prop->displayName.c_wstr());

		setTarget(obj);

		mFieldX->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec2::notifyTextChanged);
		mFieldY->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec2::notifyTextChanged);
	}

	PropertyFieldVec2::~PropertyFieldVec2()
	{
	}

	void PropertyFieldVec2::setTarget(IPropertyObj * _obj)
	{
		mObject = _obj;

		Vec2 val = mProperty->AsVec2(mObject->GetPropertyData(mProperty));

		char buf[256];

		sprintf_s(buf, 256, "%.3f", val.x);
		mFieldX->setCaption(buf);

		sprintf_s(buf, 256, "%.3f", val.y);
		mFieldY->setCaption(buf);
	}

	MyGUI::IntSize PropertyFieldVec2::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldVec2::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldVec2::notifyTextChanged(MyGUI::EditBox* _sender)
	{
		Vec2 v;

		const char * xval = mFieldX->getCaption().asUTF8_c_str();
		const char * yval = mFieldY->getCaption().asUTF8_c_str();

		v.x = (float)atof(xval);
		v.y = (float)atof(yval);

		mObject->SetPropertyData(mProperty, &v);
		mObject->OnPropertyChanged(mProperty);
	}









	PropertyFieldVec3::PropertyFieldVec3(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldVec3.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mFieldX(nullptr)
		, mFieldY(nullptr)
		, mFieldZ(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mFieldX, "FieldX");
		assignWidget(mFieldY, "FieldY");
		assignWidget(mFieldZ, "FieldZ");

		d_assert (prop->type == PT_Vec3);

		mText->setCaption(prop->displayName.c_wstr());

		setTarget(obj);

		mFieldX->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec3::notifyTextChanged);
		mFieldY->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec3::notifyTextChanged);
		mFieldZ->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec3::notifyTextChanged);
	}

	PropertyFieldVec3::~PropertyFieldVec3()
	{
	}

	void PropertyFieldVec3::setTarget(IPropertyObj * _obj)
	{
		mObject = _obj;

		Vec3 val = mProperty->AsVec3(mObject->GetPropertyData(mProperty));

		char buf[256];

		sprintf_s(buf, 256, "%.3f", val.x);
		mFieldX->setCaption(buf);

		sprintf_s(buf, 256, "%.3f", val.y);
		mFieldY->setCaption(buf);

		sprintf_s(buf, 256, "%.3f", val.z);
		mFieldZ->setCaption(buf);
	}

	MyGUI::IntSize PropertyFieldVec3::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldVec3::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldVec3::notifyTextChanged(MyGUI::EditBox* _sender)
	{
		Vec3 v;

		const char * xval = mFieldX->getCaption().asUTF8_c_str();
		const char * yval = mFieldY->getCaption().asUTF8_c_str();
		const char * zval = mFieldZ->getCaption().asUTF8_c_str();

		v.x = (float)atof(xval);
		v.y = (float)atof(yval);
		v.z = (float)atof(zval);

		mObject->SetPropertyData(mProperty, &v);

		mObject->OnPropertyChanged(mProperty);
	}















	PropertyFieldVec4::PropertyFieldVec4(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent)
		: BaseLayout("PropertyFieldVec4.layout", _parent->getClient())
		, IPropertyField(obj, prop)
		, mText(nullptr)
		, mFieldX(nullptr)
		, mFieldY(nullptr)
		, mFieldZ(nullptr)
		, mFieldW(nullptr)
	{
		assignWidget(mText, "Text");
		assignWidget(mFieldX, "FieldX");
		assignWidget(mFieldY, "FieldY");
		assignWidget(mFieldZ, "FieldZ");
		assignWidget(mFieldW, "FieldW");

		d_assert (prop->type == PT_Vec4);

		mText->setCaption(prop->displayName.c_wstr());

		setTarget(obj);

		mFieldX->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec4::notifyTextChanged);
		mFieldY->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec4::notifyTextChanged);
		mFieldZ->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec4::notifyTextChanged);
		mFieldW->eventEditTextChange += MyGUI::newDelegate (this, &PropertyFieldVec4::notifyTextChanged);
	}

	PropertyFieldVec4::~PropertyFieldVec4()
	{
	}

	void PropertyFieldVec4::setTarget(IPropertyObj * _obj)
	{
		mObject = _obj;

		Vec4 val = mProperty->AsVec4(mObject->GetPropertyData(mProperty));

		char buf[256];

		sprintf_s(buf, 256, "%.3f", val.x);
		mFieldX->setCaption(buf);

		sprintf_s(buf, 256, "%.3f", val.y);
		mFieldY->setCaption(buf);

		sprintf_s(buf, 256, "%.3f", val.z);
		mFieldZ->setCaption(buf);

		sprintf_s(buf, 256, "%.3f", val.w);
		mFieldW->setCaption(buf);
	}

	MyGUI::IntSize PropertyFieldVec4::getContentSize()
	{
		return MyGUI::IntSize(0, mMainWidget->getHeight());
	}

	void PropertyFieldVec4::setCoord(const MyGUI::IntCoord& _coord)
	{
		mMainWidget->setCoord(_coord);
	}

	void PropertyFieldVec4::notifyTextChanged(MyGUI::EditBox* _sender)
	{
		Vec4 v;

		const char * xval = mFieldX->getCaption().asUTF8_c_str();
		const char * yval = mFieldY->getCaption().asUTF8_c_str();
		const char * zval = mFieldZ->getCaption().asUTF8_c_str();
		const char * wval = mFieldW->getCaption().asUTF8_c_str();

		v.x = (float)atof(xval);
		v.y = (float)atof(yval);
		v.z = (float)atof(zval);
		v.w = (float)atof(wval);

		mObject->SetPropertyData(mProperty, &v);

		mObject->OnPropertyChanged(mProperty);
	}
}