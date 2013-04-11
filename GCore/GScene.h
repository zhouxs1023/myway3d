#pragma once

#include "GMesh.h"
#include "GLight.h"
#include "GSound.h"

namespace game {

	class GScene
	{
		DECLARE_ALLOC();

	public:
		static const int K_Magic = 'GS00';
		static const int K_Version = 1;

		static const int K_MeshChunk = 'MS00';
		static const int K_LightChunk = 'LT00';
		static const int K_SoundChunk = 'SD00';
		static const int K_EnvChunk = 'EV00';

	public:
		GScene();
		~GScene();

		bool Load(const char * source);

	protected:
		void _loadMeshes(DataStreamPtr stream);
		void _loadSounds(DataStreamPtr stream);
		void _loadLights(DataStreamPtr stream);
		void _loadEnvironment(DataStreamPtr stream);

	protected:
		Array<GMesh *> mMeshes;
		Array<GLight *> mLights;
		Array<GSound *> mSounds;
	};

}