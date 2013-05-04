#include "MWSky.h"
#include "MWEnvironment.h"

namespace Myway {

    Sky::Sky()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Sky");

        d_assert (mTech);

        _geometry();
    }

    Sky::~Sky()
    {
    }

    void Sky::Render()
    {
        float mU = Environment::Instance()->GetEvParam()->SkyU;
        float mVOff = Environment::Instance()->GetGlobalParam()->SkyVOffset;
        const TString128 & skyTexture = Environment::Instance()->GetGlobalParam()->SkyTexture;

        if (mTexture.IsNull() || mTexture->GetName() == skyTexture)
            mTexture = VideoBufferManager::Instance()->Load2DTexture(skyTexture, skyTexture);

        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        float farclip = cam->GetFarClip() * 0.9f;
        Vec3 pos = cam->GetPosition();

		ShaderParam * uU = mTech->GetVertexShaderParamTable()->GetParam("gU");

		uU->SetUnifom(mU, mVOff, 1 / (1 + mVOff), 0);

		SamplerState state;
		state.Address = TEXA_CLAMP;
		render->SetTexture(0, state, mTexture.c_ptr());

		mRender.xform.MakeTransform(pos, Quat::Identity, farclip);

		render->Render(mTech, &mRender);
    }

    void Sky::_geometry()
    {
        int iRings = 30, iSegments = 30;
        int iVertexCount = (iRings + 1) * (iSegments + 1);
        int iIndexCount = iRings * iSegments * 6;
        int iPrimCount = iIndexCount / 3;

        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        d_assert(iIndexCount < 65536 &&"index buffer too large.");

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12, 12);
        
        float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            float fTileRingAngle = (Math::PI_1 / iRings);
            float fTileSegAngle = (Math::PI_2 / iSegments);
            float r;
            short i, j;
            float x, y, z;

            for (i = 0; i <= iRings; ++i)
            {
                r = Math::Sin(i * fTileRingAngle);
                y = Math::Cos(i * fTileRingAngle);

                for (j = 0; j <= iSegments; ++j)
                {
                    x = r * Math::Cos(j * fTileSegAngle);
                    z = r * Math::Sin(j * fTileSegAngle);

                    *vert++ = x;
                    *vert++ = y;
                    *vert++ = z;
                }

            }
        }
        vb->Unlock();

        vxStream->Bind(0,vb, 12);
        vxStream->SetCount(iVertexCount);

        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
        short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
        {
            short row = 0, row_n = 0;
            short i, j;

            for (i = 0; i < iRings; ++i)
            {
                row_n = row + iSegments + 1;

                for (j = 0; j < iSegments; ++j)
                {
                    *idx++ = row + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j;

                    *idx++ = row_n + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j + 1;

                }

                row += iSegments + 1;
            }
        }
        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_OPATICY;;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }






    Sky2::Sky2()
    {
		mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Sky2");
		_init();
    }

    Sky2::~Sky2()
    {
    }

	void Sky2::Render()
	{
		_update();

		ShaderParamTable * vTable = mTech->GetVertexShaderParamTable();

		ShaderParam * uMatWVP = vTable->GetParam("uWorldViewProj");

		Mat4 wvp = World::Instance()->MainCamera()->GetViewProjMatrix();

		RenderSystem * render = RenderSystem::Instance();
		Camera * cam = World::Instance()->MainCamera();

		float farclip = cam->GetFarClip() * 0.9f;
		Vec3 pos = cam->GetPosition();

		mRender.xform.MakeTransform(pos, Quat::Identity, farclip);

		wvp = mRender.xform * wvp;

		uMatWVP->SetMatrix(wvp);

		render->Render(mTech, &mRender);
	}

	void Sky2::RenderReflection(const Plane & plane)
	{
		_update();

		ShaderParamTable * vTable = mTech->GetVertexShaderParamTable();

		ShaderParam * uMatWVP = vTable->GetParam("uWorldViewProj");

		Mat4 matMirror;
		matMirror.MakeReflect(plane);
		Mat4 wvp = matMirror * World::Instance()->MainCamera()->GetViewProjMatrix();

		RenderSystem * render = RenderSystem::Instance();
		Camera * cam = World::Instance()->MainCamera();

		float farclip = cam->GetFarClip() * 0.9f;
		Vec3 pos = cam->GetPosition();

		mRender.xform.MakeTransform(pos, Quat::Identity, farclip);

		wvp = mRender.xform * wvp;

		uMatWVP->SetMatrix(wvp);

		render->Render(mTech, &mRender);
	}

    void Sky2::_init()
    {
		int iRings = 50, iSegments = 50;
		int iVertexCount = (iRings + 1) * (iSegments + 1);
		int iIndexCount = iRings * iSegments * 6;
		int iPrimCount = iIndexCount / 3;

		VertexStream * vxStream = &mRender.vxStream;
		IndexStream * ixStream = &mRender.ixStream;

		d_assert(iIndexCount < 65536 &&"index buffer too large.");

		VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		decl->Init();

		vxStream->SetDeclaration(decl);

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12, 12);

		float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
		{
			float fTileRingAngle = (Math::PI_1 / iRings);
			float fTileSegAngle = (Math::PI_2 / iSegments);
			float r;
			short i, j;
			float x, y, z;

			for (i = 0; i <= iRings; ++i)
			{
				r = Math::Sin(i * fTileRingAngle);
				y = Math::Cos(i * fTileRingAngle);

				for (j = 0; j <= iSegments; ++j)
				{
					x = r * Math::Cos(j * fTileSegAngle);
					z = r * Math::Sin(j * fTileSegAngle);

					*vert++ = x;
					*vert++ = y;
					*vert++ = z;
				}

			}
		}
		vb->Unlock();

		vxStream->Bind(0,vb, 12);
        vxStream->SetCount(iVertexCount);

        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
        short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
        {
            short row = 0, row_n = 0;
            short i, j;

            for (i = 0; i < iRings; ++i)
            {
                row_n = row + iSegments + 1;

                for (j = 0; j < iSegments; ++j)
                {
                    *idx++ = row + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j;

                    *idx++ = row_n + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j + 1;

                }

                row += iSegments + 1;
            }
        }
        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_OPATICY;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }

	void Sky2::_update()
	{
		Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;

		ShaderParamTable * vTable = mTech->GetVertexShaderParamTable();
		ShaderParamTable * pTable = mTech->GetPixelShaderParamTable();

		ShaderParam * uLightDirVS = vTable->GetParam("uLightDir");
		ShaderParam * uLightDirPS = pTable->GetParam("uLightDir");
		
		ShaderParam * uInnerRadius = vTable->GetParam("uInnerRadius");
		ShaderParam * uCameraPos = vTable->GetParam("uCameraPos");
		ShaderParam * uScale = vTable->GetParam("uScale");
		ShaderParam * uScaleDepth = vTable->GetParam("uScaleDepth");
		ShaderParam * uScaleOverScaleDepth = vTable->GetParam("uScaleOverScaleDepth");

		ShaderParam * uKr4PI = vTable->GetParam("uKr4PI");
		ShaderParam * uKrESun = vTable->GetParam("uKrESun");

		ShaderParam * uInvWaveLength = vTable->GetParam("uInvWaveLength");

		ShaderParam * uG = pTable->GetParam("uG");
		ShaderParam * uG2 = pTable->GetParam("uG2");

		ShaderParam * uExposure = pTable->GetParam("uExposure");

		{
			uLightDirVS->SetUnifom(-sunDir.x, -sunDir.y, -sunDir.z, 0);
			uLightDirPS->SetUnifom(-sunDir.x, -sunDir.y, -sunDir.z, 0);
		}

		{
			float Scale = 1.0f / (mOptions.OuterRadius - mOptions.InnerRadius);
			float ScaleDepth = (mOptions.OuterRadius - mOptions.InnerRadius) / 2.0f;
			float ScaleOverScaleDepth = Scale / ScaleDepth;

			uInnerRadius->SetUnifom(mOptions.InnerRadius, 0, 0, 0);
			uCameraPos->SetUnifom(0, mOptions.InnerRadius + (mOptions.OuterRadius-mOptions.InnerRadius)*mOptions.HeightPosition, 0, 0);

			uScale->SetUnifom(Scale, 0, 0, 0);
			uScaleDepth->SetUnifom(ScaleDepth, 0, 0, 0);
			uScaleOverScaleDepth->SetUnifom(ScaleOverScaleDepth, 0, 0, 0);
		}

		{
			float Kr4PI  = mOptions.RayleighMultiplier * 4.0f * Math::PI_1;
			float KrESun = mOptions.RayleighMultiplier * mOptions.SunIntensity;

			uKr4PI->SetUnifom(Kr4PI, 0, 0, 0);
			uKrESun->SetUnifom(KrESun, 0, 0, 0);
		}

		{
			Vec3 invWaveLength = Vec3(1 / Math::Pow(mOptions.WaveLength.x, 4.0f),
				                      1 / Math::Pow(mOptions.WaveLength.y, 4.0f),
									  1 / Math::Pow(mOptions.WaveLength.z, 4.0f));

			uInvWaveLength->SetUnifom(invWaveLength.x, invWaveLength.y, invWaveLength.z, 0);
		}

		{
			uG->SetUnifom(mOptions.G, 0, 0, 0);
			uG2->SetUnifom(mOptions.G*mOptions.G, 0, 0, 0);
		}

		{
			uExposure->SetUnifom(mOptions.Exposure, 0, 0, 0);
		}
	}
}