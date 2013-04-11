#include "stdafx.h"

#include "GScene.h"

namespace game {

GScene::GScene()
{
}

GScene::~GScene()
{
}

bool GScene::Load(const char * source)
{
	DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

	if (stream == NULL)
		return false;

	int magic, version;

	stream->Read(&magic, sizeof(int));
	stream->Read(&version, sizeof(int));

	d_assert (magic == K_Magic && version == K_Version);

	int chunkId;

	while (stream->Read(&chunkId, sizeof(int)))
	{
		switch (chunkId)
		{
		case K_MeshChunk:
			_loadMeshes(stream);
			break;

		case K_LightChunk:
			_loadLights(stream);
			break;

		case K_SoundChunk:
			_loadSounds(stream);
			break;

		case K_EnvChunk:
			_loadEnvironment(stream);
			break;

		default:
			d_assert (0);
			break;
		}
	}

	return true;
}

void GScene::_loadMeshes(DataStreamPtr stream)
{
	int verison = 0;

	stream->Read(&verison, sizeof(int));

	if (verison == 1)
	{
		int count = 0;

		stream->Read(&count, sizeof(int));

		while (count--)
		{
			char name[128];
			char source[128];
			Vec3 position;
			Quat orientation;
			float scale;

			stream->Read(name, 128);
			stream->Read(source, 128);
			stream->Read(&position, sizeof(Vec3));
			stream->Read(&orientation, sizeof(Vec3));
			stream->Read(&scale, sizeof(float));

			GMesh * mesh = new GMesh(name, source);
			mesh->SetPosition(position);
			mesh->SetOrientation(orientation);
			mesh->SetScale(scale);

			mMeshes.PushBack(mesh);
		}
	}
}

void GScene::_loadSounds(DataStreamPtr stream)
{
	int verison = 0;

	stream->Read(&verison, sizeof(int));

	if (verison == 1)
	{
		int count = 0;

		stream->Read(&count, sizeof(int));

		while (count--)
		{
			char name[128];
			char source[128];
			Vec3 position;
			float minDist, maxDist;
			int volume;

			stream->Read(name, 128);
			stream->Read(source, 128);
			stream->Read(&position, sizeof(Vec3));
			stream->Read(&minDist, sizeof(float));
			stream->Read(&maxDist, sizeof(float));
			stream->Read(&volume, sizeof(int));

			GSound * sound = new GSound(name, source);
			sound->SetPosition(position);
			sound->SetDistance(minDist, maxDist);
			sound->SetVolume(volume);

			mSounds.PushBack(sound);
		}
	}
}

void GScene::_loadLights(DataStreamPtr stream)
{
	int verison = 0;

	stream->Read(&verison, sizeof(int));

	if (verison == 1)
	{
		int count = 0;

		stream->Read(&count, sizeof(int));

		while (--count)
		{
			char name[128];
			Vec3 position;
			Quat orientation;
			int type;
			float range;
			float inner, outer;

			stream->Read(name, 128);
			stream->Read(&position, sizeof(Vec3));
			stream->Read(&orientation, sizeof(Vec3));
			stream->Read(&type, sizeof(int));
			stream->Read(&range, sizeof(float));
			stream->Read(&inner, sizeof(float));
			stream->Read(&outer, sizeof(float));

			GLight * light = new GLight(name);

			light->SetPosition(position);
			light->SetOrientation(orientation);
			light->getLight()->SetType((LIGHT_TYPE)type);
			light->getLight()->SetRange(range);
			light->getLight()->SetInner(inner);
			light->getLight()->SetOuter(outer);

			mLights.PushBack(light);
		}
	}
}

inline void _readPropertyObj(IPropertyObj * obj, DataStreamPtr stream)
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

void GScene::_loadEnvironment(DataStreamPtr stream)
{
	int verison = 0;

	stream->Read(&verison, sizeof(int));

	if (verison == 1)
	{
		EvKeyFrame * kNight = Environment::Instance()->GetKeyFrame(EVKT_Night);
		EvKeyFrame * kMorning = Environment::Instance()->GetKeyFrame(EVKT_Morning);
		EvKeyFrame * kNoon = Environment::Instance()->GetKeyFrame(EVKT_Noon);
		EvKeyFrame * kEvening = Environment::Instance()->GetKeyFrame(EVKT_Evening);
		EvGlobalParam * kGlobal = Environment::Instance()->GetGlobalParam();

		_readPropertyObj(kNight, stream);
		_readPropertyObj(kMorning, stream);
		_readPropertyObj(kNoon, stream);
		_readPropertyObj(kEvening, stream);
		_readPropertyObj(kGlobal, stream);
	}
}

}
