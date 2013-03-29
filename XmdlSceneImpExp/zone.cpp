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




	void CNameTable::Clear()
	{
		m_vecBuffer.clear();
		m_vecStart.clear();
	}

	void CNameTable::Create(UINT nLen,TCHAR * lpszBuffer)
	{
		if(nLen>0)
		{
			m_vecBuffer.resize(nLen);
			CopyMemory(&m_vecBuffer.front(),lpszBuffer,nLen * sizeof(TCHAR));

			m_vecStart.push_back(0);
			for(UINT n=0;n<nLen-1;n++)
			{
				if(m_vecBuffer[n] == _TCHAR('\0'))
					m_vecStart.push_back(n+1);
			}
		}
	}

	UINT CNameTable::AddName(const TCHAR * lpszName)
	{
		UINT n = FindName(lpszName);
		if(n != (UINT)-1)
			return GetNameOffset(n);

		UINT nStart = m_vecBuffer.size();
		//if(nStart) //不需要这一句吧
		{
			m_vecStart.push_back(nStart);
			m_vecBuffer.resize(m_vecBuffer.size() + lstrlen(lpszName) + 1);
			CopyMemory(&m_vecBuffer.front()+nStart,lpszName,lstrlen(lpszName)+1);
		}
		return nStart;
	}

	UINT CNameTable::DelName(const TCHAR * lpszName)
	{
		UINT n = FindName(lpszName);
		if(n != (UINT)-1)
		{
			UINT nStart = GetNameOffset(n);
			UINT nEnd = GetNameOffset(n+1);
			UINT nSize = nEnd - nStart;
			std::vector<UINT>::iterator it = m_vecStart.begin();
			for(; it != m_vecStart.end(); it++)
			{
				if(*it == nStart)
				{
					m_vecStart.erase(it);
					break;
				}
			}

			if(nEnd != (UINT)-1)
			{
				CopyMemory(&m_vecBuffer.front()+nStart,&m_vecBuffer.front()+nEnd,m_vecBuffer.size()-nEnd);
				m_vecBuffer.resize(m_vecBuffer.size()-nSize);
			}
			else
			{
				m_vecBuffer.resize(nStart);
			}
		}

		return -1;
	}

	UINT CNameTable::FindName(const TCHAR * lpszName)
	{
		for(UINT i=0;i<m_vecStart.size();i++)
		{
			const TCHAR * lpszDest = &m_vecBuffer.front() + m_vecStart[i];
			if(!lstrcmp(lpszDest,lpszName))
				return i;
		}
		return (UINT)-1;
	}

	UINT CNameTable::GetNameCount() const
	{
		return m_vecStart.size();
	}

	UINT CNameTable::GetNameOffset(UINT nIndex) const
	{
		if(nIndex>=0 && nIndex<m_vecStart.size())
		{
			return m_vecStart[nIndex];
		}
		return -1;
	}

	void CNameTable::SwapName(UINT nIndex0, UINT nIndex1)
	{
		if (nIndex0 < nIndex1 && nIndex1 - nIndex0 == 1)
		{
			UINT nOffset1 = GetNameOffset(nIndex0);
			UINT nOffset2 = GetNameOffset(nIndex1);

			TCHAR szName1[MAX_PATH];
			_stprintf(szName1, _T("%s"), GetName(nOffset1));
			TCHAR szName2[MAX_PATH];
			_stprintf(szName2, _T("%s"), GetName(nOffset2));

			UINT nStrLen1 = _tcslen(szName1)+1;
			UINT nStrLen2 = _tcslen(szName2)+1;

			UINT nOffset = nOffset1;
			for (UINT n=nOffset; n<nOffset+nStrLen2; n++)
			{
				m_vecBuffer[n] = szName2[n-nOffset];
			}
			m_vecStart[nIndex0] = nOffset;

			nOffset = nOffset1+nStrLen2;
			for (UINT n=nOffset; n<nOffset+nStrLen1; n++)
			{
				m_vecBuffer[n] = szName1[n-nOffset];
			}
			m_vecStart[nIndex1] = nOffset;

		}
	}







	t_scene::t_scene()
	{
		mTerrain = NULL;

		mNameTable = NULL;
	}

	t_scene::~t_scene()
	{
		safe_delete (mTerrain);
		safe_delete_array (mNameTable);
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

			else if (ck.dwFlag == STAGE_TEXTURE)
			{
				mNameTableSize = ck.dwChunkSize;
				mNameTable = new char[mNameTableSize];
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

		mTextureTable.Create(mNameTableSize, mNameTable);

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