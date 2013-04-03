#pragma once

#include "Common\\BaseLayout.h"

namespace Infinite {

	class TerrainWaterDlg : public wraps::BaseLayout
	{
	public:
		TerrainWaterDlg(MyGUI::Widget * _parent);
		virtual ~TerrainWaterDlg();

		void _UnloadScene(Event * sender);
		void _AfterLoadScene(Event * sender);

	protected:
		void notifyBrushSizeDensityChanged(MyGUI::ScrollBar * _sender, size_t _position);
		void notifyBrushOperatorChanged(MyGUI::ComboBox * _sender, size_t _index);
		void notifyWaterHeightChanged(MyGUI::EditBox * _sender);

	protected:
		MyGUI::ScrollBar * mBrushSize;
		MyGUI::EditBox * mWaterHeight;
		MyGUI::ComboBox * mBrushOperator;
	};

}