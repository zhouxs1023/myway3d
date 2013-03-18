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
#include "MForest.h"
#include "TerrainVegInfoDlg.h"

namespace Infinite {

	class TerrainVegDlg : public wraps::BaseLayout
	{
	public:
		TerrainVegDlg(MyGUI::Widget * _parent);
		virtual ~TerrainVegDlg();

		void _UnloadScene(Event * sender);
		void _AfterLoadScene(Event * sender);

		MVegetation * GetCurVeg();

		bool IsAddMode();
		float BrushSize();
		int BrushDensity();
		float MinSize();
		float MaxSize();
		float MinLum();
		float MaxLum();

	protected:
		void _refrushList();

		void notifyAdd(MyGUI::Widget * _sender);
		void notifyRemove(MyGUI::Widget * _sender);
		void notifyEdit(MyGUI::Widget * _sender);

		void ShowInfoDlg(MVegetation * veg);

		void _OnAddVeg(Event * _sender);
		void _OnEditVeg(Event * _sender);

	protected:
		MyGUI::EditBox * mBrushSize;
		MyGUI::EditBox * mBrushDensity;
		MyGUI::ComboBox * mBrushType;

		MyGUI::Button * mAdd;
		MyGUI::Button * mRemove;
		MyGUI::Button * mEdit;

		MyGUI::EditBox * mMinSize;
		MyGUI::EditBox * mMaxSize;
		MyGUI::EditBox * mMinLum;
		MyGUI::EditBox * mMaxLum;

		MyGUI::ListBox * mVegList;

		TerrainVegInfoDlg * mVegInfoDlg;

		tEventListener<TerrainVegDlg> OnAddVeg;
		tEventListener<TerrainVegDlg> OnEditVeg;
	};
}