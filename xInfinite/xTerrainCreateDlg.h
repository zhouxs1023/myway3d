#pragma once

class xTerrainCreateDlg : public CDialog
{
public:
	xTerrainCreateDlg();
	~xTerrainCreateDlg();

	void OnOK();

	float GetSizeX() { return mSizeX; }
	float GetSizeZ() { return mSizeZ; }
	int GetVertX() { return mVertX; }
	int GetVertZ() { return mVertZ; }

protected:
	BOOL OnInitDialog();

	float mSizeX, mSizeZ;
	int mVertX, mVertZ;
};