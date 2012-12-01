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
	void Init(void * param0, void * param1);
	void Shutdown(void * param0, void * param1);

	void Update(void * param0, void * param1);
	void Render(void * param0, void * param1);

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