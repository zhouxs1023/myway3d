#pragma once

#include "terrain.h"

namespace xmdl {

#define ZONE_FLAG			*(DWORD*)"ZONE"
#define ZONE_VER			1
#define ZONE_TERRAIN_TILE	*(DWORD*)"TERT"
#define ZONE_TERRAIN_BIG	*(DWORD*)"TERB"
#define ZONE_SCENE			*(DWORD*)"SCNE"
#define ZONE_INDOOR			*(DWORD*)"INDR"
#define ZONE_COLLOSION		*(DWORD*)"CLSN"
#define ZONE_FLORA			*(DWORD*)"FLRA"

#define SCENE_STRING		*(DWORD*)"TSTR"
#define SCENE_MDL_TEMPLATE	*(DWORD*)"MDLT"
#define SCENE_SND_TEMPLATE	*(DWORD*)"SNDT"
#define SCENE_MDL_INSTANCE	*(DWORD*)"INST"
#define SCENE_LIT_INSTANCE	*(DWORD*)"LINS"
#define SCENE_SND_INSTANCE	*(DWORD*)"SINS"
#define SCENE_MDL_VCOLOR	*(DWORD*)"VCLR"
#define SCENE_DECALS		*(DWORD*)"DCLS"

	enum ScnObjType
	{
		ScnObj_Unknown= -1,				// 未知类型
		ScnObj_Model,					// 场景模型
		ScnObj_Light,					// 场景灯光
		ScnObj_Sound,					// 场景音效
		ScnObj_Decal,					// 场景贴花
		ScnObj_Flora,					// 场景植被
	};

	struct ScnObj_F
	{
		ScnObjType m_eType;				// 场景物体的类型
		UINT	m_nTemplate;			// 模板ID
		DWORD	m_dwGameID;				// 逻辑控制ID
		DWORD	m_dwUID;				// 实例ID
		Mat4	m_tmWorld;				// 空间矩阵
		UINT	m_nVCSize;				// 顶点色顶点数
		UINT	m_nVCOffset;			// 在顶点色数组中的偏移量
		Vec3	m_vBoxMin;				// 边界盒
		Vec3	m_vBoxMax;				// 
	};


	class t_zone
	{
	public:
		t_zone(t_scene * scene);
		~t_zone();

		void load(const char * filename, t_terrain * terrain);

		Aabb import();

	protected:
		void loadScene(File & file, int size);

	protected:
		t_scene * mScene;

		int mModelNameSize;
		char * mModelNames;

		int mObjectCount;
		ScnObj_F* mObjectsF;
	};


#define STAGE_FLAG		*(DWORD*)"STAG"
#define STAGE_VER		3
#define STAGE_VER4      4
#define STAGE_INFO		*(DWORD*)"INFO"	
#define STAGE_ENV		*(DWORD*)"ENVM"
#define STAGE_GLOW		*(DWORD*)"GLOW"
#define STAGE_SKYINFO	*(DWORD*)"SKY "
#define STAGE_TEXTURE	*(DWORD*)"STEX"
#define STAGE_TEX_REP	*(DWORD*)"STRP"
#define STAGE_TEX_PROJ	*(DWORD*)"STPJ"
#define STAGE_LIGHT		*(DWORD*)"SLIT"
#define STAGE_WATER		*(DWORD*)"SWAT"
#define STAGE_WATER_EX  *(DWORD*)"WAEX"
#define STAGE_VOL_FOG   *(DWORD*)"VFOG"

	struct t_scene_info
	{
		RECT rtZones;			// 以Zone为单位的全局地图大小.
		float fTileSize;		// 单元格子长度	
		float fLevelSize;		// 高度级别
		float fHeight;			// 高度
		USHORT nUnitsPreZone;	// 每个Zone的格子维度
		USHORT nLogicsPreUnit;	// 每个格子的Logic维度
		BOOL bVCFullTime;		// 是否包含完整四个顶点色时段
	};


	class t_scene
	{
	public:
		t_scene();
		~t_scene();

		void load(const char * filename);

		const t_scene_info * getInfo() { return &mInfo; }

	protected:
		t_scene_info mInfo;
		t_terrain * mTerrain;

	};

	//class MyPlugin : public Infinite::iPlugin
	//{
	//public:
	//	MyPlugin();
	//	virtual ~MyPlugin();

	//	const char * GetImpExtern() { return  ".stg"; }

	//	void Import(const char * filename);
	//	void Export(const char * filename);

	//	int GetUsages();
	//};
}