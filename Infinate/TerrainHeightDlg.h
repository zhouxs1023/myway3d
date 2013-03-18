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

#include "BaseLayout.h"

namespace Infinite {

	class TerrainHeightDlg : public wraps::BaseLayout
	{
	public:
		TerrainHeightDlg(MyGUI::Widget * _parent);
		virtual ~TerrainHeightDlg();

	protected:
		void notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyBrushTypeChanged(MyGUI::ComboBox* _sender, size_t _index);
		void notifyBrushOperatorChanged(MyGUI::ComboBox* _sender, size_t _index);

	protected:
		MyGUI::ScrollBar * mBrushSize;
		MyGUI::ScrollBar * mBrushDensity;
		MyGUI::ComboBox * mBrushType;
		MyGUI::ComboBox * mBrushOperator;
	};

}