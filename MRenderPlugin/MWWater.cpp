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

		d_assert (mTech);
	}

	void Water::Shutdown()
	{
		safe_delete_array (mData);
		safe_delete_array (mBlocks);
		mVertexDecl = NULL;
	}

	void Water::Load(const char * source)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

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

			d_assert (sizeX != mSizeX && sizeZ != mSizeZ);

			stream->Read(&mHeight, sizeof(float));

			stream->Read(mData, sizeX * sizeZ);
		}
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

	void Water::Render()
	{
		RS_RenderEvent(RenderWater);

		RenderSystem * render = RenderSystem::Instance();
		Camera * cam = World::Instance()->MainCamera();

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
					render->Render(mTech, block->mRenderOp);
				}
			}
		}
	}

}