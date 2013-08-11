#include "MGUI_LookFeel.h"
#include "MGUI_Engine.h"

namespace Myway {

	MGUI_LookFeel::MGUI_LookFeel(const TString128 & _name)
		: mName(_name)
		, mSkin(NULL)
		, mShader(NULL)
	{
		mShader = MGUI_Engine::Instance()->GetDefaultShader();

		mTextColor[MGUI_WidgetState::Disabled] = Color4::Gray;
		mTextColor[MGUI_WidgetState::Normal] = Color4::White;
		mTextColor[MGUI_WidgetState::Focused] = Color4::White;
		mTextColor[MGUI_WidgetState::Pressed] = Color4::White;
		mTextColor[MGUI_WidgetState::SelectedDisabled] = Color4::White;
		mTextColor[MGUI_WidgetState::SelectedNormal] = Color4::White;
		mTextColor[MGUI_WidgetState::SelectedFocused] = Color4::White;
		mTextColor[MGUI_WidgetState::SelectedPressed] = Color4::White;

		mColor[MGUI_WidgetState::Disabled] = Color4::White;
		mColor[MGUI_WidgetState::Normal] = Color4::White;
		mColor[MGUI_WidgetState::Focused] = Color4::White;
		mColor[MGUI_WidgetState::Pressed] = Color4::White;
		mColor[MGUI_WidgetState::SelectedDisabled] = Color4::White;
		mColor[MGUI_WidgetState::SelectedNormal] = Color4::White;
		mColor[MGUI_WidgetState::SelectedFocused] = Color4::White;
		mColor[MGUI_WidgetState::SelectedPressed] = Color4::White;
	}

	MGUI_LookFeel::~MGUI_LookFeel()
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			delete mChildren[i];
		}
	}

	void MGUI_LookFeel::SetSkin(const char * _source)
	{
		mSkin = VideoBufferManager::Instance()->Load2DTexture(_source, _source);
	}

	void MGUI_LookFeel::Init(MGUI_Widget * _widget) const
	{
		MGUI_Rect rect;
		
		rect.x1 = mUVRect[MGUI_WidgetState::Normal].Width();
		rect.y1 = mUVRect[MGUI_WidgetState::Normal].Height();

		_widget->SetRect(rect);
	}

	void MGUI_LookFeel::Affect(MGUI_Widget * _widget) const
	{
		int state = _widget->GetState();

		if (mClientRect.Width() == 0 || mClientRect.Height() == 0)
			return ;

		int dx0 = mClientRect.x0;
		int dy0 = mClientRect.y0;

		int dx1 = mUVRect[state].Width() - mClientRect.x1;
		int dy1 = mUVRect[state].Height() - mClientRect.y1;

		d_assert (dx0 >= 0 && dx0 < mUVRect[state].Width());
		d_assert (dx1 >= 0 && dx1 < mUVRect[state].Width());
		d_assert (dy0 >= 0 && dy0 < mUVRect[state].Height());
		d_assert (dy1 >= 0 && dy1 < mUVRect[state].Height());

		const MGUI_Rect & rect = _widget->GetRect();

		MGUI_Rect client;

		client.x0 = dx0;
		client.y0 = dy0;

		client.x1 = rect.Width() - dx1;
		client.y1 = rect.Height() - dy1;

		client.x1 = Math::Maximum(client.x0, client.x1);
		client.y1 = Math::Maximum(client.y0, client.y1);

		if (rect.Width() < mClientRect.Width())
		{
			client.x0 = 0;
			client.x1 = rect.Width();
		}

		if (rect.Height() < mClientRect.Height())
		{
			client.y0 = 0;
			client.y1 = rect.Height();
		}

		_widget->SetClientRect(client);
	}

	MGUI_Rect MGUI_LookFeel::GetUVClientRect(int _state) const
	{
		MGUI_Rect rect;

		rect.x0 = mUVRect[_state].x0 + mClientRect.x0;
		rect.x1 = mUVRect[_state].x0 + mClientRect.x1;
		rect.y0 = mUVRect[_state].y0 + mClientRect.y0;
		rect.y1 = mUVRect[_state].y0 + mClientRect.y1;

		return rect;
	}

	MGUI_LookFeel * MGUI_LookFeel::GetChild(const TString128 & _name)
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]->GetName() == _name)
				return mChildren[i];
		}

		return NULL;
	}

	const MGUI_LookFeel * MGUI_LookFeel::GetChild(const TString128 & _name) const
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]->GetName() == _name)
				return mChildren[i];
		}

		return NULL;
	}

	MGUI_LookFeel * MGUI_LookFeel::Clone(const TString128 & _name)
	{
		MGUI_LookFeel * kClone = new MGUI_LookFeel(_name);

		kClone->mSkin = mSkin;
		kClone->mShader = mShader;

		kClone->mUVRect = mUVRect;
		kClone->mTextColor = mTextColor;
		kClone->mColor = mColor;
		kClone->mClientRect = mClientRect;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			kClone->AddChild(mChildren[i]->Clone(mChildren[i]->GetName()));
		}

		return kClone;
	}

}