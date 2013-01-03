#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWD3D9DeviceCaps.h"
#include "MWD3D9VideoBufferManager.h"
#include "MWD3D9ShaderProgramManager.h"
#include "MWSMAAController.h"

namespace Myway
{

class D3D9RenderSystem;

class RS_ENTRY D3D9RenderWindow
{
    friend class D3D9RenderSystem;

public:
    D3D9RenderWindow(D3D9RenderSystem * pRenderSystem);
   ~D3D9RenderWindow();

    void            OnResize();

    void            Present();
    void            SaveScreenShot(const TString128 & sName, IMAGE_FILE_FORMAT iff);
    
    void            LostDevice();
    void            ResetDevice();

    RenderTarget *  GetRenderTarget() { return mRenderTarget; }
    DepthStencil *  GetDepthStencil() { return mDepthStencil; }

	void			DoSMAA(RenderTarget * rt, Texture * colorTex);

protected:
    void _acquire();


protected:
    D3D9RenderSystem *              mRenderSystem;
    int                             mAdapter;
    D3DPRESENT_PARAMETERS           mPresentParam;
    D3D9RenderTarget *              mRenderTarget;
    D3D9DepthStencil *              mDepthStencil;

    D3D9VideoBufferManager *        mVideoBufferManager;
    D3D9ShaderProgramManager *      mShaderProgramManager;

    D3D9DeviceCaps                  mDevCaps;

	SMAAController *				mSMAAController;
};

}