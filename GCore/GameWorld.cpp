#include "stdafx.h"

#include "GameWorld.h"

namespace Game {

	GameWorld::GameWorld()
	{
	}

	GameWorld::~GameWorld()
	{
	}

	void GameWorld::LoadScene(const char * filename)
	{
		mSceneFile = filename;

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);

		d_assert (stream != NULL);

		int Magic, Version;

		stream->Read(&Magic, sizeof(int));
		stream->Read(&Version, sizeof(int));

		d_assert (Magic == K_FILE_MAGIC && Version == K_FILE_VERSION);

		int chunkId = -1;

		while (stream->Read(&chunkId, sizeof(int)) == 1)
		{
			switch (chunkId)
			{
			case FCI_Enviroment:
				LoadEnviroment(stream);
				break;

			case FCI_Terrain:
				LoadTerrain(stream);
				break;

			case FCI_Mesh:
				LoadMesh(stream);
				break;

			case FCI_Forest:
				LoadForest(stream);
				break;

			case FCI_Particle:
				LoadParticle(stream);
				break;

			case FCI_Sound:
				LoadSound(stream);
				break;

			default:
				d_assert(0);
				break;
			}
		}
	}

	void GameWorld::LoadEnviroment(DataStreamPtr stream)
	{
		int version = -1;

		stream->Read(&version, sizeof(int));

		d_assert (version == K_Enviroment_Version);

		EvKeyFrame * kNight = Environment::Instance()->GetKeyFrame(EVKT_Night);
		EvKeyFrame * kMorning = Environment::Instance()->GetKeyFrame(EVKT_Morning);
		EvKeyFrame * kNoon = Environment::Instance()->GetKeyFrame(EVKT_Noon);
		EvKeyFrame * kEvening = Environment::Instance()->GetKeyFrame(EVKT_Evening);
		EvGlobalParam * kGlobal = Environment::Instance()->GetGlobalParam();

		_loadProperty(stream, kNight);
		_loadProperty(stream, kMorning);
		_loadProperty(stream, kNoon);
		_loadProperty(stream, kEvening);
		_loadProperty(stream, kGlobal);
	}

	void GameWorld::LoadTerrain(DataStreamPtr stream)
	{
		int version = -1;
		stream->Read(&version, sizeof(int));
		d_assert (version = K_Terrain_Version);

		// terrain
		TString128 sceneFile = mSceneFile;
		sceneFile = File::RemoveExternName(sceneFile);
		sceneFile += ".terrain";

		Environment::Instance()->LoadTerrain(sceneFile.c_str());
		Terrain * kTerrain = Environment::Instance()->GetTerrain();

		float h = Math::Minimum(kTerrain->GetConfig().xSize, kTerrain->GetConfig().zSize);
		World::Instance()->Resize((int)kTerrain->GetConfig().xSize, (int)h, (int)kTerrain->GetConfig().zSize);

		// water
		Environment::Instance()->GetWaterManager()->GetWater()->Init();

		TString128 waterFile = sceneFile;
		waterFile = File::RemoveExternName(waterFile);
		waterFile += ".water";

		WaterManager::Instance()->GetWater()->Load(waterFile.c_str());
	}

	void GameWorld::LoadMesh(DataStreamPtr stream)
	{
		int version = -1;

		stream->Read(&version, sizeof(int));

		d_assert(version == K_Mesh_Version);

		int count = 0;

		stream->Read(&count, sizeof(int));

		for (int i = 0; i < count; ++i)
		{
			char MeshFile[128];
			Vec3 Position;
			Quat Orientation;
			float Scale;

			stream->Read(MeshFile, 128);
			stream->Read(&Position, sizeof(Vec3));
			stream->Read(&Orientation, sizeof(Quat));
			stream->Read(&Scale, sizeof(float));

			GameMesh * mesh = new GameMesh(MeshFile);

			mesh->SetPosition(Position);
			mesh->SetOrientation(Orientation);
			mesh->SetScale(Scale);
		}
	}

	void GameWorld::LoadForest(DataStreamPtr stream)
	{
		
	}

	void GameWorld::LoadParticle(DataStreamPtr srream)
	{
	}

	void GameWorld::LoadSound(DataStreamPtr stream)
	{

	}

	void GameWorld::_loadProperty(DataStreamPtr stream, IPropertyObj * obj)
	{
		char buffer[1024];

		while (1)
		{
			char name[128];
			int size;

			stream->Read(name, 128);

			if (strcmp(name, "_eof") == 0)
				break;

			stream->Read(&size, sizeof(int));

			d_assert (size < 1024);

			stream->Read(buffer, size);

			const Property * p = obj->GetProperty(name);

			if (p)
				obj->SetPropertyData(p, buffer);
		}
	}

}



