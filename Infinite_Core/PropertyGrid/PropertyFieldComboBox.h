//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "Common\\BaseLayout.h"
#include "IPropertyField.h"
#include "PropertyGroup.h"

namespace Infinite
{
	class PropertyFieldEnum :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldEnum(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldEnum();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyComboAccept(MyGUI::ComboBox* _widget, size_t _index);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::ComboBox* mField;
	};



	class PropertyFieldBool :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldBool(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldBool();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyComboAccept(MyGUI::ComboBox* _widget, size_t _index);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::ComboBox* mField;
	};
}

