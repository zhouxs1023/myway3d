#pragma once

class xOpToolBar
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
	void Init(void *);
	void Shutdown(void *);

	void Update(void *);
	void Render(void *);

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