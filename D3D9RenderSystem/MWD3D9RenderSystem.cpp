#include "MWD3D9RenderSystem.h"
#include "MWRenderHelper.h"

#pragma warning(disable : 4312)

namespace Myway
{

D3D9RenderSystem::D3D9RenderSystem(void)
: mDirect3D(NULL),
  mD3DDevice(NULL),
  mLastFramePrimitiveCount(0),
  mLastFrameBatchCount(0),
  mPrimitivCount(0),
  mBatchCount(0),
  mVertexShader(0),
  mPixelShader(0),
  mVertexShaderParams(0),
  mPixelShaderParams(0)
{
    for (int i = 0; i < MAX_RENDER_TARGET; ++i)
        mRenderTarget[i] = NULL;

    mDepthStencil = NULL;
	mD3DQuery = NULL;
}

D3D9RenderSystem::~D3D9RenderSystem(void)
{
    safe_delete(mWindow);
    safe_release_com(mDirect3D);
}

void D3D9RenderSystem::Init()
{
    debug_assert(!mDirect3D, "Render has been Initialized!");

    mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!mDirect3D)
    {
        EXCEPTION("Create Direct3D9 failed.");
    }

    EnumDrives();

    mWindow = new D3D9RenderWindow(this);

	mD3DDevice->CreateQuery( D3DQUERYTYPE_OCCLUSION, &mD3DQuery);
}

void D3D9RenderSystem::_BeginEvent(const char * str)
{
	wchar_t eventName[1024];

	CharSet::AnsiToUnicode(eventName, 1024, str);

	D3DPERF_BeginEvent(D3DCOLOR_RGBA(0, 0, 0, 0xFF), eventName);
}

void D3D9RenderSystem::_EndEvent()
{
	D3DPERF_EndEvent();
}

void D3D9RenderSystem::BeginOcclusionQuery()
{
	HRESULT hr = mD3DQuery->Issue(D3DISSUE_BEGIN);

	d_assert (hr == D3D_OK);
}

int	D3D9RenderSystem::EndOcclusionQuery()
{
	HRESULT hr = mD3DQuery->Issue(D3DISSUE_END);

	d_assert (hr == D3D_OK);

	DWORD pixels = 0;

	do 
	{
		hr = mD3DQuery->GetData((void *) &pixels, sizeof(DWORD), D3DGETDATA_FLUSH);
	} while (hr == S_FALSE);

	return pixels;
}

void D3D9RenderSystem::OnResize()
{
    mWindow->OnResize();
}

bool D3D9RenderSystem::CheckMSAAFormat(FORMAT Format, MULTI_SAMPLE Msaa)
{
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    D3DMULTISAMPLE_TYPE D3DMsaa = D3D9Mapping::GetD3DMultiSampleType(Msaa);
    HRESULT hr = D3D_OK;

    const D3DPRESENT_PARAMETERS & param = mWindow->mPresentParam;
    DWORD adapter = mWindow->mAdapter;

    hr = mDirect3D->CheckDeviceMultiSampleType(adapter, D3DDEVTYPE_HAL, 
                                               D3DFormat, 
                                               param.Windowed, 
                                               D3DMsaa,
                                               NULL);
    return SUCCEEDED(hr);
}

bool D3D9RenderSystem::CheckTextureFormat(FORMAT Format, USAGE Usage, bool autoGenMimmap)
{
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);

	if (autoGenMimmap)
		D3DUsage |= D3DUSAGE_AUTOGENMIPMAP;

    const D3DPRESENT_PARAMETERS & param = mWindow->mPresentParam;

    HRESULT hr = mDirect3D->CheckDeviceFormat(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, param.BackBufferFormat,
        D3DUsage, D3DRTYPE_TEXTURE, D3DFormat);

    return SUCCEEDED(hr);
}

bool D3D9RenderSystem::CheckRenderTargetFormat(FORMAT Format)
{
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);

    const D3DPRESENT_PARAMETERS & param = mWindow->mPresentParam;

    HRESULT hr = mDirect3D->CheckDeviceFormat(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, param.BackBufferFormat,
        D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFormat);

    return SUCCEEDED(hr);
}


void D3D9RenderSystem::ClearBuffer(const Rect * rcRange, bool bColor, bool bDepth, bool bStencil, const Color & c, float z, int s)
{
    HRESULT hr;

    DWORD Flags = 0;
    D3DCOLOR cVal = D3DCOLOR_ARGB(c.a, c.r, c.g, c.b);
    float zVal = z;
    DWORD sVal = (DWORD)s;

    if (bColor)
        Flags |= D3DCLEAR_TARGET;
    if (bDepth)
        Flags |= D3DCLEAR_ZBUFFER;
    if (bStencil)
        Flags |= D3DCLEAR_STENCIL;

    if (rcRange)
    {
        D3DRECT rc;
        rc.x1 = rcRange->x1;
        rc.y1 = rcRange->y1;
        rc.x2 = rcRange->x2;
        rc.y2 = rcRange->y2;

        hr = mD3DDevice->Clear(0, &rc, Flags, cVal, zVal, sVal);
    }
    else
    {
        hr = mD3DDevice->Clear(0, NULL, Flags, cVal, zVal, sVal);
    }

    EXCEPTION_DEBUG(hr == D3D_OK, "Clear buffer failed.");
}

void D3D9RenderSystem::BeginScene()
{
    HRESULT hr;

    mPrimitivCount = 0;
    mBatchCount = 0;

	mVertexShader = NULL;
	mPixelShader = NULL;

    hr = mD3DDevice->BeginScene();

    EXCEPTION_DEBUG(hr == D3D_OK, "Begin scene failed.");

    SetRenderTarget(0, mWindow->GetRenderTarget());
    SetRenderTarget(1, NULL);
    SetRenderTarget(2, NULL);
    SetRenderTarget(3, NULL);

    SetDepthStencil(mWindow->GetDepthStencil());
}

void D3D9RenderSystem::EndScene()
{
    HRESULT hr;

    hr = mD3DDevice->EndScene();

    EXCEPTION_DEBUG(hr == D3D_OK, "End scene failed.");    
    
    mLastFramePrimitiveCount = mPrimitivCount;
    mLastFrameBatchCount = mBatchCount;

    mWindow->Present();
}

void D3D9RenderSystem::SetWorldTransform(const Mat4 & form)
{
    RenderRegister::Instance()->SetWorldMatrix(form);
}

void D3D9RenderSystem::SetViewTransform(const Mat4 & form)
{
    RenderRegister::Instance()->SetViewMatrix(form);
}

void D3D9RenderSystem::SetProjTransform(const Mat4 & form)
{
    RenderRegister::Instance()->SetProjMatrix(form);
}

void D3D9RenderSystem::SetBlendTransform(const Mat4 * forms, int count)
{
    RenderRegister::Instance()->SetBlendMatrix(forms, count);
}

void D3D9RenderSystem::SetRenderState(const RenderState & state)
{
	DWORD cullMode = state.cullMode;

	if (mFlipCullMode)
	{
		if (cullMode == D3DCULL_CCW)
			cullMode = D3DCULL_CW;
		else if (cullMode == D3DCULL_CW)
			cullMode = D3DCULL_CCW;
	}

    mD3DDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
    mD3DDevice->SetRenderState(D3DRS_FILLMODE, state.fillMode);

    mD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, state.colorWrite);

    mD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, state.depthWrite);

    switch (state.depthCheck)
    {
    case DCM_LESS_EQUAL:
        mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        break;

    case DCM_LESS:
        mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
        break;

    case DCM_GREATER_EQUAL:
        mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
        break;

    case DCM_GREATER:
        mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
        break;

    case DCM_EQUAL:
        mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
        break;

    case DCM_ALWAYS:
        mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
        break;

	case DCM_NONE:
		mD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		break;
    }

    switch (state.blendMode)
    {
    case BM_OPATICY:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        break;

    case BM_ALPHA_BLEND:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        break;

    case BM_ALPHA_TEST:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        mD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(state.alphaTestRef * 255));
        break;

    case BM_ADD:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        break;

    case BM_MULTIPLY:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        break;

    case BM_ALPHA_TEST_ADD:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        mD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(state.alphaTestRef * 255));
        break;

    case BM_ALPHA_TEST_MUL:
        mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
        mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        mD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        mD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        mD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        mD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(state.alphaTestRef * 255));
        break;
    }
}

//void D3D9RenderSystem::SetDepthBias(float bias, float slope)
//{
//    mD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, DWORD_BIT(bias));
//    mD3DDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, DWORD_BIT(slope));
//}

void D3D9RenderSystem::SetFog(const Vec4 & param, const Color4 & color)
{
    RenderRegister::Instance()->SetFogParam(param);
    RenderRegister::Instance()->SetFogColor(color);
}


void D3D9RenderSystem::SetSamplerState(int stage, const SamplerState & state)
{
    DWORD Sampler = (DWORD)stage;
    DWORD Filter = D3D9Mapping::GetD3DFilterType(state.Filter);
    DWORD Address = D3D9Mapping::GetD3DAddressMode(state.Address);
    DWORD BorderColor = D3DCOLOR_ARGB(state.BorderColor.a, state.BorderColor.r, state.BorderColor.g, state.BorderColor.b);
    DWORD MaxMipLevel = (DWORD)state.MaxMipLevel;

    //filter mode
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_MAGFILTER, Filter);
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_MINFILTER, Filter);
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_MIPFILTER, Filter);

    //address mode
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_ADDRESSU, Address);
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_ADDRESSV, Address);
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_ADDRESSW, Address);

    //max mip level
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_MAXMIPLEVEL, MaxMipLevel);

    //border color
    mD3DDevice->SetSamplerState(Sampler, D3DSAMP_BORDERCOLOR, BorderColor);
}

void D3D9RenderSystem::SetMaterial(const Color4 & ambient,
                                   const Color4 & emissive,
                                   const Color4 & diffuse,
                                   const Color4 & specular,
                                   float power)
{
    mRegister.SetMaterial(ambient, emissive, diffuse, specular, power);
}

void D3D9RenderSystem::SetLight(int index, Light * light)
{
    //////////////////////////////////////////////////////////////////////////
    //
    //
    //  directional lighting
    //      light_pos.w             <-  0
    //      light_dir.w             <-  1
    //      light_atten.x           <-  -1          // max attenuation distance
    //      light_atten.y           <-  1           // atten0
    //      light_atten.z           <-  0           // atten1
    //      light_atten.w           <-  0           // atten1
    //      light_spot_params.x     <-  1           // cosInner
    //      light_spot_params.y     <-  -2          // cosOuter
    //      light_spot_params.z     <-  0           // falloff

    //
    //  point lighting
    //      light_pos.w             <-  1
    //      light_dir.w             <-  0
    //      light_spot_params.x     <-  1           // cosInner
    //      light_spot_params.y     <-  -2          // cosOuter
    //      light_spot_params.z     <-  0           // falloff

    //
    //  spot lighting
    //      light_pos.w             <-  0
    //      light_dir.w             <-  1
    //
    //
    //
    Vec4 position, direction, attenuation, spot;
    LIGHT_TYPE type = light->GetType();

    if (type == LT_DIRECTIONAL)
    {
        position = Vec4(0, 0, 0, 0);
        direction = Vec4(light->GetDirection(), 1);
        attenuation = Vec4(FLT_MAX, 1, 0, 0);
        spot = Vec4(1, -2, 0, 1);
    }
    else if (type == LT_POINT)
    {
        position = Vec4(light->GetPosition(), 1);
        direction = Vec4(0, 0, 0, 0);
        attenuation = Vec4(light->GetRange(),
                           light->GetAttenuation0(),
                           light->GetAttenuation1(),
                           light->GetAttenuation2());
        spot = Vec4(1, -2, 0, 1);
    }
    else
    {
        position = Vec4(light->GetPosition(), 0);
        direction = Vec4(light->GetDirection(), 1);
        attenuation = Vec4(light->GetRange(),
                           light->GetAttenuation0(),
                           light->GetAttenuation1(),
                           light->GetAttenuation2());
        spot = Vec4(light->GetInner(),
                    light->GetOuter(),
                    light->GetFalloff(), 1);
    }

    mRegister.SetLightDiffuse(index, light->GetDiffsue());
    mRegister.SetLightSpecular(index, light->GetSpecular());
    mRegister.SetLightPosition(index, position);
    mRegister.SetLightDirection(index, direction);
    mRegister.SetLightAttenuation(index, attenuation.x, attenuation.y, attenuation.z, attenuation.w);
    mRegister.SetLightSpotParams(index, spot.x, spot.y, spot.z);
}

void D3D9RenderSystem::SetCamera(Camera * cam)
{
    // view project transform
    if (cam)
    {
        Vec4 pos(cam->GetPosition(), 1);

        mRegister.SetCameraPosition(pos);
        mRegister.SetViewMatrix(cam->GetViewMatrix());
        mRegister.SetProjMatrix(cam->GetProjMatrix());
    }
    else
    {
        mRegister.SetCameraPosition(Vec4::Zero);
        mRegister.SetViewMatrix(Mat4::Identity);
        mRegister.SetProjMatrix(Mat4::Identity);
    }
}

void D3D9RenderSystem::SetVertexShader(ShaderProgram * pVertexShader)
{
    assert (pVertexShader && ShaderProgram::IsVertexShader(pVertexShader->GetProfile()));

	if (pVertexShader == mVertexShader)
		return ;

    HRESULT hr = D3D_OK;
    D3D9VertexShader * shader = static_cast<D3D9VertexShader*>(pVertexShader);
    hr = mD3DDevice->SetVertexShader(shader->GetShader());
    EXCEPTION_DEBUG(hr == D3D_OK, "Set vertex shader failed.");

    mVertexShader = pVertexShader;
}

void D3D9RenderSystem::SetPixelShader(ShaderProgram * pPixelShader)
{
    assert (pPixelShader && ShaderProgram::IsPixelShader(pPixelShader->GetProfile()));

	if (pPixelShader == mPixelShader)
		return ;

    HRESULT hr = D3D_OK;
    D3D9PixelShader * shader = static_cast<D3D9PixelShader*>(pPixelShader);
    hr = mD3DDevice->SetPixelShader(shader->GetShader());
    EXCEPTION_DEBUG(hr == D3D_OK, "Set pixel shader failed.");

    mPixelShader = pPixelShader;
}

void D3D9RenderSystem::SetVertexShaderParam(ShaderParamTable * table)
{
    mVertexShaderParams = table;
}

void D3D9RenderSystem::SetPixelShaderParam(ShaderParamTable * table)
{
    mPixelShaderParams = table;
}

void D3D9RenderSystem::SetVertexShaderConstF(int reg, const float * data, int vec4Count)
{
     mD3DDevice->SetVertexShaderConstantF(reg, data, vec4Count);
}

void D3D9RenderSystem::SetVertexShaderConstI(int reg, const int * data, int vec4Count)
{
    mD3DDevice->SetVertexShaderConstantI(reg, data, vec4Count);
}

void D3D9RenderSystem::SetPixelShaderConstF(int reg, const float * data, int vec4Count)
{
    mD3DDevice->SetPixelShaderConstantF(reg, data, vec4Count);
}

void D3D9RenderSystem::SetPixelShaderConstI(int reg, const int * data, int vec4Count)
{
    mD3DDevice->SetPixelShaderConstantI(reg, data, vec4Count);
}

void D3D9RenderSystem::SetTexture(int index, const SamplerState & state, Texture * pTexture)
{
    if (pTexture)
    {
        TEXTURE_TYPE type = pTexture->GetTextureType();

        IDirect3DBaseTexture9 * d3d9Tex = NULL;

        if (type == TEXTYPE_2D)
        {
            D3D9Texture * p = (D3D9Texture *)pTexture;
            d3d9Tex = p->GetD3DTexture();
        }
        else if (type == TEXTYPE_CUBE)
        {
            D3D9CubeTexture * p = (D3D9CubeTexture *)pTexture;
            d3d9Tex = p->GetD3D9Texture();
        }
        else if (type == TEXTYPE_3D)
        {
            D3D9VolumeTexture * p = (D3D9VolumeTexture *)pTexture;
            d3d9Tex = p->GetD3D9Texture();
        }

		SetSamplerState(index, state);

        mD3DDevice->SetTexture(index, d3d9Tex);

        Vec4 size;

        size.x = (float)pTexture->GetWidth();
        size.y = (float)pTexture->GetHeight();
        size.z = (float)pTexture->GetDepth();
        size.w = 1.0f;

        RenderRegister::Instance()->SetTextureSize(index, size);
    }
    else
    {
        mD3DDevice->SetTexture(index, NULL);

        Vec4 size;

		size.x = 1.0f;
		size.y = 1.0f;
		size.z = 1.0f;
		size.w = 1.0f;

		RenderRegister::Instance()->SetTextureSize(index, size);
	}
}

void D3D9RenderSystem::Render(Technique * tech, Renderer * obj)
{
    RenderRegister::Instance()->Reset();

    Material * mtl = obj->GetMaterial();

    // world transform
    obj->GetWorldTransform(&mMatWorld);
    mNumBlends = obj->GetBlendMatrix(mMatBlend);

    SetWorldTransform(mMatWorld);
    SetBlendTransform(mMatBlend, mNumBlends);

    SetMaterial(mtl->GetAmbient(),
                mtl->GetEmissive(),
                mtl->GetDiffuse(),
                mtl->GetSpecular(),
                mtl->GetSpecularPower());

    SetRenderState(mtl->GetRenderState());

    for (int i = 0; i < MAX_SAMPLER_STATE; ++i)
    {
        SamplerState & stage = tech->GetSamplerState(i);
        Texture * texture = RenderHelper::Instance()->GetTexture(mtl, stage.BindType);

        SetTexture(i, stage, texture);
    }

    SetVertexShader(tech->GetVertexShader());
    SetPixelShader(tech->GetPixelShader());

    ShaderParamTable * pVertexShaderParam = tech->GetVertexShaderParamTable();
    ShaderParamTable * pPixelShaderParam = tech->GetPixelShaderParamTable();
    SetVertexShaderParam(pVertexShaderParam);
    SetPixelShaderParam(pPixelShaderParam);

    HRESULT hr = D3D_OK;

    RenderRegister::Instance()->Begin();

    mVertexShaderParams->Update();
    mPixelShaderParams->Update();

    //set shader register
    if (mVertexShaderParams)
    {
        int count = mVertexShaderParams->GetNumParam();
        ShaderParam * param = NULL;

        for (int i = 0; i < count; ++i)
        {
            param = mVertexShaderParams->GetParam(i);
            DWORD idx = param->GetRegister();
            DWORD v4c = param->GetVec4Count();

            if (param->IsInt())
            {
                hr = mD3DDevice->SetVertexShaderConstantI(idx, (const int *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantI error.");
            }
            else
            {
                hr = mD3DDevice->SetVertexShaderConstantF(idx, (const float *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantF error");
            }

        }
    }

    if (mPixelShaderParams)
    {
        int count = mPixelShaderParams->GetNumParam();
        ShaderParam * param = NULL;

        for (int i = 0; i < count; ++i)
        {
            param = mPixelShaderParams->GetParam(i);
            DWORD idx = param->GetRegister();
            DWORD v4c = param->GetVec4Count();

            if (param->IsInt())
            {
                hr = mD3DDevice->SetPixelShaderConstantI(idx, (const int *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantI error.");
            }
            else
            {
                hr = mD3DDevice->SetPixelShaderConstantF(idx, (const float *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantF error");
            }
        }
    }

    // draw
    VertexStream * vstream = obj->GetVertexStream();
    IndexStream * istream = obj->GetIndexStream();

    VertexDeclarationPtr decl = vstream->GetDeclaration();
    D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)decl.c_ptr();

	d_assert(d3dvd->GetD3DVertexDeclaration());

    hr = mD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());

    for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
    {
        int stride = vstream->GetStreamStride(i);
		int instances = vstream->GetStreamInstance(i);
        VertexBufferPtr vb = vstream->GetStream(i);

        if (stride && vb.NotNull())
        {
            D3D9VertexBuffer * d3dvb = (D3D9VertexBuffer*)vb.c_ptr();
            hr = mD3DDevice->SetStreamSource(i, d3dvb->GetD3DVertexBuffer(), 0, stride);
        }
        else
        {
            hr = mD3DDevice->SetStreamSource(i, NULL, 0, 0);
        }

		if (instances > 1)
		{
			mD3DDevice->SetStreamSourceFreq(i, D3DSTREAMSOURCE_INDEXEDDATA | instances);
		}
		else if (instances == 1)
		{
			mD3DDevice->SetStreamSourceFreq(i, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		}
		else
		{
			mD3DDevice->SetStreamSourceFreq(i, 1);
		}

		D3DErrorExceptionFunction(SetStreamSource, hr);
	}


	IndexBufferPtr ib = istream->GetStream();

	int startVertex = vstream->GetStart();
	int vertexCount = vstream->GetCount();
	int startIndex = istream->GetStart();
	int primCount = obj->GetPrimitiveCount();
	D3DPRIMITIVETYPE primType = (D3DPRIMITIVETYPE)obj->GetPrimitiveType();

	if (ib.NotNull())
	{
		D3D9IndexBuffer * d3dib = (D3D9IndexBuffer*)(ib.c_ptr());

		hr = mD3DDevice->SetIndices(d3dib->GetD3DIndexBuffer());
		hr = mD3DDevice->DrawIndexedPrimitive(primType, 0, startVertex, vertexCount, startIndex, primCount);

		D3DErrorExceptionFunction(DrawIndexedPrimitive, hr);
	}
    else
    {
         hr = mD3DDevice->DrawPrimitive(primType, startVertex, primCount);

         D3DErrorExceptionFunction(DrawPrimitive, hr);
    }

	RenderRegister::Instance()->End();

	mBatchCount += 1;
	mPrimitivCount += primCount;
}

void D3D9RenderSystem::RenderUp(Technique * tech, RenderOp * rop, const void * vertices, int stride, int numVerts, const void * indices, bool bIndex32)
{
	RenderRegister::Instance()->Reset();

	// world transform
	mMatWorld = rop->xform;
	mNumBlends = 0;

	SetWorldTransform(mMatWorld);
	SetBlendTransform(mMatBlend, mNumBlends);

	SetRenderState(rop->rState);

	SetVertexShader(tech->GetVertexShader());
	SetPixelShader(tech->GetPixelShader());

	ShaderParamTable * pVertexShaderParam = tech->GetVertexShaderParamTable();
	ShaderParamTable * pPixelShaderParam = tech->GetPixelShaderParamTable();
	SetVertexShaderParam(pVertexShaderParam);
	SetPixelShaderParam(pPixelShaderParam);

	HRESULT hr = D3D_OK;

	RenderRegister::Instance()->Begin();

	mVertexShaderParams->Update();
	mPixelShaderParams->Update();

	//set shader register
	if (mVertexShaderParams)
	{
		int count = mVertexShaderParams->GetNumParam();
		ShaderParam * param = NULL;

		for (int i = 0; i < count; ++i)
		{
			param = mVertexShaderParams->GetParam(i);
			DWORD idx = param->GetRegister();
			DWORD v4c = param->GetVec4Count();

			if (param->IsInt())
			{
				hr = mD3DDevice->SetVertexShaderConstantI(idx, (const int *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantI error.");
			}
			else
			{
				hr = mD3DDevice->SetVertexShaderConstantF(idx, (const float *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantF error");
			}

		}
	}

	if (mPixelShaderParams)
	{
		int count = mPixelShaderParams->GetNumParam();
		ShaderParam * param = NULL;

		for (int i = 0; i < count; ++i)
		{
			param = mPixelShaderParams->GetParam(i);
			DWORD idx = param->GetRegister();
			DWORD v4c = param->GetVec4Count();

			if (param->IsInt())
			{
				hr = mD3DDevice->SetPixelShaderConstantI(idx, (const int *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantI error.");
			}
			else
			{
				hr = mD3DDevice->SetPixelShaderConstantF(idx, (const float *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantF error");
			}
		}
	}

	for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
	{
		mD3DDevice->SetStreamSource(i, NULL, 0, 0);
		mD3DDevice->SetStreamSourceFreq(i, 1);
	}

	mD3DDevice->SetIndices(NULL);

	// draw
	VertexStream * vstream = &rop->vxStream;
	IndexStream * istream = &rop->ixStream;

	VertexDeclarationPtr decl = vstream->GetDeclaration();

	D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)decl.c_ptr();

	d_assert(d3dvd->GetD3DVertexDeclaration());

	mD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
	
	int primCount = rop->iPrimCount;
	D3DPRIMITIVETYPE primType = (D3DPRIMITIVETYPE)rop->ePrimType;
	D3DFORMAT indexFmt = bIndex32 ? D3DFMT_INDEX32 : D3DFMT_INDEX16;

	hr = mD3DDevice->DrawIndexedPrimitiveUP(primType, 0, numVerts, primCount, indices, indexFmt, vertices, stride);

	EXCEPTION_DEBUG(SUCCEEDED(hr), "DrawIndexedPrimitiveUP error");

	RenderRegister::Instance()->End();

	mBatchCount += 1;
	mPrimitivCount += primCount;
}

void D3D9RenderSystem::RenderUI(Technique * tech, VertexDeclarationPtr decl, VertexBufferPtr vb, int primCount)
{
	SetVertexShader(tech->GetVertexShader());
	SetPixelShader(tech->GetPixelShader());

	ShaderParamTable * pVertexShaderParam = tech->GetVertexShaderParamTable();
	ShaderParamTable * pPixelShaderParam = tech->GetPixelShaderParamTable();
	SetVertexShaderParam(pVertexShaderParam);
	SetPixelShaderParam(pPixelShaderParam);

	HRESULT hr = D3D_OK;

	//set shader register
	if (mVertexShaderParams)
	{
		int count = mVertexShaderParams->GetNumParam();
		ShaderParam * param = NULL;

		for (int i = 0; i < count; ++i)
		{
			param = mVertexShaderParams->GetParam(i);
			DWORD idx = param->GetRegister();
			DWORD v4c = param->GetVec4Count();

			if (param->IsInt())
			{
				hr = mD3DDevice->SetVertexShaderConstantI(idx, (const int *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantI error.");
			}
			else
			{
				hr = mD3DDevice->SetVertexShaderConstantF(idx, (const float *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantF error");
			}

		}
	}

	if (mPixelShaderParams)
	{
		int count = mPixelShaderParams->GetNumParam();
		ShaderParam * param = NULL;

		for (int i = 0; i < count; ++i)
		{
			param = mPixelShaderParams->GetParam(i);
			DWORD idx = param->GetRegister();
			DWORD v4c = param->GetVec4Count();

			if (param->IsInt())
			{
				hr = mD3DDevice->SetPixelShaderConstantI(idx, (const int *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantI error.");
			}
			else
			{
				hr = mD3DDevice->SetPixelShaderConstantF(idx, (const float *)param->GetData(), v4c);
				EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantF error");
			}
		}
	}

	D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)decl.c_ptr();

	d_assert(d3dvd->GetD3DVertexDeclaration());

	int stride = decl->GetStreamSize(0);

	hr = mD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());

	D3D9VertexBuffer * d3dvb = (D3D9VertexBuffer*)vb.c_ptr();

	hr = mD3DDevice->SetStreamSource(0, d3dvb->GetD3DVertexBuffer(), 0, stride);

	mD3DDevice->SetStreamSourceFreq(0, 1);

	D3DErrorExceptionFunction(SetStreamSource, hr);

	{
		hr = mD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, primCount);

		D3DErrorExceptionFunction(DrawPrimitive, hr);
	}

	mBatchCount += 1;
	mPrimitivCount += primCount;
}

void D3D9RenderSystem::SetSMAAType(eSmaaType::enum_t type)
{
	mWindow->SetSMAAType(type);
}

eSmaaType::enum_t D3D9RenderSystem::GetSMAAType()
{
	return mWindow->GetSMAAType();
}

void D3D9RenderSystem::DoSMAA(RenderTarget * rt, Texture * colorTex)
{
	mWindow->DoSMAA(rt, colorTex);
}

void D3D9RenderSystem::Render(Technique * tech, RenderOp * rd)
{
    RenderRegister::Instance()->Reset();

    // world transform
    mMatWorld = rd->xform;
    mNumBlends = 0;

    SetWorldTransform(mMatWorld);
    SetBlendTransform(mMatBlend, mNumBlends);

    SetRenderState(rd->rState);

    SetVertexShader(tech->GetVertexShader());
    SetPixelShader(tech->GetPixelShader());

    ShaderParamTable * pVertexShaderParam = tech->GetVertexShaderParamTable();
    ShaderParamTable * pPixelShaderParam = tech->GetPixelShaderParamTable();
    SetVertexShaderParam(pVertexShaderParam);
    SetPixelShaderParam(pPixelShaderParam);

    HRESULT hr = D3D_OK;

    RenderRegister::Instance()->Begin();

    mVertexShaderParams->Update();
    mPixelShaderParams->Update();

    //set shader register
    if (mVertexShaderParams)
    {
        int count = mVertexShaderParams->GetNumParam();
        ShaderParam * param = NULL;

        for (int i = 0; i < count; ++i)
        {
            param = mVertexShaderParams->GetParam(i);
            DWORD idx = param->GetRegister();
            DWORD v4c = param->GetVec4Count();

            if (param->IsInt())
            {
                hr = mD3DDevice->SetVertexShaderConstantI(idx, (const int *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantI error.");
            }
            else
            {
                hr = mD3DDevice->SetVertexShaderConstantF(idx, (const float *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetVertexShaderConstantF error");
            }

        }
    }

    if (mPixelShaderParams)
    {
        int count = mPixelShaderParams->GetNumParam();
        ShaderParam * param = NULL;

        for (int i = 0; i < count; ++i)
        {
            param = mPixelShaderParams->GetParam(i);
            DWORD idx = param->GetRegister();
            DWORD v4c = param->GetVec4Count();

            if (param->IsInt())
            {
                hr = mD3DDevice->SetPixelShaderConstantI(idx, (const int *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantI error.");
            }
            else
            {
                hr = mD3DDevice->SetPixelShaderConstantF(idx, (const float *)param->GetData(), v4c);
                EXCEPTION_DEBUG(SUCCEEDED(hr), "SetPixelShaderConstantF error");
            }
        }
    }

	// draw
	VertexStream * vstream = &rd->vxStream;
	IndexStream * istream = &rd->ixStream;

	VertexDeclarationPtr decl = vstream->GetDeclaration();
	D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)decl.c_ptr();

	d_assert(d3dvd->GetD3DVertexDeclaration());

	hr = mD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());

	for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
	{
		int stride = vstream->GetStreamStride(i);
		int instances = vstream->GetStreamInstance(i);
		VertexBufferPtr vb = vstream->GetStream(i);

		if (stride && vb.NotNull())
		{
			D3D9VertexBuffer * d3dvb = (D3D9VertexBuffer*)vb.c_ptr();
			hr = mD3DDevice->SetStreamSource(i, d3dvb->GetD3DVertexBuffer(), 0, stride);
		}
		else
		{
			hr = mD3DDevice->SetStreamSource(i, NULL, 0, 0);
		}

		if (instances > 1)
		{
			mD3DDevice->SetStreamSourceFreq(i, D3DSTREAMSOURCE_INDEXEDDATA | instances);
		}
		else if (instances == 1)
		{
			mD3DDevice->SetStreamSourceFreq(i, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		}
		else
		{
			mD3DDevice->SetStreamSourceFreq(i, 1);
		}

		D3DErrorExceptionFunction(SetStreamSource, hr);
	}


	IndexBufferPtr ib = istream->GetStream();

	int startVertex = vstream->GetStart();
	int vertexCount = vstream->GetCount();
	int startIndex = istream->GetStart();
	int primCount = rd->iPrimCount;
	D3DPRIMITIVETYPE primType = (D3DPRIMITIVETYPE)rd->ePrimType;

	if (ib.NotNull())
	{
		D3D9IndexBuffer * d3dib = (D3D9IndexBuffer*)(ib.c_ptr());

		hr = mD3DDevice->SetIndices(d3dib->GetD3DIndexBuffer());
		hr = mD3DDevice->DrawIndexedPrimitive(primType, 0, startVertex, vertexCount, startIndex, primCount);

		D3DErrorExceptionFunction(DrawIndexedPrimitive, hr);
	}
	else
	{
		hr = mD3DDevice->DrawPrimitive(primType, startVertex, primCount);

		D3DErrorExceptionFunction(DrawPrimitive, hr);
	}

	RenderRegister::Instance()->End();

	mBatchCount += 1;
	mPrimitivCount += primCount;
}


void D3D9RenderSystem::SetRenderTarget(int index, RenderTarget * target)
{
    if (target != NULL)
    {
        HRESULT hr = D3D_OK;
        D3D9RenderTarget * t = static_cast<D3D9RenderTarget*>(target);
        IDirect3DSurface9 * target = t->mRenderTarget;

        hr = mD3DDevice->SetRenderTarget(index, target);
        EXCEPTION_DEBUG(hr == D3D_OK, "set render target failed.");
    }
    else
    {
        mD3DDevice->SetRenderTarget(index, NULL);
    }

    mRenderTarget[index] = target;
}

void D3D9RenderSystem::SetDepthStencil(DepthStencil * depth)
{
    HRESULT hr = D3D_OK;

    if (depth != NULL)
    {
        D3D9DepthStencil * d = static_cast<D3D9DepthStencil*>(depth);
        IDirect3DSurface9 * surface = d->mDepthStencil;

        hr = mD3DDevice->SetDepthStencilSurface(surface);
    }
    else
    {
        hr = mD3DDevice->SetDepthStencilSurface(NULL);
    }
    
    EXCEPTION_DEBUG(hr == D3D_OK, "set render target failed.");

    mDepthStencil = depth;
}

RenderTarget * D3D9RenderSystem::GetRenderTarget(int index)
{
    return mRenderTarget[index];
}

DepthStencil * D3D9RenderSystem::GetDepthStencil()
{
    return mDepthStencil;
}

void D3D9RenderSystem::SetViewport(const RectF * vp)
{
    /*HRESULT hr = D3D_OK;

    D3DVIEWPORT9 d3dvp;

    if (vp)
    {
        d3dvp.X = vp->GetAbsoluteX();
        d3dvp.Y = vp->GetAbsoluteY();
        d3dvp.Width = vp->GetAbsoluteWidth();
        d3dvp.Height = vp->GetAbsoluteHeight();
        d3dvp.MinZ = 0.0f;
        d3dvp.MaxZ = 1.0f;
    }
    else
    {
        d_assert (mRenderTarget[0]);
        d3dvp.X = 0;
        d3dvp.Y = 0;
        d3dvp.Width = mRenderTarget[0]->GetWidth();
        d3dvp.Height = mRenderTarget[0]->GetHeight();
        d3dvp.MinZ = 0.0f;
        d3dvp.MaxZ = 1.0f;
    }
    
    hr = mD3DDevice->SetViewport(&d3dvp);

    d_assert (hr == S_OK);*/
}

void D3D9RenderSystem::SetClipPlane(int index, const Plane & plane)
{
    HRESULT hr = mD3DDevice->SetClipPlane(index, (const float *)&plane);
    D3DErrorExceptionFunction(SetClipPlane, hr);
}

void D3D9RenderSystem::SetScissorRect(const Rect & rect)
{
    RECT rc;
    rc.left = rect.x1;
    rc.top = rect.y1;
    rc.right = rect.x2;
    rc.bottom = rect.y2;

    mD3DDevice->SetScissorRect(&rc);
}

void D3D9RenderSystem::EnableClipPlane(int index, bool enable)
{
    DWORD val = enable ? (1 << index) : 0;

    mD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, val);
}

void D3D9RenderSystem::EnableScissorTest(bool enable)
{
    mD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, enable);
}

void D3D9RenderSystem::SetTime(float time)
{
    RenderRegister::Instance()->SetTime(time);
}

void D3D9RenderSystem::OutputRenderState()
{
    DWORD FillMode;
    DWORD ShadeMode;
    DWORD CullMode;

    DWORD ColorWrite;

    DWORD ZWrite;
    DWORD ZCheck;
    DWORD ZFunc;

    DWORD AlphaBlend;
    DWORD SrcBlend;
    DWORD DestBlend;
    DWORD BlendOp;

    DWORD AlphaTest;
    DWORD TestFunc;
    DWORD TestRef;

    mD3DDevice->GetRenderState(D3DRS_FILLMODE, &FillMode);
    mD3DDevice->GetRenderState(D3DRS_SHADEMODE, &ShadeMode);
    mD3DDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);

    mD3DDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &ColorWrite);

    mD3DDevice->GetRenderState(D3DRS_ZWRITEENABLE, &ZWrite);
    mD3DDevice->GetRenderState(D3DRS_ZENABLE, &ZCheck);
    mD3DDevice->GetRenderState(D3DRS_ZFUNC, &ZFunc);

    mD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &AlphaBlend);
    mD3DDevice->GetRenderState(D3DRS_SRCBLEND, &SrcBlend);
    mD3DDevice->GetRenderState(D3DRS_DESTBLEND, &DestBlend);
    mD3DDevice->GetRenderState(D3DRS_BLENDOP, &BlendOp);

    mD3DDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &AlphaTest);
    mD3DDevice->GetRenderState(D3DRS_ALPHAFUNC, &TestFunc);
    mD3DDevice->GetRenderState(D3DRS_ALPHAREF, &TestRef);

    debug_printf("-------------render state----------------\n");

    debug_printf("      file mode:      ");
    switch (FillMode)
    {
    case D3DFILL_SOLID:
        debug_printf("solid");
        break;

    case D3DFILL_WIREFRAME:
        debug_printf("frame");
        break;

    case D3DFILL_POINT:
        debug_printf("point");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      shade mode:     ");
    switch (ShadeMode)
    {
    case D3DSHADE_FLAT:
        debug_printf("flat");
        break;

    case D3DSHADE_GOURAUD:
        debug_printf("gouraud");
        break;

    case D3DSHADE_PHONG:
        debug_printf("phong");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      cull mode:      ");
    switch (CullMode)
    {
    case D3DCULL_CCW:
        debug_printf("ccw");
        break;

    case D3DCULL_CW:
        debug_printf("cw");
        break;

    case D3DCULL_NONE:
        debug_printf("none");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");


    debug_printf("      color write:    ");
    if (ColorWrite & D3DCOLORWRITEENABLE_RED)
    {
        debug_printf("r ");
    }
    if (ColorWrite & D3DCOLORWRITEENABLE_GREEN)
    {
        debug_printf("g ");
    }
    if (ColorWrite & D3DCOLORWRITEENABLE_BLUE)
    {
        debug_printf("b ");
    }
    if (ColorWrite & D3DCOLORWRITEENABLE_ALPHA)
    {
        debug_printf("a ");
    }
    debug_printf("\n");

    debug_printf("      z write:        ");
    switch (ZWrite)
    {
    case TRUE:
        debug_printf("true");
        break;

    case FALSE:
        debug_printf("false");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      z check:        ");
    switch (ZCheck)
    {
    case TRUE:
        debug_printf("true");
        break;

    case FALSE:
        debug_printf("false");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      z func:         ");
    switch (ZFunc)
    {
    case D3DCMP_NEVER:
        debug_printf("never");
        break;

    case D3DCMP_LESS:
        debug_printf("less");
        break;

    case D3DCMP_EQUAL:
        debug_printf("equal");
        break;

    case D3DCMP_LESSEQUAL:
        debug_printf("less equal");
        break;

    case D3DCMP_GREATER:
        debug_printf("greater");
        break;

    case D3DCMP_NOTEQUAL:
        debug_printf("not equal");
        break;

    case D3DCMP_GREATEREQUAL:
        debug_printf("greater equal");
        break;

    case D3DCMP_ALWAYS:
        debug_printf("always");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      alpha blend:    ");
    switch (AlphaBlend)
    {
    case TRUE:
        debug_printf("true");
        break;

    case FALSE:
        debug_printf("false");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      src blend:      ");
    switch (SrcBlend)
    {
    case D3DBLEND_ZERO:
        debug_printf("zero");
        break;

    case D3DBLEND_ONE:
        debug_printf("one");
        break;

    case D3DBLEND_SRCCOLOR:
        debug_printf("src color");
        break;

    case D3DBLEND_INVSRCCOLOR:
        debug_printf("inv src color");
        break;

    case D3DBLEND_SRCALPHA:
        debug_printf("src alpha");
        break;

    case D3DBLEND_INVSRCALPHA:
        debug_printf("inv src alpha");
        break;

    case D3DBLEND_DESTALPHA:
        debug_printf("dest alpha");
        break;

    case D3DBLEND_INVDESTALPHA:
        debug_printf("inv dest alpha");
        break;

    case D3DBLEND_DESTCOLOR:
        debug_printf("dest color");
        break;

    case D3DBLEND_INVDESTCOLOR:
        debug_printf("inv dest color");
        break;

    case D3DBLEND_SRCALPHASAT:
        debug_printf("src alpha sat");
        break;

    case D3DBLEND_BOTHSRCALPHA:
        debug_printf("both src alpha");
        break;

    case D3DBLEND_BOTHINVSRCALPHA:
        debug_printf("both inv src alpha");
        break;

    case D3DBLEND_BLENDFACTOR:
        debug_printf("blend factor");
        break;

    case D3DBLEND_INVBLENDFACTOR:
        debug_printf("inv blend factor");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      dest blend:     ");
    switch (DestBlend)
    {
    case D3DBLEND_ZERO:
        debug_printf("zero");
        break;

    case D3DBLEND_ONE:
        debug_printf("one");
        break;

    case D3DBLEND_SRCCOLOR:
        debug_printf("src color");
        break;

    case D3DBLEND_INVSRCCOLOR:
        debug_printf("inv src color");
        break;

    case D3DBLEND_SRCALPHA:
        debug_printf("src alpha");
        break;

    case D3DBLEND_INVSRCALPHA:
        debug_printf("inv src alpha");
        break;

    case D3DBLEND_DESTALPHA:
        debug_printf("dest alpha");
        break;

    case D3DBLEND_INVDESTALPHA:
        debug_printf("inv dest alpha");
        break;

    case D3DBLEND_DESTCOLOR:
        debug_printf("dest color");
        break;

    case D3DBLEND_INVDESTCOLOR:
        debug_printf("inv dest color");
        break;

    case D3DBLEND_SRCALPHASAT:
        debug_printf("src alpha sat");
        break;

    case D3DBLEND_BOTHSRCALPHA:
        debug_printf("both src alpha");
        break;

    case D3DBLEND_BOTHINVSRCALPHA:
        debug_printf("both inv src alpha");
        break;

    case D3DBLEND_BLENDFACTOR:
        debug_printf("blend factor");
        break;

    case D3DBLEND_INVBLENDFACTOR:
        debug_printf("inv blend factor");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      blend op:       ");
    switch (BlendOp)
    {
    case D3DBLENDOP_ADD:
        debug_printf("add");
        break;

    case D3DBLENDOP_SUBTRACT:
        debug_printf("subtract");
        break;

    case D3DBLENDOP_REVSUBTRACT:
        debug_printf("revsubtract");
        break;

    case D3DBLENDOP_MIN:
        debug_printf("min");
        break;

    case D3DBLENDOP_MAX:
        debug_printf("max");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      alpha test:     ");
    switch (AlphaTest)
    {
    case TRUE:
        debug_printf("true");
        break;

    case FALSE:
        debug_printf("false");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      alpha func:     ");
    switch (TestFunc)
    {
    case D3DCMP_NEVER:
        debug_printf("never");
        break;

    case D3DCMP_LESS:
        debug_printf("less");
        break;

    case D3DCMP_EQUAL:
        debug_printf("equal");
        break;

    case D3DCMP_LESSEQUAL:
        debug_printf("less equal");
        break;

    case D3DCMP_GREATER:
        debug_printf("greater");
        break;

    case D3DCMP_NOTEQUAL:
        debug_printf("not equal");
        break;

    case D3DCMP_GREATEREQUAL:
        debug_printf("greater equal");
        break;

    case D3DCMP_ALWAYS:
        debug_printf("always");
        break;

    default:
        debug_printf("unknown");
        break;
    }
    debug_printf("\n");

    debug_printf("      alpha ref:      ");
    debug_printf("%d", TestRef);
    debug_printf("\n");

    debug_printf("-------------end----------------\n\n");
}

int D3D9RenderSystem::GetFramePrimitiveCount()
{
    return mLastFramePrimitiveCount;
}

int D3D9RenderSystem::GetFrameBatchCount()
{
    return mLastFrameBatchCount;
}

void D3D9RenderSystem::EnumDrives()
{
    D3D9DisplayDevice * drive;

    for (UINT i = 0; i < mDirect3D->GetAdapterCount(); ++i)
    {
        drive = mDriveList.Increase();
        ZeroMemory(drive, sizeof(D3D9DisplayDevice));
        drive->AdapterNumber = i;
        mDirect3D->GetAdapterIdentifier(i, 0, &drive->Identifier);
        mDirect3D->GetAdapterDisplayMode(i, &drive->DisplayMode);
    }
}

int D3D9RenderSystem::GetAdapterCount()
{
    return mDriveList.Size();
}

TString128 D3D9RenderSystem::GetAdapterName(int iAdapterNumber)
{
    return TString128(mDriveList[iAdapterNumber].Identifier.Description);
}




}
