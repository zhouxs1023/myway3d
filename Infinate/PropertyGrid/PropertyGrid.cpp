#include "stdafx.h"

#include "PropertyGrid.h"
#include "PropertyFieldComboBox.h"
#include "PropertyFieldEditBox.h"
#include "PropertyFieldColour.h"
#include "PropertyFieldVector.h"

namespace Infinite {


	PropertyGrid::PropertyGrid(MyGUI::Widget* _parent)
		: BaseLayout("PropertyGrid.layout", _parent)
	{
		assignBase(mPanelView, "scroll_View");

		//mMainProperty = new PropertyGroup("test");
		//mPanelView->addItem(mMainProperty);
	}

	PropertyGrid::~PropertyGrid()
	{
		_shutdown();
	}

	void PropertyGrid::SetObject(IPropertyObj * obj)
	{
		_shutdown();

		mObject = obj;

		if (obj == NULL)
			return ;

		int size = obj->GetPropertySize();

		for (int i = 0; i < size; ++i)
			_createProperty(obj->GetProperty(i));
	}

	IPropertyField * PropertyGrid::_createProperty(const Property * prop)
	{
		IPropertyField * field = NULL;
		PropertyGroup * pg = NULL;

		if (prop->group == "")
			pg = _getPropertyGroup("Default");
		else
			pg = _getPropertyGroup(prop->group.c_str());

		if (prop->type == PT_Bool)
		{
			field = new PropertyFieldBool(mObject, prop, pg);
		}
		else if (prop->type == PT_Int && prop->enumObj)
		{
			field = new PropertyFieldEnum(mObject, prop, pg);
		}
		else if (prop->type == PT_Int)
		{
			field = new PropertyFieldInt(mObject, prop, pg);
		}
		else if (prop->type == PT_Float)
		{
			field = new PropertyFieldFloat(mObject, prop, pg);
		}
		else if (prop->type == PT_Color)
		{
			field = new PropertyFieldColour(mObject, prop, pg);
		}
		else if (prop->type == PT_TString)
		{
			field = new PropertyFieldString(mObject, prop, pg);
		}
		else if (prop->type == PT_Filename)
		{
			field = new PropertyFieldFileName(mObject, prop, pg);
		}
		else if (prop->type == PT_Vec2)
		{
			field = new PropertyFieldVec2(mObject, prop, pg);
		}
		else if (prop->type == PT_Vec3)
		{
			field = new PropertyFieldVec3(mObject, prop, pg);
		}
		else if (prop->type == PT_Vec4)
		{
			field = new PropertyFieldVec4(mObject, prop, pg);
		}
		else 
		{
			d_assert (0);
		}

		if (field)
			pg->Add(field);

		return field;
	}

	PropertyGroup * PropertyGrid::_getPropertyGroup(const char * gourpName)
	{
		for (int i = 0; i < mGroups.Size(); ++i)
		{
			if (mGroups[i]->getName() == gourpName)
				return mGroups[i];
		}

		PropertyGroup * pg = new PropertyGroup(gourpName);

		mPanelView->addItem(pg);

		mGroups.PushBack(pg);

		return pg;
	}

	void PropertyGrid::_shutdown()
	{
		mPanelView->removeAllItems();

		for (int i = 0; i < mGroups.Size(); ++i)
			delete mGroups[i];

		mGroups.Clear();
	}



}