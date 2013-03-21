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
#include "MForest.h"

namespace Infinite {

	class TerrainVegInfoDlg : public wraps::BaseLayout
	{
	public:
		Event Event_OnOK;

	public:
		TerrainVegInfoDlg();
		virtual ~TerrainVegInfoDlg();

		void DoModal(MVegetation * veg);

		MVegetation * GetVegetation() { return mVegetation; }

		TString32 Name();
		MVegetation::GeomType Type();
		TString128 MeshFile();
		TString128 DiffuseMap();
		TString128 SpecularMap();
		TString128 NormalMap();

	protected:
		void EndDialog();

		void OnOK(MyGUI::Widget * _sender);
		void OnCancel(MyGUI::Widget * _sender);

	protected:
		MyGUI::EditBox * mName;
		MyGUI::ComboBox * mType;
		MyGUI::EditBox * mMesh;
		MyGUI::EditBox * mDiffuseMap;
		MyGUI::EditBox * mNormalMap;
		MyGUI::EditBox * mSpecularMap;
		MyGUI::Button * mOk;
		MyGUI::Button * mCancel;

		MVegetation * mVegetation;
	};

}