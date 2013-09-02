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
		mTextColor[MGUI_WidgetState::Selected] = Color4::White;
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
		
		rect.x1 = mUVRect[MGUI_WidgetState::Normal].DX();
		rect.y1 = mUVRect[MGUI_WidgetState::Normal].DY();

		_widget->SetRect(rect);
	}

	void MGUI_LookFeel::Affect(MGUI_Widget * _widget) const
	{
		if (mClientRect.DX() == 0 || mClientRect.DY() == 0)
			return ;

		int dx0 = mClientRect.x0;
		int dy0 = mClientRect.y0;

		int dx1 = mUVRect[MGUI_WidgetState::Normal].DX() - mClientRect.x1;
		int dy1 = mUVRect[MGUI_WidgetState::Normal].DY() - mClientRect.y1;

		d_assert (dx0 >= 0 && dx0 < mUVRect[MGUI_WidgetState::Normal].DX());
		d_assert (dx1 >= 0 && dx1 < mUVRect[MGUI_WidgetState::Normal].DX());
		d_assert (dy0 >= 0 && dy0 < mUVRect[MGUI_WidgetState::Normal].DY());
		d_assert (dy1 >= 0 && dy1 < mUVRect[MGUI_WidgetState::Normal].DY());

		const MGUI_Rect & rect = _widget->GetRect();

		MGUI_Rect client;

		client.x0 = dx0;
		client.y0 = dy0;

		client.x1 = rect.DX() - dx1;
		client.y1 = rect.DY() - dy1;

		client.x1 = Math::Maximum(client.x0, client.x1);
		client.y1 = Math::Maximum(client.y0, client.y1);

		if (rect.DX() < mClientRect.DX())
		{
			client.x0 = 0;
			client.x1 = rect.DX();
		}

		if (rect.DY() < mClientRect.DY())
		{
			client.y0 = 0;
			client.y1 = rect.DY();
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
		kClone->mClientRect = mClientRect;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			kClone->AddChild(mChildren[i]->Clone(mChildren[i]->GetName()));
		}

		return kClone;
	}





	MGUI_LookFeelManager * MGUI_LookFeelManager::msInstance = NULL;

	MGUI_LookFeelManager::MGUI_LookFeelManager()
	{
		msInstance = this;
	}

	MGUI_LookFeelManager::~MGUI_LookFeelManager()
	{
		msInstance = NULL;

		Unload();
	}

	void MGUI_LookFeelManager::Load(const char * source)
	{
		Unload();

	}

	void MGUI_LookFeelManager::Unload()
	{
		for (int i = 0; i < MGUI_WidgetType::Max; ++i)
		{
			for (int j = 0; j <  mLookFeelGroup[i].Size(); ++j)
			{
				delete mLookFeelGroup[i][j];
			}

			mLookFeelGroup[i].Clear();
		}

		mFilename = "";
	}

	void MGUI_LookFeelManager::AddLookFeel(int _type, MGUI_LookFeel * _lookfeel)
	{
		d_assert (GetLookFeel(_type, _lookfeel->GetName()) == NULL);

		mLookFeelGroup[_type].PushBack(_lookfeel);
	}

	void MGUI_LookFeelManager::RemoveLookFeel(int _type, MGUI_LookFeel * _lookfeel)
	{
		for (int i = 0; i < mLookFeelGroup[_type].Size(); ++i)
		{
			MGUI_LookFeel * k = mLookFeelGroup[_type][i];
			
			if (k == _lookfeel)
			{
				delete k;

				mLookFeelGroup[_type].Erase(i);

				return ;
			}
		}

		d_assert (0);
	}

	int MGUI_LookFeelManager::GetLookFeelCount(int _type)
	{
		return mLookFeelGroup[_type].Size();
	}

	MGUI_LookFeel * MGUI_LookFeelManager::GetLookFeel(int _type, int _index)
	{
		return mLookFeelGroup[_type][_index];
	}

	MGUI_LookFeel * MGUI_LookFeelManager::GetLookFeel(int _type, const TString128 & _name)
	{
		for (int i = 0; i < mLookFeelGroup[_type].Size(); ++i)
		{
			MGUI_LookFeel * k = mLookFeelGroup[_type][i];

			if (k->GetName() == _name)
				return k;
		}

		return NULL;
	}

}