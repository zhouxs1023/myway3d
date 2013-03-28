#include "stdafx.h"

#include "terrain.h"
#include "xmdl.h"
#include "zone.h"

#include "xTerrain.h"

namespace xmdl {

	t_terrain::t_terrain(t_scene * scene)
		: mScene(scene)
		, mHeights(NULL)
	{
	}

	t_terrain::~t_terrain()
	{
		safe_delete_array (mHeights);
	}

	void t_terrain::loadTile(File & file, int size)
	{
		DataStreamPtr stream = new MemoryStream(size);

		int count = file.Read(stream->GetData(), size);

		d_assert (count == 1);

		t_chunk ck;

		while (size > 0 && stream->Read(&ck, sizeof(ck)) == 1)
		{
			if (ck.dwFlag == TERRAIN_BLOCK)
			{
				loadBlock(stream, ck.dwChunkSize);
			}
			else
			{
				stream->Skip(ck.dwChunkSize);
			}

			size -= sizeof(ck) + ck.dwChunkSize;
		}

		d_assert (size == 0);
	}

	void t_terrain::loadBlock(DataStreamPtr stream, int size)
	{
		t_block * block = new t_block;

		t_chunk ck;

		while (size > 0 && stream->Read(&ck, sizeof(ck)) == 1)
		{
			if (ck.dwFlag == TERRAIN_BLOCKINFO)
			{
				d_assert (ck.dwChunkSize == sizeof(t_blockInfo));

				stream->Read(&block->info, ck.dwChunkSize);
			}
			else if (ck.dwFlag == TERRAIN_HEIGHT)
			{
				block->nHeights = ck.dwChunkSize / sizeof(WORD);
				block->pHeights = new WORD[block->nHeights];

				d_assert (block->nHeights * sizeof(WORD) == ck.dwChunkSize);

				stream->Read(block->pHeights, ck.dwChunkSize);
			}
			else
			{
				stream->Skip(ck.dwChunkSize);
			}

			size -= sizeof(ck) + ck.dwChunkSize;
		}

		block->xIndex = (block->info.bZoneX * mScene->getInfo()->nUnitsPreZone + block->info.bBlockX * TERRAIN_BLOCK_SIZE);
		block->zIndex = (block->info.bZoneZ * mScene->getInfo()->nUnitsPreZone + block->info.bBlockZ * TERRAIN_BLOCK_SIZE);

		mBlocks.PushBack(block);

		d_assert (size == 0);
	}

	void t_terrain::import()
	{
		if (mBlocks.Size() == 0)
			return ;

		int xCount = 0, zCount = 0;

		for (int i = 0; i < mBlocks.Size(); ++i)
		{
			xCount = Math::Maximum(xCount, mBlocks[i]->xIndex);
			zCount = Math::Maximum(zCount, mBlocks[i]->zIndex);
		}

		mXVertSize = xCount + TERRAIN_BLOCK_SIZE + 1;
		mZVertSize = zCount + TERRAIN_BLOCK_SIZE + 1;

		mHeights = new float[mXVertSize * mZVertSize];

		for (int i = 0; i < mBlocks.Size(); ++i)
		{
			t_block * block = mBlocks[i];

			for (int z = 0; z <= TERRAIN_BLOCK_SIZE; ++z)
			{
				for (int x = 0; x <= TERRAIN_BLOCK_SIZE; ++x)
				{
					int nOffset = x + z * (TERRAIN_BLOCK_SIZE + 1);

					int r = block->zIndex + z;
					int c = block->xIndex + x;

					int index = r * mXVertSize + c;

					float fy = block->pHeights[nOffset] * mScene->getInfo()->fLevelSize;

					mHeights[index] = fy;
				}
			}
		}

		d_assert (mXVertSize == mZVertSize && mXVertSize <= 1024 && mZVertSize <= 1024);

		Terrain::Config config;

		config.xVertexCount = 1024;
		config.zVertexCount = 1024;
		config.xSize = (mXVertSize - 1) * TERRAIN_GRID_SIZE;
		config.zSize = (mZVertSize - 1) * TERRAIN_GRID_SIZE;

		Infinite::xTerrain * terrain = (Infinite::xTerrain *)Infinite::ShapeManager::Instance()->_Create("_UTerrain", "Terrain");

		terrain->_create(config);

		Rect rc = { 0, 0, 1024, 1024 };

		float * height = terrain->_getTerrain()->LockHeight(rc);
		int index = 0;
		for (int z = 0; z <= 1024; ++z)
		{
			for (int x = 0; x <= 1024; ++x)
			{
				height[index++] = getHeight(x / 1024.0f, 1 - z / 1024.0f);
			}
		}
		terrain->_getTerrain()->UnlockHeight();
	}

	float t_terrain::getHeight(float x, float z)
	{
		int xCell = mXVertSize - 1;
		int zCell = mZVertSize - 1;

		int ix0 = int(x * xCell);
		int iz0 = int(z * zCell);

		int ix1 = ix0 + 1;
		int iz1 = iz0 + 1;

		if (ix1 > xCell)
			ix1 = xCell;

		if (iz1 > zCell)
			iz1 = zCell;

		float kx = x * xCell - ix0;
		float kz = z * zCell - iz0;

		float hx00 = mHeights[iz0 * mXVertSize + ix0];
		float hx01 = mHeights[iz0 * mXVertSize + ix1];
		float hx10 = mHeights[iz1 * mXVertSize + ix0];
		float hx11 = mHeights[iz1 * mXVertSize + ix1];

		float h0 = hx00 + (hx01 - hx00) * kx;
		float h1 = hx10 + (hx11 - hx10) * kx;

		float h = h0 + (h1 - h0) * kz;

		return h;
	}

}