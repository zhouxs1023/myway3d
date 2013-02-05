#include "MTree.h"
#include "MWResourceManager.h"
#include "MWVideoBufferManager.h"
#include "MForest.h"

namespace Myway {

	static const float c_fNearLodFactor = 1.0f;
	static const float c_fFarLodFactor = 5.0f;

	MTree::MTree(const TString128 & source)
	{
		mSourceName = source;

		mRenderOp_Branch = NULL;
		mSpeedTree = new CSpeedTreeRT();

		mTextureInfo = NULL;
		mGeometryCache = NULL;

		memset(mBoundingBox, 0, sizeof(float) * 6);
	}

	MTree::~MTree()
	{
		safe_delete (mSpeedTree);
		safe_delete (mRenderOp_Branch);

		safe_delete (mRenderOp_Leaf0);
		safe_delete (mRenderOp_Leaf1);
	}

	void MTree::DeleteSelf()
	{
		MForest::Instance()->DeleteTree(this);
	}

	void MTree::Load()
	{
		if (mLoadState == Resource::LOADED)
			return ;

		if (!CanLoad())
		{
			mLoadState = Resource::LOADED;
			return ;
		}

		ResourceManager::Instance()->GetResourceLoader()->Load(this);
	}

	void MTree::Reload()
	{
		if (!CanLoad())
			return ;

		if (mLoadState == Resource::LOADED)
			Unload();

		ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
	}

	void MTree::Unload()
	{
		if (!CanLoad())
			return ;
	}

	void MTree::LoadImp(DataStreamPtr stream)
	{
		bool bSuccess = false;

		if (mSpeedTree->LoadTree((const unsigned char *)stream->GetData(), stream->Size()))
		{
			mSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
			mSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
			mSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);

			mSpeedTree->SetBranchWindMethod(CSpeedTreeRT::WIND_GPU);
			mSpeedTree->SetLeafWindMethod(CSpeedTreeRT::WIND_GPU);
			mSpeedTree->SetFrondWindMethod(CSpeedTreeRT::WIND_GPU);

			mSpeedTree->SetNumWindMatrices(MTreeGlobal::K_NumWindMatrix);

			//mSpeedTree->SetLeafRockingState(false);
			mSpeedTree->SetNumLeafRockingGroups(2);
			CSpeedTreeRT::SetDropToBillboard(true);

			if (mSpeedTree->Compute(NULL))
			{
				bSuccess = true;

				mSpeedTree->GetBoundingBox(mBoundingBox);

				mBranchMaterial.Set(mSpeedTree->GetBranchMaterial( ));
				mFrondMaterial.Set(mSpeedTree->GetFrondMaterial( ));

				for (int i = 0; i < MTreeGlobal::K_MaxLeafPerTree; ++i)
				{
					mLeafMaterial[i].Set(mSpeedTree->GetLeafMaterial());
				}

				float fHeight = mBoundingBox[5] - mBoundingBox[2];
				mSpeedTree->SetLodLimits(fHeight * c_fNearLodFactor, fHeight * c_fFarLodFactor);

				_setupGeometry();
				_setupTexture();

				safe_delete (mTextureInfo);
				safe_delete (mGeometryCache);
			}
		}

		if (!bSuccess)
			LOG_PRINT_FORMAT("Tree: Fatal Error, cannot compute tree [%s]\n\n", CSpeedTreeRT::GetCurrentError());
	}

	void MTree::_setupGeometry()
	{
		if (mGeometryCache == NULL)
			mGeometryCache = new CSpeedTreeRT::SGeometry;

		mSpeedTree->GetGeometry(*mGeometryCache);

		_setupBranchGeometry();
		_setupLeafGeometry0();
		_setupLeafGeometry1();
	}

	void MTree::_setupTexture()
	{
		mTextureInfo = new CSpeedTreeRT::STextures;
		mSpeedTree->GetTextures(*mTextureInfo);

		TString128 sourceDir = File::GetFileDir(mSourceName);

		// branch texture
		TString128 branchTex = sourceDir + "Texture\\" + File::RemoveExternName(File::GetBaseName(mTextureInfo->m_pBranchTextureFilename)) + ".dds";

		mBranchMaterial.DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(branchTex, branchTex);

		// leaf texture
		for (unsigned int i = 0; i < mTextureInfo->m_uiLeafTextureCount && i < MTreeGlobal::K_MaxLeafPerTree; ++i)
		{
			TString128 leafTex = sourceDir + "Texture\\" + File::RemoveExternName(File::GetBaseName(mTextureInfo->m_pLeafTextureFilenames[i])) + ".dds";
			
			mLeafMaterial[i].DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(leafTex, leafTex);
		}
	}

	void MTree::_setupBranchGeometry()
	{
		CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(mGeometryCache->m_sBranches);

		unsigned short iVertexCount = pBranches->m_usVertexCount;

		mRenderOp_Branch = new RenderOp();

		VertexStream & vxStream = mRenderOp_Branch->vxStream;
		IndexStream & ixStream = mRenderOp_Branch->ixStream;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
		vdecl->AddElement(0, 24, DT_FLOAT3, DU_TANGENT, 0);
		vdecl->AddElement(0, 36, DT_FLOAT4, DU_TEXCOORD, 0);
		vdecl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(BranchVertex) * iVertexCount);

		BranchVertex * verts = (BranchVertex *)vb->Lock(0, 0, LOCK_NORMAL);
		for (int i = 0; i < iVertexCount; ++i)
		{
			memcpy(&verts[i].Position, &pBranches->m_pCoords[i * 3], sizeof(Vec3));
			memcpy(&verts[i].Normal, &pBranches->m_pNormals[i * 3], sizeof(Vec3));
			memcpy(&verts[i].Tangent, &pBranches->m_pTangents[i * 3], sizeof(Vec3));

			verts[i].TexCoords[0] = pBranches->m_pTexCoords0[i * 2];
			verts[i].TexCoords[1] = pBranches->m_pTexCoords0[i * 2 + 1];

			verts[i].WindIndex = 4.0f * pBranches->m_pWindMatrixIndices[i];
			verts[i].WindWeight = (float)pBranches->m_pWindWeights[i];
		}
		vb->Unlock();


		mNumBranchLods = mSpeedTree->GetNumBranchLodLevels();
		mBranchIndexCounts = new unsigned short[mNumBranchLods];

		mSpeedTree->GetGeometry(*mGeometryCache, SpeedTree_BranchGeometry, 0);

		if (pBranches->m_usNumStrips > 0)
			mBranchIndexCounts[0] = pBranches->m_pStripLengths[0];
		else
			mBranchIndexCounts[0] = 0;

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof(short) * mBranchIndexCounts[0]);

		short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);
		{
			memcpy(idx, pBranches->m_pStrips[0], mBranchIndexCounts[0] * sizeof(unsigned short));
		}

		ib->Unlock();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vb, sizeof(BranchVertex));
		vxStream.SetCount(iVertexCount);

		ixStream.Bind(ib, 0);
		ixStream.SetCount(mBranchIndexCounts[0]);

		mRenderOp_Branch->rState.cullMode = CULL_CW;
		//mRenderOp_Branch->rState.fillMode = FILL_FRAME;
		mRenderOp_Branch->iPrimCount = mBranchIndexCounts[0] - 2;
		mRenderOp_Branch->ePrimType = PRIM_TRIANGLESTRIP;

		for (int i = 1; i < mNumBranchLods; ++i)
		{
			// force geometry update for this LOD
			mSpeedTree->GetGeometry(*mGeometryCache, SpeedTree_BranchGeometry, (short)i);

			// check if this LOD has branches
			if (pBranches->m_usNumStrips > 0)
				mBranchIndexCounts[i] = pBranches->m_pStripLengths[0];
			else
				mBranchIndexCounts[i] = 0;
		}
	}

	void MTree::_setupLeafGeometry0()
	{
		const int c_nVertexShader_LeafTables = 4;

		const short vIndices[6] = { 0, 1, 2, 0, 2, 3 };
		mNumLeafLods = mSpeedTree->GetNumLeafLodLevels();

		const Vec2 vTexcoord[4] = { Vec2(0, 0), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1) };

		mSpeedTree->GetGeometry(*mGeometryCache, SpeedTree_LeafGeometry, 0);

		unsigned short leafCount = mGeometryCache->m_sLeaves0.m_usLeafCount;

		if (leafCount < 1)
			return ;

		int iVertexCount = leafCount * 6;

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(LeafVertex) * iVertexCount);

		for (int lod = 0; lod < 1/*mNumLeafLods*/; ++lod)
		{
			unsigned int count = 0;
			const Vec4 * table = (const Vec4 *)mSpeedTree->GetLeafBillboardTable(count);

			LeafVertex * vert = (LeafVertex *)vb->Lock(0, 0, LOCK_DISCARD);
			for (int i = 0; i < leafCount; ++i)
			{
				const CSpeedTreeRT::SGeometry::SLeaf * leaf = &mGeometryCache->m_sLeaves0;

				Vec2 vMin = Vec2::Zero, vMax = Vec2::Zero;

				for (int v = 0; v < 6; ++v)
				{
					memcpy(&vert[v].Position, &leaf->m_pCenterCoords[i * 3], sizeof(Vec3));
					memcpy(&vert[v].Normal, &leaf->m_pNormals[i * 3], sizeof(Vec3));
					//memcpy(&vert->TexCoords, &leaf->m_pLeafMapTexCoords[i][vIndices[v] * 2], 2 * sizeof(float));
					vert[v].TexCoords[0] = vTexcoord[vIndices[v]].x;
					vert[v].TexCoords[1] = vTexcoord[vIndices[v]].y;

					vert[v].WindIndex = 4.0f * leaf->m_pWindMatrixIndices[i];
					vert[v].WindWeight = leaf->m_pWindWeights[i];

					int PlacementIndex = c_nVertexShader_LeafTables + leaf->m_pLeafClusterIndices[i] * 4 + vIndices[v];
					float ScalarValue = mSpeedTree->GetLeafLodSizeAdjustments()[lod];

					float w = table[PlacementIndex].x * ScalarValue;
					float h = table[PlacementIndex].y * ScalarValue;

					vMin.x = Math::Minimum(vMin.x, w);
					vMin.y = Math::Minimum(vMin.x, h);
					vMax.x = Math::Maximum(vMax.x, w);
					vMax.y = Math::Maximum(vMax.y, h);
				}

				Vec2 vHalfSize = (vMax - vMin);

				d_assert (vHalfSize.x > 0.1f && vHalfSize.y > 0.1f);

				for (int v = 0; v < 6; ++v)
				{
					vert[v].Width = vHalfSize.x;
					vert[v].Height = vHalfSize.y;
				}

				vert += 6;
			}
			vb->Unlock();
		}

		mRenderOp_Leaf0 = new RenderOp();

		VertexStream & vxStream = mRenderOp_Leaf0->vxStream;
		IndexStream & ixStream = mRenderOp_Leaf0->ixStream;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
		vdecl->AddElement(0, 24, DT_FLOAT4, DU_TEXCOORD, 0);
		vdecl->AddElement(0, 40, DT_FLOAT2, DU_TEXCOORD, 1);
		vdecl->Init();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vb, sizeof(LeafVertex));
		vxStream.SetCount(vb->GetSize() / sizeof(LeafVertex));

		mRenderOp_Leaf0->rState.cullMode = CULL_NONE;
		mRenderOp_Leaf0->rState.alphaTestRef = 0.1f;
		mRenderOp_Leaf0->rState.blendMode = BM_ALPHA_TEST;
		//mRenderOp_Leaf->rState.fillMode = FILL_FRAME;
		mRenderOp_Leaf0->iPrimCount = vxStream.GetCount() / 3;
		mRenderOp_Leaf0->ePrimType = PRIM_TRIANGLELIST;
	}

	void MTree::_setupLeafGeometry1()
	{
		const int c_nVertexShader_LeafTables = 4;

		const short vIndices[6] = { 0, 1, 2, 0, 2, 3 };
		mNumLeafLods = mSpeedTree->GetNumLeafLodLevels();

		const Vec2 vTexcoord[4] = { Vec2(0, 0), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1) };

		unsigned short leafCount = mGeometryCache->m_sLeaves1.m_usLeafCount;

		if (leafCount < 1)
			return ;

		int iVertexCount = leafCount * 6;

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(LeafVertex) * iVertexCount);

		for (int lod = 0; lod < 1/*mNumLeafLods*/; ++lod)
		{
			unsigned int count = 0;
			const Vec4 * table = (const Vec4 *)mSpeedTree->GetLeafBillboardTable(count);

			LeafVertex * vert = (LeafVertex *)vb->Lock(0, 0, LOCK_DISCARD);
			for (int i = 0; i < leafCount; ++i)
			{
				const CSpeedTreeRT::SGeometry::SLeaf * leaf = &mGeometryCache->m_sLeaves1;

				Vec2 vMin = Vec2::Zero, vMax = Vec2::Zero;

				for (int v = 0; v < 6; ++v)
				{
					memcpy(&vert[v].Position, &leaf->m_pCenterCoords[i * 3], sizeof(Vec3));
					memcpy(&vert[v].Normal, &leaf->m_pNormals[i * 3], sizeof(Vec3));
					//memcpy(&vert->TexCoords, &leaf->m_pLeafMapTexCoords[i][vIndices[v] * 2], 2 * sizeof(float));
					vert[v].TexCoords[0] = vTexcoord[vIndices[v]].x;
					vert[v].TexCoords[1] = vTexcoord[vIndices[v]].y;

					vert[v].WindIndex = 4.0f * leaf->m_pWindMatrixIndices[i];
					vert[v].WindWeight = leaf->m_pWindWeights[i];

					int PlacementIndex = c_nVertexShader_LeafTables + leaf->m_pLeafClusterIndices[i] * 4 + vIndices[v];
					float ScalarValue = mSpeedTree->GetLeafLodSizeAdjustments()[lod];

					float w = table[PlacementIndex].x * ScalarValue;
					float h = table[PlacementIndex].y * ScalarValue;

					vMin.x = Math::Minimum(vMin.x, w);
					vMin.y = Math::Minimum(vMin.x, h);
					vMax.x = Math::Maximum(vMax.x, w);
					vMax.y = Math::Maximum(vMax.y, h);
				}

				Vec2 vHalfSize = (vMax - vMin) * 0.5f;

				d_assert (vHalfSize.x > 0.1f && vHalfSize.y > 0.1f);

				for (int v = 0; v < 6; ++v)
				{
					vert[v].Width = vHalfSize.x;
					vert[v].Height = vHalfSize.y;
				}

				vert += 6;
			}
			vb->Unlock();
		}

		mRenderOp_Leaf1 = new RenderOp();

		VertexStream & vxStream = mRenderOp_Leaf1->vxStream;
		IndexStream & ixStream = mRenderOp_Leaf1->ixStream;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
		vdecl->AddElement(0, 24, DT_FLOAT4, DU_TEXCOORD, 0);
		vdecl->AddElement(0, 40, DT_FLOAT2, DU_TEXCOORD, 1);
		vdecl->Init();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vb, sizeof(LeafVertex));
		vxStream.SetCount(vb->GetSize() / sizeof(LeafVertex));

		mRenderOp_Leaf1->rState.cullMode = CULL_NONE;
		mRenderOp_Leaf1->rState.alphaTestRef = 0.1f;
		mRenderOp_Leaf1->rState.blendMode = BM_ALPHA_TEST;
		//mRenderOp_Leaf->rState.fillMode = FILL_FRAME;
		mRenderOp_Leaf1->iPrimCount = vxStream.GetCount() / 3;
		mRenderOp_Leaf1->ePrimType = PRIM_TRIANGLELIST;
	}

	RenderOp * MTree::_getBranchRenderOp(int lod)
	{
		if (!mRenderOp_Branch)
			return NULL;

		if (lod > mNumBranchLods - 1)
			lod = mNumBranchLods - 1;

		mRenderOp_Branch->iPrimCount = mBranchIndexCounts[lod] - 2;

		return mRenderOp_Branch;
	}

	RenderOp * MTree::_getLeafRenderOp0(int lod)
	{
		if (!mRenderOp_Leaf0)
			return NULL;

		return mRenderOp_Leaf0;
	}

	RenderOp * MTree::_getLeafRenderOp1(int lod)
	{
		if (!mRenderOp_Leaf1)
			return NULL;

		return mRenderOp_Leaf1;
	}

}