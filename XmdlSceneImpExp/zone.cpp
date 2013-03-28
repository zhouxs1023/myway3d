#include "stdafx.h"

#include "zone.h"
#include "xmdl.h"
#include "Shape.h"
#include "xMesh.h"

using namespace Infinite;

namespace xmdl {
	
	t_zone::t_zone(t_scene * scene)
	{
		mScene = scene;
		mModelNameSize = NULL;
		mModelNames = NULL;

		mObjectCount = NULL;
		mObjectsF = NULL;
	}

	t_zone::~t_zone()
	{
		safe_delete_array(mModelNames);
		safe_delete_array(mObjectsF);
	}

	void t_zone::load(const char * filename, t_terrain * terrain)
	{
		File file;

		file.Open(filename, OM_READ_BINARY);

		t_head head;
		file.Read(&head, sizeof(t_head));

		d_assert (head.dwFlag == ZONE_FLAG && head.dwVersion == ZONE_VER);

		t_chunk ck;

		while ( file.Read(&ck, sizeof(ck)) == 1 )
		{
			if (ck.dwFlag == ZONE_SCENE)
			{
				loadScene(file, ck.dwChunkSize);
			}
			else if (ck.dwFlag == ZONE_TERRAIN_TILE)
			{
				terrain->loadTile(file, ck.dwChunkSize);
			}
			else if (ck.dwFlag == ZONE_TERRAIN_BIG)
			{
				file.Skip(ck.dwChunkSize);
			}
			else if (ck.dwFlag == ZONE_INDOOR)
			{
				file.Skip(ck.dwChunkSize);
			}
			else if (ck.dwFlag == ZONE_COLLOSION)
			{
				file.Skip(ck.dwChunkSize);
			}
			else
			{
				d_assert (0);
				file.Skip(ck.dwChunkSize);
			}
		}
	}

	void t_zone::loadScene(File & file, int size)
	{
		t_chunk ck;

		while ( size > 0 && file.Read(&ck, sizeof(ck)) == 1 )
		{
			if (ck.dwFlag == SCENE_STRING)
			{
				mModelNameSize = ck.dwChunkSize;
				mModelNames = new char[ck.dwChunkSize];

				file.Read(mModelNames, ck.dwChunkSize);
			}

			else if (ck.dwFlag == SCENE_MDL_INSTANCE)
			{
				mObjectCount = ck.dwChunkSize / sizeof (ScnObj_F);

				d_assert (mObjectCount * sizeof(ScnObj_F) == ck.dwChunkSize);

				mObjectsF = new ScnObj_F[mObjectCount];

				file.Read(mObjectsF, ck.dwChunkSize);
			}

			else
			{
				file.Skip(ck.dwChunkSize);
			}

			size -= sizeof(ck) + ck.dwChunkSize;
		}

		d_assert (size == 0);
	}

	Aabb t_zone::import()
	{
		static int s_index = 0;

		Aabb bound = Aabb::Zero;

		for (int i = 0; i < mObjectCount; ++i)
		{
			const ScnObj_F * obj = &mObjectsF[i];

			if (obj->m_eType != ScnObj_Model)
				continue ;

			const char * filename = &mModelNames[obj->m_nTemplate];
			TString128 uID = TString128("_mesh_") + s_index++;

			TString128 meshFile = TString128(filename) + ".mesh";

			xMesh * mesh = (xMesh *)ShapeManager::Instance()->_Create(uID.c_str(), "Mesh");

			d_assert (mesh != NULL);

			Vec3 pos, scale;
			Quat orient;

			obj->m_tmWorld.Decompose(pos, orient, scale);

			mesh->SetPosition(pos);
			mesh->SetOrientation(orient);
			mesh->SetScale(scale.x);
			mesh->SetMeshFile("mesh\\" + meshFile);
			
			bound.Merge(mesh->GetBound());
		}

		return bound;
	}









	t_scene::t_scene()
	{
		mTerrain = NULL;
	}

	t_scene::~t_scene()
	{
		safe_delete (mTerrain);
	}

	void t_scene::load(const char * filename)
	{
		mTerrain = new t_terrain(this);

		// load scene
		File file;

		file.Open(filename, OM_READ_BINARY);

		d_assert (file.IsOpen());

		t_head head;

		file.Read(&head, sizeof(t_head));

		d_assert (head.dwFlag == STAGE_FLAG && (head.dwVersion == STAGE_VER || head.dwVersion == STAGE_VER4));

		t_chunk ck;

		while (file.Read(&ck, sizeof(t_chunk)) == 1)
		{
			if (ck.dwFlag == STAGE_INFO)
			{
				d_assert (ck.dwChunkSize == sizeof(t_scene_info));

				file.Read(&mInfo, sizeof(t_scene_info));
			}
			else
			{
				file.Skip(ck.dwChunkSize);
			}
		}

		file.Close();

		// load all zone
		{
			TString128 zonePath = File::RemoveExternName(filename);

			FileSystem fs(zonePath);

			fs.Load();

			Archive::FileInfoVisitor v = fs.GetFileInfos();

			Aabb bound = Aabb(0, 0, 0, 1000, 1000, 1000);

			while (!v.Endof())
			{
				TString128 zoneFile = zonePath + "\\" + v.Cursor()->second.name;

				if (File::GetExternName(zoneFile) == "zone")
				{
					t_zone zone(this);

					zone.load(zoneFile.c_str(), mTerrain);

					Aabb abb = zone.import();

					bound.Merge(abb);
				}

				++v;
			}

			World::Instance()->Resize((int)bound.GetWidth(), (int)bound.GetHeight(), (int)bound.GetDepth());
		}

		mTerrain->import();
	}





	//MyPlugin::MyPlugin()
	//{
	//}

	//MyPlugin::~MyPlugin()
	//{
	//}

	//void MyPlugin::Import(const char * filename)
	//{
	//	// load zone.
	//	TString128 zonePath = File::RemoveExternName(filename);

	//	FileSystem fs(zonePath);

	//	fs.Load();

	//	Archive::FileInfoVisitor v = fs.GetFileInfos();

	//	while (!v.Endof())
	//	{
	//		TString128 zoneFile = zonePath + "\\" + v.Cursor()->second.name;

	//		if (File::GetExternName(zoneFile) == ".zone")
	//		{
	//			t_zone zone;

	//			zone.Load(zoneFile.c_str());
	//		}
	//	}

	//	// load terrain.
	//}

	//void MyPlugin::Export(const char * filename)
	//{
	//}

	//int MyPlugin::GetUsages()
	//{
	//	int usages = iPlugin::eImport;
	//	usages |= iPlugin::eExport;

	//	return usages;
	//}
}