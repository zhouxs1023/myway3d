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

	class TerrainLayerInfoDlg : public wraps::BaseLayout
	{
	public:
		Event Event_OnOK;

	public:
		TerrainLayerInfoDlg();
		virtual ~TerrainLayerInfoDlg();

		void DoModal(const Terrain::Layer * layer);

		TString128 GetDiffuseMap();
		TString128 GetNormalMap();
		TString128 GetSpecularMap();
		float GetUVScale();

		const Terrain::Layer * GetLayer() { return mLayer; }

	protected:
		void EndDialog();

		void OnOK(MyGUI::Widget * _sender);
		void OnCancel(MyGUI::Widget * _sender);

	protected:
		MyGUI::EditBox * mDiffuseMap;
		MyGUI::EditBox * mNormalMap;
		MyGUI::EditBox * mSpecularMap;
		MyGUI::EditBox * mUVScale;
		MyGUI::EditBox * mMaterialId;

		MyGUI::Button * mOk;
		MyGUI::Button * mCancel;

		const Terrain::Layer * mLayer;
	};

}
