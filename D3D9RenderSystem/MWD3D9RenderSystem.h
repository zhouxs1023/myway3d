#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9RenderWindow.h"
#include "MWRenderSystem.h"

namespace Myway
{

class RS_ENTRY D3D9RenderSystem : public RenderSystem
{
    friend class D3D9RenderWindow;

public:
                                        D3D9RenderSystem(void);
                                        ~D3D9RenderSystem(void);

    virtual void                        Init();

    virtual void                        OnResize();

    virtual bool                        CheckMSAAFormat(FORMAT format, MULTI_SAMPLE msaa);
    virtual bool                        CheckTextureFormat(FORMAT format, USAGE usage, bool autoGenMimmap);
    virtual bool                        CheckRenderTargetFormat(FORMAT Format);

    virtual void                        ClearBuffer(const Rect * range, 
                                                    bool color, bool depth, bool stencil,
                                                    const Color & c, float z, int s);

    virtual void                        BeginScene();
    virtual void                        EndScene();

    virtual void                        SetWorldTransform(const Mat4 & form);
    virtual void                        SetViewTransform(const Mat4 & form);
    virtual void                        SetProjTransform(const Mat4 & form);
    virtual void                        SetBlendTransform(const Mat4 * forms, int count);

    virtual void                        SetRenderState(const RenderState & state);

    virtual void                        SetFog(const Vec4 & param, const Color4 & color);

            void                        SetSamplerState(int stage, const SamplerState & state);
    virtual void                        SetTexture(int stage, const SamplerState & state, Texture * texture);

    virtual void                        SetMaterial(const Color4 & Ambient,
                                                    const Color4 & Emissive,
                                                    const Color4 & Diffuse,
                                                    const Color4 & Specular,
                                                    float power);

    virtual void                        SetLight(int index, Light * light);

    virtual void                        SetCamera(Camera * cam);

    virtual void                        SetVertexShader(ShaderProgram * shader);
    virtual void                        SetPixelShader(ShaderProgram * shader);
    virtual void                        SetVertexShaderParam(ShaderParamTable * table);
    virtual void                        SetPixelShaderParam(ShaderParamTable * table);

    virtual void                        SetVertexShaderConstF(int reg, const float * data, int vec4Count);
    virtual void                        SetVertexShaderConstI(int reg, const int * data, int vec4Count);

    virtual void                        SetPixelShaderConstF(int reg, const float * data, int vec4Count);
    virtual void                        SetPixelShaderConstI(int reg, const int * data, int vec4Count);

    virtual void                        SetRenderTarget(int index, RenderTarget * target);
    virtual void                        SetDepthStencil(DepthStencil * depth);
    virtual RenderTarget *              GetRenderTarget(int index);
    virtual DepthStencil *              GetDepthStencil();

    virtual void                        SetTime(float time);

    virtual void                        SetViewport(const RectF * vp);

    virtual void                        SetClipPlane(int index, const Plane & plane);
    virtual void                        SetScissorRect(const Rect & rect);
    virtual void                        EnableClipPlane(int index, bool enable);
    virtual void                        EnableScissorTest(bool enable);

    virtual void                        Render(Technique * efx, Renderer * obj);
    virtual void                        Render(Technique * efx, RenderOp * rd);

    virtual int                         GetFramePrimitiveCount();
    virtual int                         GetFrameBatchCount();

            void                        OutputRenderState();

            void                        EnumDrives();

    virtual int                         GetAdapterCount();
    virtual TString128                  GetAdapterName(int adapter);

    IDirect3D9 *                        GetDirect3D() { return mDirect3D; }
    IDirect3DDevice9 *                  GetD3DDevice() { return mD3DDevice; }
    const D3D9DisplayDevice &           GetDisplayDevice(int adapter) { return mDriveList[adapter]; }

protected:
    IDirect3D9 *                        mDirect3D;
    IDirect3DDevice9 *                  mD3DDevice;

    int                                 mMipmapLevel;

    int                                 mLastFramePrimitiveCount;
    int                                 mLastFrameBatchCount;
    int                                 mPrimitivCount;
    int                                 mBatchCount;
    Array<D3D9DisplayDevice>            mDriveList;

    D3D9RenderWindow *                  mWindow;
    RenderRegister                      mRegister;

    ShaderProgram *                     mVertexShader;
    ShaderProgram *                     mPixelShader;
    ShaderParamTable *                  mVertexShaderParams;
    ShaderParamTable *                  mPixelShaderParams;
    
    RenderTarget *                      mRenderTarget[MAX_RENDER_TARGET];
    DepthStencil *                      mDepthStencil;

    Mat4                                mMatWorld;
    Mat4                                mMatBlend[MAX_BLEND_MATRIX];
    int                                 mNumBlends;
};

}


