#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

namespace Myway {

	class WaterBlock
	{
		friend class Water;

		DECLARE_ALLOC();

	public:
		WaterBlock();
		virtual ~WaterBlock();

		void Init(Water* water, int x, int z);
		void Shudown();

	protected:
		RenderOp  * mRenderOp;
		Aabb mBound;

		int mIndexX;
		int mIndexZ;
		Water * mWater;
	};

    class MRENDER_ENTRY Water
	{
		static const int K_Magic = 'wtr0';

		DECLARE_ALLOC();

	public:
		Water();
		~Water();

		void SetHeight(float height);
		float GetHeight() const { return mHeight; }

		void Init();
		void Shutdown();

		void Resize(int w, int h);

		void Load(const char * source);
		void Save(const char * source);

		char GetData(int i, int j);
		void SetData(int i, int j, char v);

		void NotifyChanged(const Rect & rc);

		VertexDeclarationPtr GetVertexDecl();

		int GetSizeX() { return mSizeX; }
		int GetSizeZ() { return mSizeZ; }
		int GetBlockCountX() { return mBlockCountX; }
		int GetBlockCountZ() { return mBlockCountZ; }
		float GetGridSize() { return mGridSize; }

		void Render(Texture * depthTex, Texture * colorTex);

	protected:
		void _allocBlock();
		void _initBlock();

		void _initRT();

		void _PreRender(Event * sender);

		void _renderUnderWater(WaterBlock * block, Texture * depthTex, Texture * colorTex);
		void _renderUpWater(WaterBlock * block, Texture * depthTex, Texture * colorTex);

		void _renderRelfection();

	protected:
		tEventListener<Water> OnPreRender;

		float mHeight;
	
		int mSizeX;
		int mSizeZ;
		char * mData;
		
		float mGridSize;

		int mBlockCountX;
		int mBlockCountZ;
		WaterBlock * mBlocks;

		VertexDeclarationPtr mVertexDecl;


		TexturePtr mTex_Wave;
		TexturePtr mTex_Fresnel;
		TexturePtr mTex_Normal0;
		TexturePtr mTex_Normal1;

		RenderTargetPtr mRT_Refl;
		TexturePtr mTex_Refl;
		DepthStencilPtr mDepthStencil;

		Technique * mTech;
		Technique * mTech_UnderWater;
	};

}