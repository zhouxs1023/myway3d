#pragma once

#include "MVegetation.h"

class xVegetationDlg : public CDialog
{
public:
	xVegetationDlg(MVegetation * veg);
	~xVegetationDlg();

	const TString32 & Name() const { return mName; }
	MVegetation::GeomType Type() const { return mType; }
	const TString128 & MeshFile() const { return mMesh; }
	const TString128 & DiffuseMap() const { return mDiffuseMap; }
	const TString128 & SpecularMap() const { return mSpecularMap; }
	const TString128 & NormalMap() const { return mNormalMap; }

protected:
	BOOL OnInitDialog();
	void OnOK();

protected:
	TString32 mName;
	MVegetation::GeomType mType;
	TString128 mMesh;
	TString128 mDiffuseMap;
	TString128 mNormalMap;
	TString128 mSpecularMap;
};