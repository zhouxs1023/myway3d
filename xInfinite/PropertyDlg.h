#pragma once

class PropertyDlg : public CDockablePane
{
	DECLARE_MESSAGE_MAP()

public:
	PropertyDlg();
	~PropertyDlg();

	void Init();

protected:
	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
};