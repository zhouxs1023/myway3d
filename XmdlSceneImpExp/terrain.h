#pragma once

#define TERRAIN_BLOCK		*(DWORD*)"TBLK"

#define TERRAIN_BLOCKINFO	*(DWORD*)"TBKI"
#define TERRAIN_WATERINFO	*(DWORD*)"TWAT"
#define TERRAIN_LAYER		*(DWORD*)"TLYR"
#define TERRAIN_SHADOW		*(DWORD*)"TSHW"
#define TERRAIN_SHADOWT		*(DWORD*)"TSWT"
#define TERRAIN_HEIGHT		*(DWORD*)"THIT"
#define TERRAIN_NORMAL		*(DWORD*)"TNRM"
#define TERRAIN_ECOTYPE		*(DWORD*)"TECO"

#define TERRAIN_GRID_SIZE 25.6f
#define TERRAIN_BLOCK_SIZE 16

namespace xmdl {

	struct t_blockInfo
	{
		BYTE bZoneX;
		BYTE bZoneZ;
		BYTE bBlockX;
		BYTE bBlockZ;
		BYTE bLayers;		// ÎÆÀí²ãÊý
	};

	struct t_block
	{
		t_blockInfo info;

		WORD* pHeights;
		UINT nHeights;

		int xIndex;
		int zIndex;

		t_block()
		{
			pHeights = NULL;
			nHeights = 0;

			xIndex = 0;
			zIndex = 0;
		}

		~t_block()
		{
			safe_delete_array (pHeights);
		}
	};

	class t_scene;

	class t_terrain
	{
	public:
		t_terrain(t_scene * scene);
		~t_terrain();

		void loadTile(File & file, int size);

		void import();

	protected:
		void loadBlock(DataStreamPtr stream, int size);

		float getHeight(float x, float z);

	protected:
		Array<t_block *> mBlocks;
		t_scene * mScene;
		float * mHeights;
		int mXVertSize;
		int mZVertSize;
	};
}