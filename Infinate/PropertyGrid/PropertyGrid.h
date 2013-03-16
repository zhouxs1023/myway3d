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

#include "BasePanelView.h"
#include "PropertyGroup.h"

namespace Infinite {

	class PanelViewCell :
		public wraps::BasePanelViewCell
	{
	public:
		PanelViewCell(MyGUI::Widget* _parent) :
		  BasePanelViewCell("PanelCell.layout", _parent),
			  mButtonMinimize(nullptr)
		  {
			  assignWidget(mTextCaption, "text_Caption");
			  assignWidget(mButtonMinimize, "button_Minimize");
			  assignWidget(mWidgetClient, "widget_Client");

			  mTextCaption->eventMouseButtonDoubleClick += MyGUI::newDelegate(this, &PanelViewCell::notifyMouseButtonDoubleClick);
			  mButtonMinimize->eventMouseButtonPressed += MyGUI::newDelegate(this, &PanelViewCell::notfyMouseButtonPressed);
		  }

		  virtual void setMinimized(bool _minimized)
		  {
			  wraps::BasePanelViewCell::setMinimized(_minimized);
			  mButtonMinimize->setStateSelected(isMinimized());
		  }

	private:
		void notfyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left)
			{
				setMinimized( ! isMinimized() );
			}
		}

		void notifyMouseButtonDoubleClick(MyGUI::Widget* _sender)
		{
			setMinimized( ! isMinimized() );
		}

	private:
		MyGUI::Button* mButtonMinimize;

	};

	class PanelView :
		public wraps::BasePanelView<PanelViewCell>
	{
	public:
		PanelView(MyGUI::Widget* _parent) :
		  wraps::BasePanelView<PanelViewCell>("", _parent)
		  {
		  }
	};






	class PropertyGrid : public wraps::BaseLayout
	{
	public:
		PropertyGrid(MyGUI::Widget* _parent);
		~PropertyGrid();

		void SetObject(IPropertyObj * obj);

	protected:
		void _shutdown();

		IPropertyField * _createProperty(const Property * prop);
		PropertyGroup * _getPropertyGroup(const char * gourpName);

	protected:
		MyGUI::IntSize mOldSize;
		PanelView* mPanelView;

		IPropertyObj * mObject;

		Array<PropertyGroup *> mGroups;
	};
}