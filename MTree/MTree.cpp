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

			if (mSpeedTree->Compute(NULL))
			{
				bSuccess = true;

				mSpeedTree->GetBoundingBox(mBoundingBox);

				mSpeedTree->SetLeafRockingState(true);

				CSpeedTreeRT::SetDropToBillboard(false);

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
		_setupLeafGeometry();
	}

	void MTree::_setupTexture()
	{
		mTextureInfo = new CSpeedTreeRT::STextures;
		mSpeedTree->GetTextures(*mTextureInfo);

		TString128 sourceDir = File::GetFileDir(mSourceName);

		// branch texture
		TString128 branchTex = sourceDir + File::GetBaseName(mTextureInfo->m_pBranchTextureFilename);

		mBranchMaterial.DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(branchTex, branchTex);

		// leaf texture
		for (unsigned int i = 0; i < mTextureInfo->m_uiLeafTextureCount && i < MTreeGlobal::K_MaxLeafPerTree; ++i)
		{
			TString128 leafTex = sourceDir + "Texture\\" + File::GetBaseName(mTextureInfo->m_pLeafTextureFilenames[i]);
			
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


	void MTree::_setupLeafGeometry()
	{
		const int c_nVertexShader_LeafTables = 4;

		mNumLeafs = 1;
		const short vIndices[6] = { 0, 1, 2, 0, 2, 3 };
		mNumLeafLods = mSpeedTree->GetNumLeafLodLevels();

		const Vec2 vTexcoord[4] = { Vec2(0, 0), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1) };

		mLeafBuffers = new VertexBufferPtr[mNumLeafLods];

		for (int lod = 0; lod < mNumLeafLods; ++lod)
		{
			unsigned short leafCount = mGeometryCache->m_sLeaves0.m_usLeafCount;

			if (leafCount < 1)
				continue;

			int iVertexCount = leafCount * 6;

			mLeafBuffers[lod] = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(LeafVertex) * iVertexCount);

			LeafVertex * vert = (LeafVertex *)mLeafBuffers[lod]->Lock(0, 0, LOCK_DISCARD);
			for (int i = 0; i < leafCount; ++i)
			{
				const CSpeedTreeRT::SGeometry::SLeaf * leaf = &mGeometryCache->m_sLeaves0;

				for (int v = 0; v < 6; ++v)
				{
					memcpy(&vert->Position, &leaf->m_pCenterCoords[i * 3], sizeof(Vec3));
					memcpy(&vert->Normal, &leaf->m_pNormals[i * 3], sizeof(Vec3));
					//memcpy(&vert->TexCoords, &leaf->m_pLeafMapTexCoords[i][vIndices[v] * 2], 2 * sizeof(float));
					vert->TexCoords[0] = vTexcoord[vIndices[v]].x;
					vert->TexCoords[1] = vTexcoord[vIndices[v]].y;

					vert->WindIndex = 4.0f * leaf->m_pWindMatrixIndices[i];
					vert->WindWeight = leaf->m_pWindWeights[i];

					vert->PlacementIndex = c_nVertexShader_LeafTables + leaf->m_pLeafClusterIndices[i] * 4 + vIndices[v];
					vert->ScalarValue = mSpeedTree->GetLeafLodSizeAdjustments()[lod];

					++vert;
				}
			}
			mLeafBuffers[lod]->Unlock();
		}

		mRenderOp_Leaf = new RenderOp();

		VertexStream & vxStream = mRenderOp_Leaf->vxStream;
		IndexStream & ixStream = mRenderOp_Leaf->ixStream;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
		vdecl->AddElement(0, 24, DT_FLOAT4, DU_TEXCOORD, 0);
		vdecl->AddElement(0, 40, DT_FLOAT2, DU_TEXCOORD, 1);
		vdecl->Init();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, mLeafBuffers[0], sizeof(LeafVertex));
		vxStream.SetCount(mLeafBuffers[0]->GetSize() / sizeof(LeafVertex));

		mRenderOp_Leaf->rState.cullMode = CULL_NONE;
		mRenderOp_Leaf->rState.alphaTestRef = 0.1f;
		mRenderOp_Leaf->rState.blendMode = BM_ALPHA_TEST;
		//mRenderOp_Leaf->rState.fillMode = FILL_FRAME;
		mRenderOp_Leaf->iPrimCount = vxStream.GetCount() / 3;
		mRenderOp_Leaf->ePrimType = PRIM_TRIANGLELIST;
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

	RenderOp * MTree::_getLeafRenderOp(int lod)
	{
		if (!mRenderOp_Leaf)
			return NULL;

		if (lod > mNumLeafLods - 1)
			lod = mNumLeafLods - 1;

		mRenderOp_Leaf->vxStream.Bind(0, mLeafBuffers[lod], sizeof(LeafVertex));
		mRenderOp_Leaf->vxStream.SetCount(mLeafBuffers[lod]->GetSize() / sizeof(LeafVertex));
		mRenderOp_Leaf->iPrimCount = mRenderOp_Leaf->vxStream.GetCount() / 3;

		return mRenderOp_Leaf;
	}

	void MTree::PrepareRenderLeaf(ShaderParam * bdTabel)
	{
		unsigned int count = 0;
		const Vec4 * table = (const Vec4 *)mSpeedTree->GetLeafBillboardTable(count);

		bdTabel->SetVector(table, count);
	}
}