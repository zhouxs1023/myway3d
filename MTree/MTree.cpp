#include "MTree.h"
#include "MWResourceManager.h"
#include "MWVideoBufferManager.h"
#include "MForest.h"
#include "SpeedTreeRT.h"
#include "SpeedTreeExp.h"


namespace Myway {

	MTree::MTree(const TString128 & source)
		: mSpeedTree(NULL)
	{
		mRenderOp_Branch = NULL;
		mRenderOp_Frond = NULL;
		mRenderOp_Leaf = NULL;
		mRenderOp_Billboard = NULL;

		mSourceName = source;
	}

	MTree::~MTree()
	{
		if (mSpeedTree)
			SpeedTreeExp::DelTree(mSpeedTree);

		mSpeedTree = NULL;

		safe_delete(mRenderOp_Branch);
		safe_delete(mRenderOp_Billboard);
		safe_delete(mRenderOp_Frond);
		safe_delete(mRenderOp_Leaf);
	}

	void MTree::DeleteSelf()
	{
		MForest::Instance()->DestroyTree(this);
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

		if (mSpeedTree)
			SpeedTreeExp::DelTree(mSpeedTree);

		mSpeedTree = NULL;
	}

	void MTree::LoadImp(DataStreamPtr stream)
	{
		d_assert (mSpeedTree == NULL);

		const void * data = stream->GetData();
		int size = stream->Size();

		mSpeedTree = SpeedTreeExp::LoadTree((const char *)data, size);
		d_assert (mSpeedTree);

		_calcuBound();

		SpeedTreeExp::Material mBranchMaterial;
		SpeedTreeExp::Material mFrondMaterial;
		SpeedTreeExp::Material mLeafMaterial;

		SpeedTreeExp::GetMaterial(&mBranchMaterial, &mFrondMaterial, &mLeafMaterial, mSpeedTree);

		_setupBranchGeometry();
	}

	void MTree::_calcuBound()
	{
		float bound[6];
		SpeedTreeExp::GetBound(bound, mSpeedTree);

		mBound.minimum.x = bound[0];
		mBound.minimum.y = bound[1];
		mBound.minimum.z = bound[2];

		mBound.maximum.x = bound[3];
		mBound.maximum.y = bound[4];
		mBound.maximum.z = bound[5];

		mSphere.center = mBound.GetCenter();
		mSphere.radius = mSphere.center.Distance(mBound.GetLeftBottomBackPoint());
	}

	void MTree::_loadTexture()
	{
		TString128 path = File::GetFileDir(GetSourceName());

		if (path != "")
			path += "\\";

		// query the textures from RT
		CSpeedTreeRT::SMapBank * sMapBank = SpeedTreeExp::GetMapBank(mSpeedTree);

		for (int nLayer = CSpeedTreeRT::TL_DIFFUSE; nLayer < CSpeedTreeRT::TL_SHADOW; ++nLayer)
		{
			if (nLayer != CSpeedTreeRT::TL_DIFFUSE || nLayer != CSpeedTreeRT::TL_NORMAL)
				continue;

			TString128 branchTex = path + File::GetBaseName(sMapBank->m_pBranchMaps[nLayer]);

			mBranchTexture[nLayer] = VideoBufferManager::Instance()->Load2DTexture(branchTex, branchTex);
		}

		SpeedTreeExp::DelMapBank();
	}

	void MTree::_setupBranchGeometry()
	{
		d_assert (mRenderOp_Branch == NULL);

		// query vertex attribute data
		CSpeedTreeRT::SGeometry * sGeometry = SpeedTreeExp::GetGeometry(mSpeedTree, SpeedTree_BranchGeometry);
		const CSpeedTreeRT::SGeometry::SIndexed & sBranches = sGeometry->m_sBranches;

		int vertexCount = sBranches.m_nNumVertices;

		if (vertexCount == 0)
			return ;

		mRenderOp_Branch = new RenderOp();

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();

		int offset = 0, tc = 0;
		vdecl->AddElement(0, offset, DT_FLOAT4, DU_POSITION, 0); offset += 16;
		vdecl->AddElement(0, offset, DT_FLOAT4, DU_TEXCOORD, tc++); offset +=16;
		vdecl->AddElement(0, offset, DT_FLOAT4, DU_TEXCOORD, tc++); offset +=16;
		vdecl->AddElement(0, offset, DT_FLOAT4, DU_TEXCOORD, tc++); offset +=16;
		vdecl->AddElement(0, offset, DT_FLOAT4, DU_TEXCOORD, tc++); offset +=16;
		vdecl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(offset * vertexCount);

		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);

		for (int i = 0; i < vertexCount; ++i)
		{
			// 1. position
			float fSelfShadowCoord_S = 0 ? sBranches.m_pTexCoords[CSpeedTreeRT::TL_SHADOW][i * 2 + 0] : 0;

			*verts++ = sBranches.m_pCoords[i * 3 + 0];
			*verts++ = sBranches.m_pCoords[i * 3 + 1];
			*verts++ = sBranches.m_pCoords[i * 3 + 2];
			*verts++ = fSelfShadowCoord_S;


			// 2. normal
			float fSelfShadowCoord_T = 0 ? sBranches.m_pTexCoords[CSpeedTreeRT::TL_SHADOW][i * 2 + 1] : 0;

			*verts++ = sBranches.m_pNormals[i * 3 + 0];
			*verts++ = sBranches.m_pNormals[i * 3 + 1];
			*verts++ = sBranches.m_pNormals[i * 3 + 2];
			*verts++ = fSelfShadowCoord_T;


			// 3. diffuse u v, wind params
			const float c_fWindWeightCompressionRange = 0.98f;
			const float c_fWeightRange = 0.98f;

			const float* pDiffuseCoords = sBranches.m_pTexCoords[CSpeedTreeRT::TL_DIFFUSE] + i * 2;
			float fWindParam1 = sBranches.m_pWindMatrixIndices[0][i] + sBranches.m_pWindWeights[0][i] * c_fWeightRange;
			float fWindParam2 = sBranches.m_pWindMatrixIndices[1][i] + sBranches.m_pWindWeights[1][i] * c_fWeightRange;
			*verts++ = pDiffuseCoords[0], *verts++ = pDiffuseCoords[1];
			*verts++ = fWindParam1, *verts++ = fWindParam2;


			// 4. detail u v, normal map u v 
			const float* pDetailCoords = sBranches.m_pTexCoords[CSpeedTreeRT::TL_DETAIL] + i * 2;
			const float* pNormalMapCoords = sBranches.m_pTexCoords[CSpeedTreeRT::TL_NORMAL] + i * 2;
			*verts++ = pDetailCoords[0], *verts++ = pDetailCoords[1];
			*verts++ = pNormalMapCoords[0], *verts++ = pNormalMapCoords[1];


			// 5. tangent
			*verts++ = sBranches.m_pTangents[i * 3 + 0];
			*verts++ = sBranches.m_pTangents[i * 3 + 1];
			*verts++ = sBranches.m_pTangents[i * 3 + 2];
			*verts++ = sBranches.m_pLodFadeHints[i];
		}


		// index data (only lod 0)
		int nNumLods = SpeedTreeExp::GetNumBranchLod(mSpeedTree);
		int nLod = 0;

		d_assert (nNumLods > 0 && sBranches.m_pNumStrips);

		d_assert (sBranches.m_pNumStrips[nLod]);
		d_assert (sBranches.m_pStripLengths[nLod]);

		int indexCount = sBranches.m_pStripLengths[nLod][0];

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(indexCount * sizeof(short));

		short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);

		for (int i = 0; i < indexCount; ++i)
		{
			d_assert(sBranches.m_pStrips[nLod][0][i] > -1 && sBranches.m_pStrips[nLod][0][i] < vertexCount);

			*idx++ = sBranches.m_pStrips[nLod][0][i];
		}

		ib->Unlock();


		// init render op
		mRenderOp_Branch->vxStream.SetDeclaration(vdecl);
		mRenderOp_Branch->vxStream.Bind(0, vb, offset);
		mRenderOp_Branch->vxStream.SetCount(vertexCount);

		mRenderOp_Branch->ixStream.Bind(ib, 0);
		mRenderOp_Branch->ixStream.SetCount(indexCount);

		mRenderOp_Branch->iPrimCount = indexCount / 3;
		mRenderOp_Branch->ePrimType = PRIM_TRIANGLELIST;

		SpeedTreeExp::DelGeometry();
	}

	void MTree::_setupFrondGeometry()
	{
	}

	void MTree::_setupLeafGeometry()
	{
	}

	void MTree::_setupBillboardGeometry()
	{
	}

}