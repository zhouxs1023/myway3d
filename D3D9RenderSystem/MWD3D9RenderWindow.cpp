#include "MWD3D9RenderWindow.h"
#include "MWD3D9VideoBufferManager.h"
#include "MWD3D9ShaderProgram.h"
#include "MWD3D9RenderSystem.h"
#include "Engine.h"

namespace Myway
{

D3D9RenderWindow::D3D9RenderWindow(D3D9RenderSystem * pRenderSystem)
: mRenderSystem(pRenderSystem)
{
    const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

    IDirect3D9 * pDirect3D = pRenderSystem->GetDirect3D();
    IDirect3DDevice9 * pD3D9Device = NULL;

    memset(&mPresentParam, 0, sizeof (D3DPRESENT_PARAMETERS));

    mAdapter = dp->Adpter;

    mPresentParam.hDeviceWindow				    = dp->hWnd;
    mPresentParam.Windowed					    = dp->bWnded;
    mPresentParam.SwapEffect					= D3DSWAPEFFECT_DISCARD;
    mPresentParam.BackBufferCount				= 1;
    mPresentParam.BackBufferWidth				= dp->Width;
    mPresentParam.BackBufferHeight			    = dp->Height;
    mPresentParam.EnableAutoDepthStencil		= FALSE;
    mPresentParam.Flags                         = 0;

    if (dp->bVSync)
        mPresentParam.PresentationInterval    = D3DPRESENT_INTERVAL_DEFAULT;
    else
        mPresentParam.PresentationInterval    = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (dp->bWnded)
    {
        mPresentParam.BackBufferFormat		        = D3DFMT_UNKNOWN;
        mPresentParam.FullScreen_RefreshRateInHz	= 0;
    }
    else
    {
        mPresentParam.BackBufferFormat			    = D3DFMT_X8R8G8B8;
        mPresentParam.FullScreen_RefreshRateInHz	= dp->RefreshRate;
    }

    //multi sample support
    mPresentParam.MultiSampleType                 = D3DMULTISAMPLE_NONE;
    mPresentParam.MultiSampleQuality              = 0;


    //check format 
    //check back buffer format 
    if(FAILED(pDirect3D->CheckDeviceMultiSampleType(mAdapter, D3DDEVTYPE_HAL, 
                                                    D3DFMT_X8R8G8B8, 
                                                    mPresentParam.Windowed, 
                                                    mPresentParam.MultiSampleType, NULL)))
    {
        FORMAT format = D3D9Mapping::GetFormat(mPresentParam.BackBufferFormat);

        EXCEPTION("D3D Error: back buffer format '" +  D3D9Mapping::GetFormatString(format) + 
                  "' not support.");
    }
    
    D3DDEVTYPE devtype = D3DDEVTYPE_HAL;

    UINT adapterToUse = mAdapter;

    if (dp->bNVPerfHUD)
    {
        // Look for 'NVIDIA NVPerfHUD' adapter (<= v4)
        // or 'NVIDIA PerfHUD' (v5)
        // If it is present, override default settings
        for (UINT adapter=0; adapter < mRenderSystem->GetDirect3D()->GetAdapterCount(); ++adapter)
        {
            D3DADAPTER_IDENTIFIER9 identifier;
            HRESULT res;
            res = pDirect3D->GetAdapterIdentifier(adapter, 0, &identifier);
            if (strstr(identifier.Description,"PerfHUD") != 0)
            {
                adapterToUse = adapter;
                devtype = D3DDEVTYPE_REF;
                break;
            }
        }
    }

    HRESULT hr;
    hr = pDirect3D->CreateDevice(adapterToUse, devtype,
                                 NULL, 
                                 D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                 &mPresentParam, &pD3D9Device);

    D3DErrorExceptionFunction(CreateDevice, hr);

    pRenderSystem->mD3DDevice = pD3D9Device;

    mVideoBufferManager = new D3D9VideoBufferManager(pD3D9Device);
    mShaderProgramManager = new D3D9ShaderProgramManager(pD3D9Device);

    mRenderTarget = new D3D9RenderTarget(pD3D9Device);
    mRenderTarget->mRenderTarget = NULL;
    mRenderTarget->mName = "Core_MainRenderTarget";
    mRenderTarget->mFormat = D3D9Mapping::GetFormat(mPresentParam.BackBufferFormat);
    mRenderTarget->mMSAA = D3D9Mapping::GetMultiSampleType(mPresentParam.MultiSampleType);
    mRenderTarget->mWidth = -1;
    mRenderTarget->mHeight = -1;
    mRenderTarget->IncRef();

    mDepthStencil = new D3D9DepthStencil(pD3D9Device);
    mDepthStencil->mDepthStencil = NULL;
    mDepthStencil->mName = "Core_MainDepthStencil";
    mDepthStencil->mFormat = D3D9Mapping::GetFormat(mPresentParam.AutoDepthStencilFormat);
    mDepthStencil->mMSAA = D3D9Mapping::GetMultiSampleType(mPresentParam.MultiSampleType);
    mRenderTarget->mWidth = -1;
    mRenderTarget->mHeight = -1;
    mDepthStencil->IncRef();

    _acquire();

    pDirect3D->GetDeviceCaps(mAdapter, D3DDEVTYPE_HAL, mDevCaps.GetCaps());

	mSMAAController = new SMAAController();
	mSMAAController->Init();
}

D3D9RenderWindow::~D3D9RenderWindow()
{
    mRenderTarget->DecRef();
    mDepthStencil->DecRef();

    safe_delete(mDepthStencil);
    safe_delete(mRenderTarget);

    delete mVideoBufferManager;
    delete mShaderProgramManager;

	delete mSMAAController;
}


void D3D9RenderWindow::OnResize()
{
    const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();
    
    mPresentParam.BackBufferWidth = dp->Width;
    mPresentParam.BackBufferHeight = dp->Height;

    ResetDevice();
}

void D3D9RenderWindow::Present()
{
    profile_code();

    HRESULT hr;

    IDirect3DDevice9 * device = mRenderSystem->GetD3DDevice();

    hr = device->Present(NULL, NULL, NULL, NULL);

    if (FAILED(hr))
    {
        LostDevice();
    }
}

void D3D9RenderWindow::LostDevice()
{
    IDirect3DDevice9 * device = mRenderSystem->GetD3DDevice();

    HRESULT hr;

    hr = device->TestCooperativeLevel();

    switch (hr)
    {
    case D3D_OK:
        break;

    case D3DERR_DEVICELOST:
        break;

    case D3DERR_DEVICENOTRESET:
        ResetDevice();
        break;

    case D3DERR_DRIVERINTERNALERROR:
        assert(0 && "drive internal error.");
        break;

    default:
        assert(0 && "unknow error");
        break;

    }
}

void D3D9RenderWindow::ResetDevice()
{
    IDirect3DDevice9 * device = mRenderSystem->GetD3DDevice();

    safe_release_com(mRenderTarget->mRenderTarget);
    safe_release_com(mDepthStencil->mDepthStencil);
	safe_release_com(mRenderSystem->mD3DQuery);

    mVideoBufferManager->LostDevice();

	mSMAAController->Shutdown();

    HRESULT hr = device->Reset(&mPresentParam);

    if (hr != D3D_OK)
    {
        d_assert (0);
        //D3DErrorExceptionFunction(Reset, hr);
    }

    _acquire();

    mVideoBufferManager->ResetDevice();

	device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &mRenderSystem->mD3DQuery);

	mSMAAController->Init();
}

void D3D9RenderWindow::SaveScreenShot(const TString128 & sFile, IMAGE_FILE_FORMAT iff)
{
    IDirect3DDevice9 * device = mRenderSystem->GetD3DDevice();
    LPDIRECT3DSURFACE9 surface = NULL;

    HRESULT hr;

    hr = device->CreateOffscreenPlainSurface(mPresentParam.BackBufferWidth, mPresentParam.BackBufferHeight, 
                                             D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, NULL);

    D3DErrorExceptionFunction(CreateOffscreenPlainSurface, hr);

    device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);

    hr = D3DXSaveSurfaceToFile(sFile.c_str(), D3D9Mapping::GetD3DXImageFileFormat(iff), surface, NULL, NULL);

    D3DErrorExceptionFunction(D3DXSaveSurfaceToFile, hr);

    safe_release_com(surface);
}

void D3D9RenderWindow::_acquire()
{
    d_assert (mRenderTarget->mRenderTarget == NULL && mDepthStencil->mDepthStencil == NULL);

    IDirect3DDevice9 * device = mRenderSystem->GetD3DDevice();

    device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &mRenderTarget->mRenderTarget);
    device->GetDepthStencilSurface(&mDepthStencil->mDepthStencil);
}

void D3D9RenderWindow::SetSMAAType(eSmaaType::enum_t type)
{
	mSMAAController->SetSMAAType(type);
}

eSmaaType::enum_t D3D9RenderWindow::GetSMAAType()
{
	return mSMAAController->GetSMAAType();
}

void D3D9RenderWindow::DoSMAA(RenderTarget * rt, Texture * colorTex)
{
	mSMAAController->Do(rt, colorTex);
}

}
