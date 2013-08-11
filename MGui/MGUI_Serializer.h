#pragma once

#include "MGUI_Widget.h"
#include "MGUI_LookFeel.h"

namespace Myway {

	class MGUI_ENTRY MGUI_Serializer
	{
	public:
		MGUI_Serializer() {}
		virtual ~MGUI_Serializer() {}

		virtual TString128 GetTypeName() = 0;

		virtual MGUI_Widget * LoadWidget(xml_node * _node, MGUI_Widget * _parent) = 0;
		virtual MGUI_LookFeel * LoadLookFeel(xml_node * _node) = 0;

		virtual void SaveWidget(xml_node * _node, MGUI_Widget * _widget) = 0;
		virtual void SaveLookFeel(xml_node * _node, MGUI_LookFeel * _lookfeel) = 0;
	};

}