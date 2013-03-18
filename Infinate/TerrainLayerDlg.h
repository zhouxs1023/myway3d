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
#include "TerrainLayerInfoDlg.h"

namespace Infinite {

	class TerrainLayerDlg : public wraps::BaseLayout
	{
	public:
		TerrainLayerDlg(MyGUI::Widget * _parent);
		virtual ~TerrainLayerDlg();

		void _UnloadScene(Event * sender);
		void _AfterLoadScene(Event * sender);

		int GetCurLayer();

	protected:
		void notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyBrushTypeChanged(MyGUI::ComboBox* _sender, size_t _index);

		void notifyAdd(MyGUI::Widget * _sender);
		void notifyRemove(MyGUI::Widget * _sender);
		void notifyEdit(MyGUI::Widget * _sender);

		void ShowLayerInfo(const Terrain::Layer * layer);

		void _notifyAddLayer(Event * _sender);
		void _notifyEditLayer(Event * _sender);

	protected:
		MyGUI::ScrollBar * mBrushSize;
		MyGUI::ScrollBar * mBrushDensity;
		MyGUI::ComboBox * mBrushType;

		MyGUI::Button * mAdd;
		MyGUI::Button * mRemove;
		MyGUI::Button * mEdit;

		MyGUI::ListBox * mLayerList;

		Array<int> mLayerIds;

		TerrainLayerInfoDlg * mLayerInfoDlg;

		tEventListener<TerrainLayerDlg> OnAddLayer;
		tEventListener<TerrainLayerDlg> OnEditLayer;
	};
}