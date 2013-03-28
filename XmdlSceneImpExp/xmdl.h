#pragma once

namespace xmdl {

#define XMDL_FLAG		*(DWORD*)"XMDL"
#define XMDL_VER		3

#define MODEL_FLAG		*(DWORD*)"MODL"
#define TEXS_FLAG		*(DWORD*)"TEXS"
#define FXS_FLAG		*(DWORD*)"FXS "
#define STR_FLAG		*(DWORD*)"STRS"
#define NODE_FLAG		*(DWORD*)"NODE"
#define HOOK_FLAG		*(DWORD*)"HOOK"
#define GEOS_FLAG		*(DWORD*)"GEOS"
#define EMIT_FLAG		*(DWORD*)"EMIT"
#define BOARD_FLAG		*(DWORD*)"BORD"
#define CAMERA_FLAG		*(DWORD*)"CAMR"
#define LIGHT_FLAG		*(DWORD*)"LIGT"
#define NTRACK_FLAG		*(DWORD*)"NTRK"
#define FIGURE_FLAG		*(DWORD*)"FIGR"
#define RIBBON_FLAG		*(DWORD*)"RIBN"
#define RIBBONCTL_FLAG	*(DWORD*)"RIBC"
#define SHACK_FLAG		*(DWORD*)"SHAK"
#define FLYOBJ_FLAG		*(DWORD*)"FLYO"
#define DECAL_FLAG		*(DWORD*)"DECL"



#define GEOSET_PARAM	*(DWORD*)"PARM"
#define GEOSET_VERTEX	*(DWORD*)"VPOS"
#define GEOSET_NORMAL	*(DWORD*)"VNRM"
#define GEOSET_BINORMAL	*(DWORD*)"BNRM"
#define GEOSET_TANGENT	*(DWORD*)"TANG"
#define GEOSET_TEXCOORD	*(DWORD*)"VUV0"
#define GEOSET_VCOLOR	*(DWORD*)"VCLR"
#define GEOSET_INDEX	*(DWORD*)"INDX"
#define GEOSET_SKINS	*(DWORD*)"SKIN"
#define GEOSET_BWEIGHT	*(DWORD*)"BWGT"
#define GEOSET_EXTENT	*(DWORD*)"EXTN"
#define GEOSET_UNWRAP	*(DWORD*)"WRAP"
#define GEOSET_UVWMAP	*(DWORD*)"MAPA"
#define GEOSET_MPOINT	*(DWORD*)"MPNT"



	struct t_chunk
	{
		DWORD dwFlag;
		UINT nVersion;
		DWORD dwChunkSize;
	};

	struct t_head
	{
		DWORD dwFlag;
		DWORD dwVersion;
	};

	struct t_bound
	{
		FLOAT fBoundsRadius;
		FLOAT fMinimumExtent[3];
		FLOAT fMaximumExtent[3];
	};

	struct t_model
	{
		WORD wFlags;
		USHORT nFrames;
		t_bound bound;
	};

	struct t_hook
	{
		USHORT nHookName;
		USHORT nBoneIndex;
	};

	struct t_geoset
	{
		DWORD dwFlags;				// 属性
		BYTE byFxId;				// 材质
		BYTE byFaceType;			// D3DPRIMITIVETYPE
		USHORT nFrames;				// 基于网格的动画桢数(顶点变形,UV变形)
		t_bound b;					// 网格边界
		DWORD dwNameStr;			// 名字字符串
	};

	struct t_texture
	{
		DWORD dwFileNameStr;
	};


	struct t_mesh
	{
		t_geoset mGeoset;

		int mTextureId;
		int mVertexCount;
		int mIndexCount;

		Vec3 * mVertex;
		Vec3 * mNormal;
		Vec2 * mUV;
		WORD * mIndex;

		t_mesh()
		{
			mVertexCount = 0;
			mIndexCount = 0;

			mTextureId = -1;

			mVertex = NULL;
			mNormal = NULL;
			mUV = NULL;
			mIndex = NULL;
		}

		~t_mesh()
		{
			safe_delete_array (mVertex);
			safe_delete_array (mNormal);
			safe_delete_array (mUV);
			safe_delete_array (mIndex);

			mVertexCount = 0;
			mIndexCount = 0;
		}
	};


	struct t_xmdl
	{

	public:
		t_xmdl();
		virtual ~t_xmdl();

		void load(const char * filename);
		void save(const char * filename);

		MeshPtr build();

	protected:
		void _loadHead(DataStreamPtr & file);
		void _loadGeoset(DataStreamPtr & file, int size);
		int _loadTexture(DataStreamPtr & file, int size);

	protected:
		TString128 mFilename;

		int mCount;
		t_model * mModel;

		int mStrTableSize;
		char * mStrTable;

		int mTextureCount;
		t_texture * mTextures;

		Array<t_mesh *> mMeshes;
	};

}