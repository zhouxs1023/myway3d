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
	class PropertyFieldInt :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldInt(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldInt();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyTextChanged(MyGUI::EditBox* _sender);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
	};


	class PropertyFieldFloat :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldFloat(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldFloat();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyTextChanged(MyGUI::EditBox* _sender);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
	};

	 
	class PropertyFieldString :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldString(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldString();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyTextChanged(MyGUI::EditBox* _sender);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
	};

	typedef PropertyFieldString PropertyFieldFileName;
}

