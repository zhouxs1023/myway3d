#pragma once

namespace Infinite
{
	typedef MyGUI::delegates::CDelegate3<const std::string&, const std::string&, bool> PropertyFieldActionDelegate;

	class IPropertyField
	{
	public:
		IPropertyField(IPropertyObj * obj, const Property * prop) : mObject(obj), mProperty(prop) {}
		virtual ~IPropertyField() { }
		
		virtual IPropertyObj * getObject() { return mObject; }
		virtual const Property * getProperty() { return mProperty; };

		virtual MyGUI::IntSize getContentSize() = 0;
		virtual void setCoord(const MyGUI::IntCoord& _coord) = 0;

	protected:
		IPropertyObj * mObject;
		const Property * mProperty;
	};
	
}

