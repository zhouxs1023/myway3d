#pragma once

#include "xEditor.h"

class X_ENTRY xOpToolBar
{
	static const int MAX_BUTTONS = 32;
	static const int ICON_SIZE = 24;

	struct Button
	{
		TString128 image;
		int id;
	};

	DECLARE_SINGLETON(xOpToolBar);

public:
	xOpToolBar();
	virtual ~xOpToolBar();

	void Add(const char * image, int id);

protected:
	void Init(Event * sender);
	void Shutdown(Event * sender);

	void Update(Event * sender);
	void Render(Event * sender);

protected:
	int mNumButtons;
	Button mButtons[MAX_BUTTONS];
	TexturePtr mTexture[MAX_BUTTONS];

	int mCheckedButton;

	Technique * mTech_Back;
	Technique * mTech_Button;
	Technique * mTech_Checked;

	tEventListener<xOpToolBar> mOnInitUI;
	tEventListener<xOpToolBar> mOnRender;
	tEventListener<xOpToolBar> mOnUpdate;
	tEventListener<xOpToolBar> mOnShutdown;
};