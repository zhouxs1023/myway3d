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

		mNumBranchLods = 0;
		mRenderOp_Branch = NULL;
		mRenderOp_Frond = NULL;
		mRenderOp_Leaf = NULL;

		mSpeedTree = new CSpeedTreeRT();

		mTextureInfo = NULL;
		mGeometryCache = NULL;

		memset(mBoundingBox, 0, sizeof(float) * 6);
	}

	MTree::~MTree()
	{
		Unload();
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

		Load();
	}

	void MTree::Unload()
	{
		if (!CanLoad())
			return ;

		safe_delete (mGeometryCache);
		safe_delete (mTextureInfo);

		safe_delete (mSpeedTree);
		safe_delete (mRenderOp_Branch);
		safe_delete (mRenderOp_Frond);
		safe_delete (mRenderOp_Leaf);
	}

	void MTree::LoadImp(DataStreamPtr stream)
	{
		bool bSuccess = false;

		if (mSpeedTree->LoadTree((const unsigned char *)stream->GetData(), stream->Size()))
		{
			mSpeedTree->SetTextureFlip(true);

			mSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
			mSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
			mSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);

			mSpeedTree->SetBranchWindMethod(CSpeedTreeRT::WIND_GPU);
			mSpeedTree->SetLeafWindMethod(CSpeedTreeRT::WIND_GPU);
			mSpeedTree->SetFrondWindMethod(CSpeedTreeRT::WIND_GPU);

			mSpeedTree->SetNumWindMatrices(MTreeGlobal::K_NumWindMatrix);

			mSpeedTree->SetNumLeafRockingGroups(1);

			if (mSpeedTree->Compute(NULL))
			{
				bSuccess = true;

				mSpeedTree->GetBoundingBox(mBoundingBox);

				CSpeedTreeRT::SetDropToBillboard(true);

				mBranchMaterial.Set(mSpeedTree->GetBranchMaterial( ));
				mFrondMaterial.Set(mSpeedTree->GetFrondMaterial( ));
				mLeafMaterial.Set(mSpeedTree->GetLeafMaterial());
				
				float fHeight = mBoundingBox[5] - mBoundingBox[2];
				mSpeedTree->SetLodLimits(fHeight * c_fNearLodFactor, fHeight * c_fFarLodFactor);

				_setupTexture();
				_setupGeometry();
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
		_setupFrondGeometry();
	}

	void MTree::_setupTexture()
	{
		mTextureInfo = new CSpeedTreeRT::STextures;
		mSpeedTree->GetTextures(*mTextureInfo);

		TString128 sourceDir = File::GetFileDir(mSourceName);

		// branch texture
		TString128 branchTex = sourceDir + "Texture\\" + File::RemoveExternName(File::GetBaseName(mTextureInfo->m_pBranchTextureFilename)) + ".dds";

		mBranchMaterial.DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(branchTex, branchTex);

		TString128 compositeTex = sourceDir + "Texture\\" + File::RemoveExternName(File::GetBaseName(mTextureInfo->m_pCompositeFilename)) + ".dds";
		
		TexturePtr comTex = VideoBufferManager::Instance()->Load2DTexture(compositeTex, compositeTex);
		mFrondMaterial.DiffuseMap = comTex;
		mLeafMaterial.DiffuseMap = comTex;
	}

	void MTree::_setupBranchGeometry()
	{
		CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(mGeometryCache->m_sBranches);

		unsigned short iVertexCount = pBranches->m_usVertexCount;

		if (iVertexCount == 0)
			return ;

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

		if (mNumBranchLods > MTreeGlobal::K_MaxLods)
			mNumBranchLods = MTreeGlobal::K_MaxLods;

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
		mRenderOp_Branch->rState.alphaTestRef = mGeometryCache->m_fBranchAlphaTestValue / 255.0f;
		mRenderOp_Branch->rState.blendMode = BM_ALPHA_TEST;
		//mRenderOp_Branch->rState.fillMode = FILL_FRAME;

		mRenderOp_Branch->iPrimCount = mBranchIndexCounts[0] - 2;
		mRenderOp_Branch->ePrimType = PRIM_TRIANGLESTRIP;

		_setupColMesh();

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

	void  MTree::_setupFrondGeometry()
	{
		CSpeedTreeRT::SGeometry::SIndexed* pFronds = &(mGeometryCache->m_sFronds);

		unsigned short iVertexCount = pFronds->m_usVertexCount;

		if (iVertexCount == 0)
			return ;

		mRenderOp_Frond = new RenderOp();

		VertexStream & vxStream = mRenderOp_Frond->vxStream;
		IndexStream & ixStream = mRenderOp_Frond->ixStream;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
		vdecl->AddElement(0, 24, DT_FLOAT4, DU_TEXCOORD, 0);
		vdecl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(FrondVertex) * iVertexCount);

		FrondVertex * verts = (FrondVertex *)vb->Lock(0, 0, LOCK_NORMAL);
		for (int i = 0; i < iVertexCount; ++i)
		{
			memcpy(&verts[i].Position, &pFronds->m_pCoords[i * 3], sizeof(Vec3));
			memcpy(&verts[i].Normal, &pFronds->m_pNormals[i * 3], sizeof(Vec3));

			verts[i].TexCoords[0] = pFronds->m_pTexCoords0[i * 2];
			verts[i].TexCoords[1] = pFronds->m_pTexCoords0[i * 2 + 1];

			verts[i].WindIndex = 4.0f * pFronds->m_pWindMatrixIndices[i];
			verts[i].WindWeight = (float)pFronds->m_pWindWeights[i];
		}
		vb->Unlock();

		int iIndexCount = pFronds->m_pStripLengths[0];

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof(short) * iIndexCount);

		short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);
		{
			memcpy(idx, pFronds->m_pStrips[0], iIndexCount * sizeof(unsigned short));
		}

		ib->Unlock();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vb, sizeof(FrondVertex));
		vxStream.SetCount(iVertexCount);

		ixStream.Bind(ib, 0);
		ixStream.SetCount(iIndexCount);

		mRenderOp_Frond->rState.cullMode = CULL_NONE;
		mRenderOp_Frond->rState.alphaTestRef = mGeometryCache->m_fFrondAlphaTestValue / 255.0f;
		mRenderOp_Frond->rState.blendMode = BM_ALPHA_TEST;
		//mRenderOp_Frond->rState.fillMode = FILL_FRAME;

		mRenderOp_Frond->iPrimCount = iIndexCount - 2;
		mRenderOp_Frond->ePrimType = PRIM_TRIANGLESTRIP;
	}

	void MTree::_setupLeafGeometry()
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
					memcpy(&vert[v].TexCoords, &leaf->m_pLeafMapTexCoords[i][vIndices[v] * 2], 2 * sizeof(float));
					/*vert[v].TexCoords[0] = vTexcoord[vIndices[v]].x;
					vert[v].TexCoords[1] = vTexcoord[vIndices[v]].y;*/

					vert[v].WindIndex = 4.0f * leaf->m_pWindMatrixIndices[i];
					vert[v].WindWeight = leaf->m_pWindWeights[i];

					vert[v].PlacementIndex = float(c_nVertexShader_LeafTables + leaf->m_pLeafClusterIndices[i] * 4 + vIndices[v]);
					vert[v].ScalarValue = mSpeedTree->GetLeafLodSizeAdjustments()[lod];
				}

				vert += 6;
			}
			vb->Unlock();
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
		vxStream.Bind(0, vb, sizeof(LeafVertex));
		vxStream.SetCount(vb->GetSize() / sizeof(LeafVertex));

		mRenderOp_Leaf->rState.cullMode = CULL_NONE;
		mRenderOp_Leaf->rState.alphaTestRef = mGeometryCache->m_sLeaves0.m_fAlphaTestValue / 255.0f;
		mRenderOp_Leaf->rState.blendMode = BM_ALPHA_TEST;
		//mRenderOp_Leaf->rState.fillMode = FILL_FRAME;
		mRenderOp_Leaf->iPrimCount = vxStream.GetCount() / 3;
		mRenderOp_Leaf->ePrimType = PRIM_TRIANGLELIST;
	}

	void MTree::_setupColMesh()
	{
		CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(mGeometryCache->m_sBranches);

		unsigned short iVertexCount = pBranches->m_usVertexCount;
		int iTriCount = mBranchIndexCounts[0] - 2;

		if (iVertexCount == 0)
			return ;

		mColMesh.Alloc(iVertexCount, iTriCount);

		int indexV = 0, indexI = 0;

		Array<Vec3> & colVert = mColMesh.GetPositions();
		Array<int> & colIndex = mColMesh.GetIndices();

		for (int i = 0; i < iVertexCount; ++i)
		{
			Vec3 v = *(Vec3 *)&pBranches->m_pCoords[i * 3];

			colVert[indexV++] = v;
		}

		for (int i = 0; i < (mBranchIndexCounts[0] - 2) / 2; ++i)
		{
			int i0 = pBranches->m_pStrips[0][i * 2 + 0];
			int i1 = pBranches->m_pStrips[0][i * 2 + 1];
			int i2 = pBranches->m_pStrips[0][i * 2 + 2];
			int i3 = pBranches->m_pStrips[0][i * 2 + 3];

			d_assert (i0 < indexV && i1 < indexV && i2 < indexV && i3 < indexV);

			colIndex[indexI++] = i2;
			colIndex[indexI++] = i1;
			colIndex[indexI++] = i0;

			colIndex[indexI++] = i3;
			colIndex[indexI++] = i1;
			colIndex[indexI++] = i2;
		}
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

	RenderOp *  MTree::_getFrondRenderOp(int lod)
	{
		if (lod == 0)
			return mRenderOp_Frond;

		return NULL;
	}

	RenderOp * MTree::_getLeafRenderOp(int lod)
	{
		if (!mRenderOp_Leaf)
			return NULL;

		return mRenderOp_Leaf;
	}

	void MTree::SetupLeafBillboard(ShaderParam * param)
	{
		mSpeedTree->GetGeometry(*mGeometryCache, SpeedTree_LeafGeometry, -1, -1, 0);

		unsigned int uiEntryCount = 0;
		const float * pTable = mSpeedTree->GetLeafBillboardTable(uiEntryCount);

		param->SetVector((const Vec4 *)pTable, uiEntryCount / 4);
	}
}