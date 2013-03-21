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

namespace Infinite {


	class PropertyFieldVec2 :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldVec2(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldVec2();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyTextChanged(MyGUI::EditBox* _sender);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mFieldX;
		MyGUI::EditBox* mFieldY;
	};







	class PropertyFieldVec3 :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldVec3(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldVec3();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyTextChanged(MyGUI::EditBox* _sender);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mFieldX;
		MyGUI::EditBox* mFieldY;
		MyGUI::EditBox* mFieldZ;
	};









	class PropertyFieldVec4 :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldVec4(IPropertyObj * obj, const Property * prop, PropertyGroup* _parent);
		virtual ~PropertyFieldVec4();

		virtual void setTarget(IPropertyObj * _obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyTextChanged(MyGUI::EditBox* _sender);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mFieldX;
		MyGUI::EditBox* mFieldY;
		MyGUI::EditBox* mFieldZ;
		MyGUI::EditBox* mFieldW;
	};
}