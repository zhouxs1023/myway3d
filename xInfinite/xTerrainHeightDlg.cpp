#include "stdafx.h"
#include "xTerrainHeightDlg.h"
#include "xApp.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////
//
// Terrain Height
//

BEGIN_MESSAGE_MAP(xTerrainHeightDlg, CDialog)

END_MESSAGE_MAP()

xTerrainHeightDlg::xTerrainHeightDlg()
{
}

xTerrainHeightDlg::~xTerrainHeightDlg()
{
}

BOOL xTerrainHeightDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	return TRUE;
}

void xTerrainHeightDlg::_Init(void *)
{
	CComboBox * cbBrushType = (CComboBox *)GetDlgItem(IDC_Combo_TH_BrushType);
	CComboBox * cbBrushOp = (CComboBox *)GetDlgItem(IDC_Combo_TH_BrushOp);

	cbBrushOp->InsertString(0, "À­Éý");
	cbBrushOp->InsertString(1, "ÏÂ°¼");
	cbBrushOp->InsertString(2, "Æ½»¬");

	FileSystem fs("../Core/Terrain/Brushes");

	fs.Load();

	Archive::FileInfoVisitor v = fs.GetFileInfos();

	while (!v.Endof())
	{
		const Archive::FileInfo & info = v.Cursor()->second;

		if (info.type == Archive::FILE_ARCHIVE)
		{
			TString128 base = File::RemoveExternName(info.name);
			TString128 ext = File::GetExternName(info.name);

			if (ext == "png")
				cbBrushType->AddString(base.c_str());
		}

		++v;
	}
}

