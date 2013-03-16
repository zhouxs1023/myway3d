#pragma once

#include "BaseLayout.h"
#include "IPropertyField.h"
#include "PropertyGroup.h"
#include "ColourPanel.h"

namespace Infinite
{
	class PropertyFieldColour :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldColour(IPropertyObj * obj, const Property * prop, PropertyGroup * _parent);
		virtual ~PropertyFieldColour();

		virtual void setTarget(IPropertyObj * obj);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	protected:
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

		void notifyPreviewColour(const MyGUI::Colour & _clr);
		void notifyEndDialog(ColourPanel * _sender, bool _ok);

	private:
		MyGUI::TextBox* mText;
		MyGUI::Widget* mField;

		Color4 mOldColour;
	};

}

