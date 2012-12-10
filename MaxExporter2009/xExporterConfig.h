#pragma once

class xExportConfig
{
	DECLARE_SINGLETON(xExportConfig)

public:
	xExportConfig();
	~xExportConfig();

	void SetExportPath(const TString128 & path) { mExportPath = path; }
	const TString128 & GetExportPath() const { return mExportPath; }

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

protected:
	TString128 mExportPath;
	bool mExportSelected;
	bool mExportNormal;
	bool mExportTangent;
	bool mExportColor;
	bool mExportTexcoord;
	bool mExportLightmapUV;
};