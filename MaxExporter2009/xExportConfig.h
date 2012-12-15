#pragma once

class xExportConfig
{
	DECLARE_SINGLETON(xExportConfig)

public:
	xExportConfig();
	~xExportConfig();

	void Load();
	void Save();

	void SetExportFilename(const TString128 & filename);

	const TString128 & GetExportFilename() const { return mExportFilename; }
	const TString128 & GetExportPath() const { return mExportPath; }
	const TString128 & GetExportBaseName() const { return mExportBaseName; }

	void SetExportSelected(bool b) { mExportSelected = b; }
	bool IsExportSelected() const { return mExportSelected; }

	void SetExportNormal(bool b) { mExportNormal = b; }
	bool IsExportNormal() const { return mExportNormal; }

	void SetExportTangent(bool b) { mExportTangent = b; }
	bool IsExportTangent() const { return mExportTangent; }

	void SetExportColor(bool b) { mExportColor = b; }
	bool IsExportColor() const { return mExportColor; }

	void SetExportTexcoord(bool b) { mExportTexcoord = b; }
	bool IsExportTexcoord() const { return mExportTexcoord; }

	void SetExportLightmapUV(bool b) { mExportLightmapUV = b; }
	bool IsExportLightmapUV() const { return mExportLightmapUV; }

	void SetExportAnimation(bool b) { mExportAnimation = b; }
	bool IsExportAnimation() const { return mExportAnimation; }

protected:
	TString128 mExportFilename;
	TString128 mExportPath;
	TString128 mExportBaseName;

	bool mExportSelected;
	bool mExportNormal;
	bool mExportTangent;
	bool mExportColor;
	bool mExportTexcoord;
	bool mExportLightmapUV;

	bool mExportAnimation;
};