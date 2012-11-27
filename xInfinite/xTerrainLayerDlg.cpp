#include "stdafx.h"

#include "xTerrainLayerDlg.h"
#include "xApp.h"
#include "resource.h"


//////////////////////////////////////////////////////////////////////////
//
// Terrain Layer
//

BEGIN_MESSAGE_MAP(xTerrainLayerDlg, CDialog)

END_MESSAGE_MAP()

xTerrainLayerDlg::xTerrainLayerDlg()
{
}

xTerrainLayerDlg::~xTerrainLayerDlg()
{
}

BOOL xTerrainLayerDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	return TRUE;
}

void xTerrainLayerDlg::_Init(void *)
{
	CComboBox * cbBrushType = (CComboBox *)GetDlgItem(IDC_Combo_TL_BrushType);

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
