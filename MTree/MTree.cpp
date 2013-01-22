#include "MTree.h"
#include "MWResourceManager.h"
#include "MWVideoBufferManager.h"
#include "MForest.h"

namespace Myway {

	MTree::MTree(const TString128 & source)
		: mBound(Aabb::Zero)
		, mSphere(Sphere::Zero)
	{
		mSourceName = source;

		mRenderOp_Branch = NULL;
	}

	MTree::~MTree()
	{
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
	}

	void MTree::DoGenerate()
	{
		safe_delete (mRenderOp_Branch);

		MTreeGeometry geo;

		mResource.GetGeomtry(geo);

		_setupBranchGeometry(geo);
		_calcuBound(geo);
	}

	MTreeRes * MTree::GetTreeResource()
	{
		return &mResource;
	}

	void MTree::_setupBranchGeometry(MTreeGeometry & geo)
	{
		mRenderOp_Branch = new RenderOp();

		VertexStream & vxStream = mRenderOp_Branch->vxStream;
		IndexStream & ixStream = mRenderOp_Branch->ixStream;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
		vdecl->AddElement(0, 24, DT_FLOAT3, DU_TANGENT, 0);
		vdecl->AddElement(0, 36, DT_FLOAT2, DU_TEXCOORD, 0);
		vdecl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(sizeof(MTreeVertex) * geo.BranchVerts.Size());

		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);
		{
			memcpy(verts, &geo.BranchVerts[0], sizeof(MTreeVertex) * geo.BranchVerts.Size());
		}
		vb->Unlock();

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof(short) * geo.BranchFaces.Size() * 3);

		short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);
		for (int i = 0; i < geo.BranchFaces.Size(); ++i)
		{
			*idx++ = geo.BranchFaces[i].p0;
			*idx++ = geo.BranchFaces[i].p1;
			*idx++ = geo.BranchFaces[i].p2;
		}
		ib->Unlock();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vb, sizeof(MTreeVertex));
		vxStream.SetCount(geo.BranchVerts.Size());

		ixStream.Bind(ib, 0);
		ixStream.SetCount(geo.BranchFaces.Size() * 3);

		mRenderOp_Branch->iPrimCount = geo.BranchFaces.Size();
		mRenderOp_Branch->ePrimType = PRIM_TRIANGLELIST;
	}

	void MTree::_calcuBound(MTreeGeometry & geo)
	{
		mBound = Aabb::Invalid;

		for (int i = 0; i < geo.BranchVerts.Size(); ++i)
		{
			mBound.minimum = mBound.minimum.Minimum(geo.BranchVerts[i].Position);
			mBound.maximum = mBound.maximum.Maximum(geo.BranchVerts[i].Position);
		}

		mSphere.center = mBound.GetCenter();
		mSphere.radius = mSphere.center.Distance(mBound.minimum);
	}
}