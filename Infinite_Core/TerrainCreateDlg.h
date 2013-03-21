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

	class TerrainCreateDlg : public wraps::BaseLayout
	{
	public:
		Event Event_OnOK;
		Event Event_OnCancel;

	public:
		TerrainCreateDlg();
		virtual ~TerrainCreateDlg();

		void DoModal();

		float GetXSize();
		float GetZSize();
		int GetXVertSize();
		int GetZVertSize();

	protected:
		void OnInit();

		void EndDialog();

		void OnOK(MyGUI::Widget *);
		void OnCancel(MyGUI::Widget *);

	protected:
		MyGUI::EditBox * mXSize;
		MyGUI::EditBox * mZSize;
		MyGUI::ComboBox * mXVertSize;
		MyGUI::ComboBox * mZVertSize;

		MyGUI::Button * mOK;
		MyGUI::Button * mCancel;
	};

}