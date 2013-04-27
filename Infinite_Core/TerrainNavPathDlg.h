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

namespace Infinite {

	class TerrainNavPathDlg : public wraps::BaseLayout
	{
	public:
		TerrainNavPathDlg(MyGUI::Widget * _parent);
		virtual ~TerrainNavPathDlg();

	protected:
		void notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyBrushTypeChanged(MyGUI::ComboBox* _sender, size_t _index);
		void notifyBrushOperatorChanged(MyGUI::ComboBox* _sender, size_t _index);

	protected:
		MyGUI::ScrollBar * mBrushSize;
		MyGUI::ComboBox * mBrushOperator;
	};

}