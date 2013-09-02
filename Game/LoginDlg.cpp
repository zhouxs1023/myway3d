#include "stdafx.h"

#include "LoginDlg.h"
#include "Client.h"
#include "Pack_LoginCL.h"

cLoginDlg::cLoginDlg()
{
	mLoginBackImage = new MGUI_LookFeel("LoginBackImage");
	mLoginBackImage->SetSkin("LoginBackImage.png");
	mLoginBackImage->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(true, 0, 0, 2048, 1024));
	mLoginBackImage->SetClientRect(MGUI_Rect(true, 0, 0, 2048, 1024));

	mLayout = new MGUI_Layout(mLoginBackImage, NULL);
	mLayout->eventUpdate += OnUpdate(this, &cLoginDlg::OnUpdate_);

	MGUI_Engine::Instance()->AddLayout(mLayout);

	mBnLookFeel = new MGUI_LookFeel("ScrollTrack");
	mBnLookFeel->SetSkin("UISkin.png");
	mBnLookFeel->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 198, 113 + 18, 198 + 13));
	mBnLookFeel->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 183, 113 + 18, 183 + 13));
	mBnLookFeel->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 213, 113 + 18, 213 + 13));
	mBnLookFeel->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 228, 113 + 18, 228 + 13));
	mBnLookFeel->SetClientRect(MGUI_Rect(4, 4, 8, 10));

	MGUI_Button * bnEnter = new MGUI_Button(mBnLookFeel, mLayout);
	bnEnter->SetAlign(MGUI_Align::Center);
	bnEnter->SetRect(0, 0, 64, 24);
	bnEnter->SetCaption(L"Enter");

	bnEnter->eventMouseClick += OnEnter(this, &cLoginDlg::OnEnter_);
}

cLoginDlg::~cLoginDlg()
{
	MGUI_Engine::Instance()->RemoveLayout(mLayout);
	delete mLoginBackImage;
	delete mBnLookFeel;
}

void cLoginDlg::OnUpdate_()
{
	mLayout->SetRect(MGUI_Engine::Instance()->GetRect());
}

void cLoginDlg::OnEnter_()
{
	d_assert (!Client::Instance()->IsConnect());
	Client::Instance()->Connect();

	if (!Client::Instance()->IsConnect())
	{
		// 无法连接服务器
	}
	
	Pack_LoginCL pack;
	pack.SetAccount("test2001");
	pack.SetPassword("123456");
	Client::Instance()->SendPacket(&pack);
}
