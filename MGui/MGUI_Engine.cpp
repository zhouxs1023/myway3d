#include "MGUI_Engine.h"

namespace Myway {

	MGUI_Engine * MGUI_Engine::msInstance = NULL;

	MGUI_Engine::MGUI_Engine()
	{
		msInstance = this;

		mHelper = new MGUI_Helper;

		mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("MGUI.ShaderLib", "Shaders\\MGUI.ShaderLib");
		d_assert (mShaderLib);

		mDefaultShader = new MGUI_ShaderDefault;

		mRect.x0 = 0;
		mRect.y0 = 0;
		mRect.x1 = Engine::Instance()->GetDeviceProperty()->Width;
		mRect.y1 = Engine::Instance()->GetDeviceProperty()->Height;

		mFont = new MGUI_Font;
	}

	MGUI_Engine::~MGUI_Engine()
	{
		RemoveAllLayout();
		RemoveAllSerializer();

		delete mDefaultShader;
		delete mHelper;
		delete mFont;

		msInstance = NULL;
	}

	// Layout
	void MGUI_Engine::AddLayout(MGUI_Layout * _layout)
	{
		mLayouts.PushBack(_layout);
	}

	void MGUI_Engine::RemoveLayout(MGUI_Layout * _layout)
	{
		for (int i = 0; i < mLayouts.Size(); ++i)
		{
			if (mLayouts[i] == _layout)
			{
				mLayouts.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	void MGUI_Engine::RemoveAllLayout(bool _delete)
	{
		if (_delete)
		{
			for (int i = 0; i < mLayouts.Size(); ++i)
				delete mLayouts[i];
		}
		
		mLayouts.Clear();
	}


	MGUI_Layout * MGUI_Engine::GetLayout(const TString128 & name)
	{
		for (int i = 0; i < mLayouts.Size(); ++i)
		{
			if (mLayouts[i]->GetName() == name)
				return mLayouts[i];
		}

		return NULL;
	}


	void MGUI_Engine::Resize()
	{
		mRect.x0 = 0;
		mRect.y0 = 0;
		mRect.x1 = Engine::Instance()->GetDeviceProperty()->Width;
		mRect.y1 = Engine::Instance()->GetDeviceProperty()->Height;
	}

	void MGUI_Engine::Update()
	{
		mInputManager.Update();

		for (int i = 0; i < mLayouts.Size(); ++i)
		{
			mLayouts[i]->Update();
		}
	}

	void MGUI_Engine::Render()
	{
		RenderState rs;
		rs.cullMode = CULL_NONE;
		rs.depthCheck = DCM_ALWAYS;
		rs.depthWrite = false;
		rs.blendMode = BM_ALPHA_BLEND;

		RenderSystem::Instance()->SetRenderState(rs);

		for (int i = 0; i < mLayouts.Size(); ++i)
		{
			if (mLayouts[i]->GetVisible())
			{
				mLayouts[i]->UpdateRenderItem();
				mLayouts[i]->DoRender();
			}
		}
	}

	void MGUI_Engine::AddSerializer(MGUI_Serializer * _loader)
	{
		d_assert (GetSerializer(_loader->GetTypeName().c_str()) == NULL);

		mSerializers.PushBack(_loader);
	}

	MGUI_Serializer * MGUI_Engine::GetSerializer(const char * _typeName)
	{
		for (int i = 0; i < mSerializers.Size(); ++i)
		{
			if (mSerializers[i]->GetTypeName() == _typeName)
				return mSerializers[i];
		}

		return NULL;
	}

	void MGUI_Engine::RemoveAllSerializer()
	{
		for (int i = 0; i < mSerializers.Size(); ++i)
		{
			delete mSerializers[i];
		}

		mSerializers.Clear();
	}

	MGUI_Widget * MGUI_Engine::GetWidget(int _x, int _y)
	{
		for (int i = 0; i < mLayouts.Size(); ++i)
		{
			MGUI_Widget * item = mLayouts[i]->Pick(_x, _y);

			if (item)
				return item;
		}

		return NULL;
	}
}