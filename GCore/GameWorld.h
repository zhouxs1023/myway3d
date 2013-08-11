#pragma once

#include "GameCoreEntry.h"
#include "GameMesh.h"

namespace Game {

	class GCORE_ENTRY GameWorld
	{
	public:
		enum FileChunkId
		{
			FCI_Enviroment = 1000,
			FCI_Terrain = 2000,
			FCI_Mesh = 3000,
			FCI_Particle = 4000,
			FCI_Sound = 5000,
			FCI_Forest = 6000,
		};

		static const int K_FILE_MAGIC = 'MWLD';
		static const int K_FILE_VERSION = 'WD01';

		static const int K_Enviroment_Version = 1;
		static const int K_Terrain_Version = 1;
		static const int K_Mesh_Version = 1;
		static const int K_Particle_Version = 1;
		static const int K_Sound_Version = 1;
		static const int K_Forest_Version = 1;

	public:
		GameWorld();
		~GameWorld();

		void LoadScene(const char * filename);

	protected:
		void LoadEnviroment(DataStreamPtr stream);
		void LoadTerrain(DataStreamPtr stream);
		void LoadMesh(DataStreamPtr stream);
		void LoadForest(DataStreamPtr stream);
		void LoadParticle(DataStreamPtr srream);
		void LoadSound(DataStreamPtr stream);

	protected:
		void _loadProperty(DataStreamPtr stream, IPropertyObj * obj);

	protected:
		TString128 mSceneFile;
		Array<Entity*> mObjects;
	};

}