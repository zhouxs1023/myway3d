#include "MWRenderHelper.h"
#include "MWRenderRegister.h"
#include "MWRenderEvent.h"
#include "Engine.h"

namespace Myway
{
    IMP_SLN(RenderHelper);
    RenderHelper::RenderHelper()
		: OnReisze(RenderEvent::OnResize, this, &RenderHelper::_resize)
    {  
        INIT_SLN;

        _initTexture();
        _initScreenQuad();
    }

    RenderHelper::~RenderHelper()
    {
        SHUT_SLN;
    }

	void RenderHelper::_resize(Event * sender)
	{
		_updateScreenQuad();
	}

    void RenderHelper::_initTexture()
    {
        mWhiteTexture = VideoBufferManager::Instance()->CreateTexture("Core_WhiteTexture", 4, 4, 1);
        mBlackTexture = VideoBufferManager::Instance()->CreateTexture("Core_BlackTexture", 4, 4, 1);
        mDefaultNormaLTexture = VideoBufferManager::Instance()->CreateTexture("Core_DefaultTexture", 4, 4, 1);

        {
            LockedBox pbox;
            mWhiteTexture->Lock(0, &pbox, NULL, LOCK_NORMAL);
            Color * color = (Color *)pbox.pData;
            for (int j = 0; j < 4; ++j)
            {
                for (int i = 0; i < 4; ++i)
                {
                    *color++ = Color::White;
                }
            }
            mWhiteTexture->Unlock(0);
        }

        {
            LockedBox pbox;
            mBlackTexture->Lock(0, &pbox, NULL, LOCK_NORMAL);
            Color * color = (Color *)pbox.pData;
            for (int j = 0; j < 4; ++j)
            {
                for (int i = 0; i < 4; ++i)
                {
                    *color++ = Color::Black;
                }
            }
            mBlackTexture->Unlock(0);
        }

        {
            LockedBox pbox;
            mDefaultNormaLTexture->Lock(0, &pbox, NULL, LOCK_NORMAL);
            int * color = (int *)pbox.pData;
            for (int j = 0; j < 4; ++j)
            {
                for (int i = 0; i < 4; ++i)
                {
                    *color++ = M_XRGB(128, 128, 255);
                }
            }
            mDefaultNormaLTexture->Unlock(0);
        }

        mDefaultTextureFilter = TEXF_LINEAR;
    }

    Texture * RenderHelper::GetTexture(Material * mat, SamplerBindType type)
    {
        switch (type)
        {
        case SBT_Emissive:
            return mat->GetEmissiveMap().c_ptr();

        case SBT_Diffuse:
            return mat->GetDiffuseMap().c_ptr();

        case SBT_Specular:
            return mat->GetSpecularMap().c_ptr();

        case SBT_Normal:
            return mat->GetNormalMap().c_ptr();

        case SBT_Scene:
            break;

        case SBT_Shadow:
            break;
        }

        return NULL;
    }

    void RenderHelper::_initScreenQuad()
    {
        VertexStream * vxStream = &mScreenQuad.vxStream;
        IndexStream * ixStream = &mScreenQuad.ixStream;

        int iVertexCount = 4;
        int iPrimCount = 2;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20, 20, USAGE_DYNAMIC);

        vxStream->Bind(0, vb, 20, 0);
        vxStream->SetCount(iVertexCount);

        mScreenQuad.iPrimCount = iPrimCount;
        mScreenQuad.ePrimType = PRIM_TRIANGLESTRIP;

        mScreenQuad.rState.cullMode = CULL_NONE;
        mScreenQuad.rState.blendMode = BM_OPATICY;
        mScreenQuad.rState.depthWrite = false;
        mScreenQuad.rState.depthCheck = DCM_ALWAYS;

        _updateScreenQuad();
    }

    void RenderHelper::_updateScreenQuad()
    {
        const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

        float halfInvWidth = 0.5f / dp->Width;
        float halfInvHeight = 0.5f / dp->Height;

        //halfInvWidth = 0;
        //halfInvHeight = 0;

        VertexStream * vxStream = &mScreenQuad.vxStream;
        VertexBufferPtr vb = vxStream->GetStream(0);

        float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            float x = 0, y = 0, z = 0;

            *vert++ = -1; *vert++ = 1; *vert++ = 0;
            *vert++ = 0 + halfInvWidth; *vert++ = 0 + halfInvHeight;

            *vert++ = 1; *vert++ = 1; *vert++ = 1;
            *vert++ = 1 + halfInvWidth; *vert++ = 0 + halfInvHeight;

            *vert++ = -1; *vert++ = -1; *vert++ = 0;
            *vert++ = 0 + halfInvWidth; *vert++ = 1 + halfInvHeight;

            *vert++ = 1; *vert++ = -1; *vert++ = 0;
            *vert++ = 1 + halfInvWidth; *vert++ = 1 + halfInvHeight;
        }
        vb->Unlock();
    }

    void RenderHelper::DrawScreenQuad(BLEND_MODE mode, Technique * tech)
    {
        mScreenQuad.rState.blendMode = mode;
        RenderSystem::Instance()->Render(tech, &mScreenQuad);
    }
}