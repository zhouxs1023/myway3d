#include "MWWater.h"
#include "MWEnvironment.h"
#include "MWUnderWater.h"
#include "MWProjectionGrid.h"
#include "Engine.h"

namespace Myway {

	static int kBlockGridSize = Terrain::kSectionVertexSize - 1;

	struct WaterIdentityVertex
	{
		struct Vertex {
			int _x, _y;
			Vec3 position;
		};

		Array<Vertex> vertArray;

		int Add(int _x, int _y, const Vec3 & pos)
		{
			for (int i = 0; i < vertArray.Size(); ++i)
			{
				if (vertArray[i]._x == _x && vertArray[i]._y == _y)
					return i;
			}

			Vertex v = { _x, _y, pos };

			vertArray.PushBack(v);

			return vertArray.Size() - 1;
		}
	};

	WaterBlock::WaterBlock()
		: mRenderOp(NULL)
		, mBound(Aabb::Invalid)
	{
	}

	WaterBlock::~WaterBlock()
	{
		Shudown();
	}

	void WaterBlock::Shudown()
	{
		safe_delete (mRenderOp);

		mBound = Aabb::Invalid;
	}

	void WaterBlock::Init(Water* water, int x, int z)
	{
		Shudown();

		mWater = water;
		mIndexX = x;
		mIndexZ = z;

		Terrain * terrain = Environment::Instance()->GetTerrain();

		int startX = kBlockGridSize * x;
		int startZ = kBlockGridSize * z;
		int sizeZ = water->GetSizeZ() - 1;

		WaterIdentityVertex wiv;
		Array<unsigned short> indexArray;

		for (int j = startZ; j < startZ + kBlockGridSize; ++j)
		{
			for (int i = startX; i < startX + kBlockGridSize; ++i)
			{
				char flag = mWater->GetData(i, j);
				
				if (flag == 1)
				{
					int _x = i;
					int _y = sizeZ - j;

					Vec3 p0 = terrain->GetPosition(_x + 0, _y + 0);
					Vec3 p1 = terrain->GetPosition(_x + 1, _y + 0);
					Vec3 p2 = terrain->GetPosition(_x + 0, _y + 1);
					Vec3 p3 = terrain->GetPosition(_x + 1, _y + 1);

					int i0 = wiv.Add(_x + 0, _y + 0, p0);
					int i1 = wiv.Add(_x + 1, _y + 0, p1);
					int i2 = wiv.Add(_x + 0, _y + 1, p2);
					int i3 = wiv.Add(_x + 1, _y + 1, p3);

					indexArray.PushBack(i0);
					indexArray.PushBack(i1);
					indexArray.PushBack(i2);
					indexArray.PushBack(i3);

					mBound.Merge(p0);
					mBound.Merge(p1);
					mBound.Merge(p2);
					mBound.Merge(p3);
				}
			}
		}

		if (indexArray.Size() == 0)
			return ;

		mRenderOp = new RenderOp();

		mRenderOp->iPrimCount = indexArray.Size() - 2;
		mRenderOp->ePrimType = PRIM_TRIANGLELIST;

		int iVertexCount = wiv.vertArray.Size();
		int iIndexCount = mRenderOp->iPrimCount * 3;

		VertexDeclarationPtr vdecl = mWater->GetVertexDecl();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * sizeof(Vec3));
		Vec3 * vert = (Vec3 *)vb->Lock(0, 0, LOCK_DISCARD);
		{
			for (int i = 0; i < iVertexCount; ++i)
			{
				vert[i] = wiv.vertArray[i].position;
			}
		}
		vb->Unlock();

		mRenderOp->vxStream.SetDeclaration(vdecl);
		mRenderOp->vxStream.Bind(0, vb, sizeof(Vec3));
		mRenderOp->vxStream.SetCount(iVertexCount);

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
		short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
		{
			for (int i = 0; i < indexArray.Size(); i += 4)
			{ 
				*idx++ = indexArray[i + 0];
				*idx++ = indexArray[i + 1];
				*idx++ = indexArray[i + 2];

				*idx++ = indexArray[i + 2];
				*idx++ = indexArray[i + 1];
				*idx++ = indexArray[i + 3];
			}
		}
		ib->Unlock();

		mRenderOp->ixStream.Bind(ib, 0);
		mRenderOp->ixStream.SetCount(iIndexCount);
	}






	Water::Water()
		: mData(NULL)
		, mBlocks(NULL)
		, mHeight(0)
		, mRenderRefl(false)
		, OnPreRender(RenderEvent::OnPreRenderEvent, this, &Water::_PreRender)
		, OnOptimizeCull(RenderEvent::OnOptimizeMirrorCullResult, this, &Water::_OnOptimizeCullResult)
	{
	}

	Water::~Water()
	{
		Shutdown();
	}

	void Water::SetHeight(float height)
	{
		mHeight = height;
	}

	void Water::Init()
	{
		Shutdown();

		Terrain * terrain = Environment::Instance()->GetTerrain();

		if (!terrain)
			return ;

		mSizeX = terrain->GetConfig().xVertexCount - 1;
		mSizeZ = terrain->GetConfig().zVertexCount - 1;

		mGridSize = terrain->GetConfig().xSize / mSizeX;

		mData = new char[mSizeX * mSizeZ];

		ZeroMemory(mData, mSizeX * mSizeZ);

		_allocBlock();

		mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Water");
		mTech_UnderWater = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWater");
		mTex_Wave = VideoBufferManager::Instance()->Load2DTexture("Water\\PerlinNormalHeight.dds", "Water\\PerlinNormalHeight.dds");
		mTex_Fresnel = VideoBufferManager::Instance()->Load2DTexture("Water\\Fresnel.bmp", "Water\\Fresnel.bmp");
		mTex_Normal0 = mTex_Wave;//VideoBufferManager::Instance()->Load2DTexture("Water\\WaterNormal1.tga", "Water\\WaterNormal1.tga");
		mTex_Normal1 = mTex_Wave;//VideoBufferManager::Instance()->Load2DTexture("Water\\WaterNormal2.tga", "Water\\WaterNormal2.tga");

		d_assert (mTech != NULL && mTex_Normal0 != NULL && mTech_UnderWater != NULL);

		_initRT();
	}

	void Water::Shutdown()
	{
		safe_delete_array (mData);
		safe_delete_array (mBlocks);
		mVertexDecl = NULL;
	}

	void Water::Resize(int w, int h)
	{
		_initRT();
	}

	void Water::_initRT()
	{
		if (mData == NULL)
			return ;

		mTex_Refl = NULL;
		mDepthStencil = NULL;
		mRT_Refl = NULL;

		RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

		int width = scheme->GetColorTexture()->GetWidth() / 2;
		int height = scheme->GetColorTexture()->GetHeight() / 2;

		mTex_Refl = VideoBufferManager::Instance()->CreateTextureRT("_Core_Water_Tex", width, height);
		mDepthStencil = VideoBufferManager::Instance()->CreateDepthStencil("_Core_Water_DS", width, height, FMT_D24S8, MSAA_NONE);
		mRT_Refl = VideoBufferManager::Instance()->CreateRenderTarget(mTex_Refl);
	}

	void Water::Load(const char * source)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		if (stream == NULL)
			return ;

		int Magic, Version;

		stream->Read(&Magic, sizeof(int));
		stream->Read(&Version, sizeof(int));

		d_assert (Magic == K_Magic);

		d_assert (Version == 0);

		if (Version == 0)
		{
			int sizeX, sizeZ;

			stream->Read(&sizeX, sizeof(int));
			stream->Read(&sizeZ, sizeof(int));

			d_assert (sizeX == mSizeX && sizeZ == mSizeZ);

			stream->Read(&mHeight, sizeof(float));

			stream->Read(mData, sizeX * sizeZ);
		}

		_initBlock();
	}

	void Water::Save(const char * source)
	{
		File file;

		file.Open(source);

		d_assert (file.IsOpen());

		int K_Version = 0;

		file.Write(&K_Magic, sizeof(int));
		file.Write(&K_Version, sizeof(int));

		file.Write(&mSizeX, sizeof(int));
		file.Write(&mSizeZ, sizeof(int));

		file.Write(&mHeight, sizeof(float));

		file.Write(mData, mSizeX * mSizeZ);
	}

	char Water::GetData(int i, int j)
	{
		d_assert (i < mSizeX && j < mSizeZ && mData);

		return mData[j * mSizeX + i];
	}

	void Water::SetData(int i, int j, char v)
	{
		d_assert (i < mSizeX && j < mSizeZ && mData);

		mData[j * mSizeX + i] = v;
	}

	void Water::MapCoord(int & _x, int & _z, float x, float z)
	{
		_x = int((x / mGridSize));
		_z = int((z / mGridSize));
	}

	bool Water::HasWater(float x, float z)
	{
		int i = int((x / mGridSize));
		int j = int((z / mGridSize));

		if (i < 0 || i >= mSizeX ||
			j < 0 || j >= mSizeZ)
			return false;

		i /= kBlockGridSize;
		j /= kBlockGridSize;

		WaterBlock * block = &mBlocks[j * mBlockCountX + i];

		return block->mRenderOp != NULL;
	}

	void Water::NotifyChanged(const Rect & rc)
	{
		for (int i = 0; i < mBlockCountX * mBlockCountZ; ++i)
		{
			int x1 = mBlocks[i].mIndexX * kBlockGridSize;
			int y1 = mBlocks[i].mIndexZ * kBlockGridSize;
			int x2 = x1 + kBlockGridSize - 1;
			int y2 = y1 + kBlockGridSize - 1;

			if (rc.x1 > x2 || rc.x2 < x1 ||
				rc.y1 > y2 || rc.y2 < y1)
				continue ;

			mBlocks[i].Init(this, mBlocks[i].mIndexX, mBlocks[i].mIndexZ);
		}
	}

	VertexDeclarationPtr Water::GetVertexDecl()
	{
		if (mVertexDecl == NULL)
		{
			mVertexDecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
			mVertexDecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
			mVertexDecl->Init();
		}

		return mVertexDecl;
	}

	void Water::_allocBlock()
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();

		mBlockCountX = terrain->GetConfig().xSectionCount;
		mBlockCountZ = terrain->GetConfig().zSectionCount;

		mBlocks = new WaterBlock[terrain->GetConfig().iSectionCount];

		_initBlock();
	}

	void Water::_initBlock()
	{
		int index = 0;
		for (int z = 0; z < mBlockCountZ; ++z)
		{
			for (int x = 0; x < mBlockCountX; ++x)
			{
				mBlocks[index++].Init(this, x, z);
			}
		}
	}

	void Water::Render(Texture * depthTex, Texture * colorTex)
	{
		RS_RenderEvent(RenderWater);

		RenderSystem * render = RenderSystem::Instance();
		Camera * cam = World::Instance()->MainCamera();

		bool upWater = !WaterManager::Instance()->IsUnderWater();

		int index = 0;

		ShaderParam * gHeight = mTech->GetVertexShaderParamTable()->GetParam("gHeight");

		gHeight->SetUnifom(mHeight, 0, 0, 0);

		for (int z = 0; z < mBlockCountZ; ++z)
		{
			for (int x = 0; x < mBlockCountX; ++x)
			{
				WaterBlock * block = &mBlocks[index++];

				block->mBound.minimum.y = block->mBound.maximum.y = mHeight;

				if (block->mRenderOp && cam->IsVisible(block->mBound))
				{
					if (upWater)
						_renderUpWater(block, depthTex, colorTex);
					else
						_renderUnderWater(block, depthTex, colorTex);
				}
			}
		}
	}


	void Water::_PreRender(Event * sender)
	{
		if (!WaterManager::Instance()->IsUnderWater())
			_renderRelfection();
	}

	void Water::_renderUnderWater(WaterBlock * block, Texture * depthTex, Texture * colorTex)
	{
	}

	void Water::_renderUpWater(WaterBlock * block, Texture * depthTex, Texture * colorTex)
	{
		RenderSystem * render = RenderSystem::Instance();
		Camera * cam = World::Instance()->MainCamera();
		float time = Engine::Instance()->GetTime();

		const Vec3 * worldCorner = cam->GetWorldCorner();
		const Vec3 & camPos = cam->GetPosition();

		Vec3 worldCornerLeftTop = worldCorner[4] - camPos;
		Vec3 worldCornerRightDir = worldCorner[5] - worldCorner[4];
		Vec3 worldCornerDownDir = worldCorner[6] - worldCorner[4];

		const Vec3 * viewCorner = cam->GetCorner();

		Vec3 viewCornerLeftTop = viewCorner[4];
		Vec3 viewCornerRightDir = viewCorner[5] - viewCorner[4];
		Vec3 viewCornerDownDir = viewCorner[6] - viewCorner[4];

		ShaderParam * uFresnelParam = mTech->GetPixelShaderParamTable()->GetParam("gFresnelParam");
		ShaderParam * uCamPos = mTech->GetPixelShaderParamTable()->GetParam("gCameraPos");
		ShaderParam * uWorldCornerLeftTop = mTech->GetPixelShaderParamTable()->GetParam("gWorldCornerLeftTop");
		ShaderParam * uWorldCornerRightDir = mTech->GetPixelShaderParamTable()->GetParam("gWorldCornerRightDir");
		ShaderParam * uWorldCornerDownDir = mTech->GetPixelShaderParamTable()->GetParam("gWorldCornerDownDir");
		ShaderParam * uViewCornerLeftTop = mTech->GetPixelShaderParamTable()->GetParam("gViewCornerLeftTop");
		ShaderParam * uViewCornerRightDir = mTech->GetPixelShaderParamTable()->GetParam("gViewCornerRightDir");
		ShaderParam * uViewCornerDownDir = mTech->GetPixelShaderParamTable()->GetParam("gViewCornerDownDir");
		ShaderParam * uDeepColor = mTech->GetPixelShaderParamTable()->GetParam("gDeepColor");
		ShaderParam * uRefractionDist = mTech->GetPixelShaderParamTable()->GetParam("gRefractionDist");

		ShaderParam * uSunDir = mTech->GetPixelShaderParamTable()->GetParam("gSunDir");
		ShaderParam * uSunColor = mTech->GetPixelShaderParamTable()->GetParam("gSunColor");
		ShaderParam * uSunLightParam = mTech->GetPixelShaderParamTable()->GetParam("gSunLightParam");

		ShaderParam * uNoiseScale = mTech->GetPixelShaderParamTable()->GetParam("gNoiseScale");
		ShaderParam * uNoiseUVParam = mTech->GetPixelShaderParamTable()->GetParam("gNoiseUVParam");

		ShaderParam * uNormalUVParam = mTech->GetPixelShaderParamTable()->GetParam("gNormalUVParam");
		ShaderParam * uNormalWeight = mTech->GetPixelShaderParamTable()->GetParam("gNormalWeight");

		ShaderParam * uInvScreenSize = mTech->GetPixelShaderParamTable()->GetParam("gInvScreenSize");
		ShaderParam * uFarClip = mTech->GetPixelShaderParamTable()->GetParam("gFarClip");

		Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
		Color4 sunColor = Environment::Instance()->GetEvParam()->SunColor;
		Color4 deepColor = Environment::Instance()->GetEvParam()->WaterParam.deepColor;

		uCamPos->SetUnifom(camPos.x, camPos.y, camPos.z, 0);

		uWorldCornerLeftTop->SetUnifom(worldCornerLeftTop.x, worldCornerLeftTop.y, worldCornerLeftTop.z, 0);
		uWorldCornerRightDir->SetUnifom(worldCornerRightDir.x, worldCornerRightDir.y, worldCornerRightDir.z, 0);
		uWorldCornerDownDir->SetUnifom(worldCornerDownDir.x, worldCornerDownDir.y, worldCornerDownDir.z, 0);

		uViewCornerLeftTop->SetUnifom(viewCornerLeftTop.x, viewCornerLeftTop.y, viewCornerLeftTop.z, 0);
		uViewCornerRightDir->SetUnifom(viewCornerRightDir.x, viewCornerRightDir.y, viewCornerRightDir.z, 0);
		uViewCornerDownDir->SetUnifom(viewCornerDownDir.x, viewCornerDownDir.y, viewCornerDownDir.z, 0);

		uDeepColor->SetUnifom(deepColor.r, deepColor.g, deepColor.b, 1);
		uRefractionDist->SetUnifom(40, 0, 0, 0);

		uSunColor->SetUnifom(sunColor.r, sunColor.g, sunColor.b, 0);
		uSunDir->SetUnifom(-sunDir.x, -sunDir.y, -sunDir.z, 0);
		uSunLightParam->SetUnifom(0.8f, 200, 0, 0);

		float uvNoiseScroll = time * 0.02f;

		uNoiseUVParam->SetUnifom(0.01f, 0.01f, uvNoiseScroll, uvNoiseScroll);
		uNoiseScale->SetUnifom(0.03f, 0, 0 , 0);

		Vec4 uvNormalParam[4] = {
			Vec4 (0.02f, 0.02f, time * 0.03f, time * 0.03f),
			Vec4 (0.01f, 0.01f, time * 0.03f, time * 0.03f),
			Vec4 (0.015f, 0.015f, time * 0.03f, time * 0.03f),
			Vec4 (0.005f, 0.005f, time * 0.03f, time * 0.03f)
		};

		uNormalUVParam->SetVector(uvNormalParam, 4);
		uNormalWeight->SetUnifom(0.08f, 0.1f, 0.06f, 0.1f);

		float screenW = (float)Engine::Instance()->GetDeviceProperty()->Width;
		float screenH = (float)Engine::Instance()->GetDeviceProperty()->Height;

		uInvScreenSize->SetUnifom(1 / screenW, 1 / screenW, 0, 0);
		uFarClip->SetUnifom(cam->GetFarClip(), 0, 0, 0);

		SamplerState state;
		render->SetTexture(0, state, mTex_Wave.c_ptr());
		state.Address = TEXA_CLAMP;
		state.Filter = TEXF_POINT;
		render->SetTexture(1, state, depthTex);
		render->SetTexture(2, state, colorTex);
		render->SetTexture(3, state, mTex_Refl.c_ptr());
		state.Filter = TEXF_LINEAR;
		render->SetTexture(4, state, mTex_Fresnel.c_ptr());

		SamplerState state1;
		render->SetTexture(5, state1, mTex_Wave.c_ptr());
		render->SetTexture(6, state1, mTex_Wave.c_ptr());

		render->Render(mTech, block->mRenderOp);
	}

	void Water::_renderRelfection()
	{
		if (WaterManager::Instance()->IsUnderWater())
			return ;

		mRenderRefl = true;

		RenderSystem * render = RenderSystem::Instance();

		Plane mirrorPlane = Plane(Vec3(0, 1, 0), -mHeight);

		RenderTarget * oldRT = render->GetRenderTarget(0);
		DepthStencil * oldDS = render->GetDepthStencil();

		// ---> Bind RenderTarget
		render->SetRenderTarget(0, mRT_Refl.c_ptr());
		render->SetRenderTarget(1, NULL);
		render->SetRenderTarget(2, NULL);
		render->SetRenderTarget(3, NULL);
		render->SetDepthStencil(mDepthStencil.c_ptr());

		// ---> clear buffer
		render->ClearBuffer(NULL, true, true, false, Color::Black, 1, 0);

		Color4 deepColor = Environment::Instance()->GetEvParam()->WaterParam.reflColor;

		RenderRegister::Instance()->SetMirrorColor(deepColor);

		Engine::Instance()->GetRenderScheme()->RenderInMirror(mirrorPlane);

		mRT_Refl->Stretch(mTex_Refl.c_ptr());

		render->SetRenderTarget(0, oldRT);
		render->SetDepthStencil(oldDS);

		mRenderRefl = false;
	}

	void Water::_OnOptimizeCullResult(Event * _sender)
	{
		VisibleCullResult * result = (VisibleCullResult *)_sender->GetParam(0);

		SceneNodeList & list = result->nodes;

		SceneNodeList::Iterator whr = list.Begin();
		SceneNodeList::Iterator end = list.End();

		while (whr != end)
		{
			SceneNode * node = *whr;

			Vec3 pos = node->GetWorldAabb().GetCenter();

			if (!HasWater(pos.x, pos.z))
				node->_setVisibleMask(false);

			++whr;
		}
	}
}