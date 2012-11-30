#pragma once


class xTerrainLayerInfoDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainLayerInfoDlg(const Terrain::Layer * layer);
	~xTerrainLayerInfoDlg();

	const TString128 & GetDetailMap() { return mDetailMap; }
	const TString128 & GetNormalMap() { return mNormalMap; }
	const TString128 & GetSpecularMap() { return mSpecularMap; }
	float GetUVScale() { return mUVScale; }

protected:
	void OnOK();
	BOOL OnInitDialog();

protected:
	const Terrain::Layer * mLayer;
	TString128 mDetailMap;
	TString128 mNormalMap;
	TString128 mSpecularMap;
	float mUVScale;
};

