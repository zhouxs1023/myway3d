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
#include "xEditTerrainHeight.h"
#include "xEditTerrainLayer.h"
#include "xEditTerrainVeg.h"

namespace Infinite {

	class TerrainHeightDlg : public wraps::BaseLayout
	{
	public:
		TerrainHeightDlg(MyGUI::Widget * _parent);
		virtual ~TerrainHeightDlg();


	protected:
		void notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position);

	protected:
		MyGUI::ScrollBar * mBrushSize;
		MyGUI::ScrollBar * mBrushDensity;
		MyGUI::ComboBox * mBrushType;
		MyGUI::ComboBox * mBrushOperator;
	};


	class TerrainPane : public wraps::BaseLayout
	{
	public:
		TerrainPane(MyGUI::Widget * _parent);
		virtual ~TerrainPane();

		xEditTerrainHeight * GetTerrainHeight() { return &mEditHeight; }
		xEditTerrainLayer * GetTerrainLayer() { return &mEditLayer; }
		xEditTerrainVeg * GetTerrainVeg() { return &mEditVeg; }

	protected:
		void _Init(Event * sender);
		void _Shutdown(Event * sender);
		void _Update(Event * sender);
		void _Render(Event * sender);
		void _RenderUI(Event * sender);
		void _UnloadScene(Event * sender);
		void _AfterloadScene(Event * sender);

	protected:
		tEventListener<TerrainPane> OnInit;
		tEventListener<TerrainPane> OnShutdown;
		tEventListener<TerrainPane> OnUpdate;
		tEventListener<TerrainPane> OnRender;
		tEventListener<TerrainPane> OnRenderUI;
		tEventListener<TerrainPane> OnUnLoadScene;
		tEventListener<TerrainPane> OnAfterLoadScene;

		xEditTerrainHeight mEditHeight;
		xEditTerrainLayer mEditLayer;
		xEditTerrainVeg mEditVeg;

		MyGUI::TabControl * mTabControl;
	};
}