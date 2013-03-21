#pragma once

#include "MWVideoBufferManager.h"
#include "MWMaterial.h"
#include "MWShaderProgram.h"
#include "MWRenderer.h"
#include "MWRenderRegister.h"
#include "MWDepthStencil.h"
#include "MWCamera.h"
#include "MWRenderQueue.h"


namespace Myway
{
	

/* Interface: RenderSystem
--------------------------------------------------------------
    @Remark:
        äÖÈ¾Æ÷.
--------------------------------------------------------------
*/
class MW_ENTRY RenderSystem
{
    DECLARE_SINGLETON(RenderSystem);

public:
                            RenderSystem();
    virtual                 ~RenderSystem();

    virtual void            Init() = 0;
    virtual void            OnResize() = 0;

	virtual void			_BeginEvent(const char * str) = 0;	// for pix
	virtual void			_EndEvent() = 0; 

	virtual void			BeginOcclusionQuery() = 0;
	virtual int				EndOcclusionQuery() = 0;

    virtual bool            CheckTextureFormat(FORMAT format, USAGE usage, bool autoGenMimmap = false) = 0;
    virtual bool            CheckRenderTargetFormat(FORMAT format) = 0;
    virtual bool            CheckMSAAFormat(FORMAT format, MULTI_SAMPLE msaa) = 0;

    virtual void            ClearBuffer(const Rect * range, 
                                        bool color, bool depth, bool stencil,
                                        const Color & c = Color::Black, float z = 1, int s = 0) = 0;

    virtual void            BeginScene() = 0;
    virtual void            EndScene() = 0;

    virtual void            SetWorldTransform(const Mat4 & form) = 0;
    virtual void            SetViewTransform(const Mat4 & form) = 0;
    virtual void            SetProjTransform(const Mat4 & form) = 0;
    virtual void            SetBlendTransform(const Mat4 * forms, int count) = 0;

    virtual void            SetRenderState(const RenderState & state) = 0;
    virtual void            SetTexture(int stage, const SamplerState & state, Texture * texture) = 0;

    virtual void            SetFog(const Vec4 & param, const Color4 & color) = 0;


    virtual void            SetMaterial(const Color4 & ambient,
                                        const Color4 & emissive,
                                        const Color4 & diffuse,
                                        const Color4 & specular,
                                        float power) = 0;

    virtual void            SetLight(int index, Light * light) = 0;

    virtual void            SetCamera(Camera * cam) = 0;

    virtual void            SetVertexShader(ShaderProgram * shader) = 0;
    virtual void            SetPixelShader(ShaderProgram * shader) = 0;
    
    virtual void            SetVertexShaderParam(ShaderParamTable * table) = 0;
    virtual void            SetPixelShaderParam(ShaderParamTable * table) = 0;

    virtual void            SetVertexShaderConstF(int reg, const float * data, int vec4Count) = 0;
    virtual void            SetVertexShaderConstI(int reg, const int * data, int vec4Count) = 0;

    virtual void            SetPixelShaderConstF(int reg, const float * data, int vec4Count) = 0;
    virtual void            SetPixelShaderConstI(int reg, const int * data, int vec4Count) = 0;

    virtual void            SetRenderTarget(int index, RenderTarget * target) = 0;
    virtual void            SetDepthStencil(DepthStencil * depth) = 0;
    virtual RenderTarget *  GetRenderTarget(int index) = 0;
    virtual DepthStencil *  GetDepthStencil()= 0;

    virtual void            SetTime(float time) = 0;

    virtual void            SetViewport(const RectF * vp) = 0;

    virtual void            SetClipPlane(int index, const Plane & plane) = 0;
    virtual void            SetScissorRect(const Rect & rect) = 0;
    virtual void            EnableClipPlane(int index, bool enable) = 0;
    virtual void            EnableScissorTest(bool enable) = 0;

    virtual void            Render(Technique * efx, Renderer * obj) = 0;
    virtual void            Render(Technique * efx, RenderOp * rd) = 0;
	virtual void			RenderUp(Technique * efx, RenderOp * rop,
									 const void * vertices, int stride, int numVerts,
		                             const void * indices, bool bIndex32 = true) = 0;

	virtual void			RenderUI(Technique * efx, VertexDeclarationPtr decl, VertexBufferPtr vb, int primCount) = 0;

	virtual void			SetSMAAType(eSmaaType::enum_t type) = 0;
	virtual eSmaaType::enum_t GetSMAAType() = 0;
	virtual void			DoSMAA(RenderTarget * rt, Texture * colorTex) = 0;

    virtual int             GetFramePrimitiveCount() = 0;
    virtual int             GetFrameBatchCount() = 0;

    virtual int             GetAdapterCount() = 0;
    virtual TString128      GetAdapterName(int adapter) = 0;

			void			SetFlipCullMode(bool enable) { mFlipCullMode = enable; }

protected:
	bool mFlipCullMode;
};


struct MW_ENTRY __RenderEventHelpr
{
	__RenderEventHelpr(const char * str)
	{
		RenderSystem::Instance()->_BeginEvent(str);
	}

	~__RenderEventHelpr()
	{
		RenderSystem::Instance()->_EndEvent();
	}
};


#ifdef _DEBUG

#define RS_RenderEvent(name) __RenderEventHelpr __re_##name(#name)

#else

#define RS_RenderEvent(name)

#endif

}
