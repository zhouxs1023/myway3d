#include "stdafx.h"
#include "xEnvironment.h"
#include "xEvent.h"
#include "xSerializer.h"
#include "xScene.h"

namespace Infinite {

Event xEnvironment::OnChanged;

xEnvironment::xEnvironment()
	: OnNewScene(xEvent::OnNewScene, this, &xEnvironment::_OnNewScene)
	, OnLoadScene(xEvent::OnLoadScene, this, &xEnvironment::_OnLoadScene)
	, OnUnloadScene(xEvent::OnUnloadScene, this, &xEnvironment::_OnUnloadScene)
	, OnSerialize(xEvent::OnSerialize, this, &xEnvironment::_OnSerialize)
{
}

xEnvironment::~xEnvironment()
{
}

void xEnvironment::_OnNewScene(Event * sender)
{
	Environment::Instance()->InitEv();

	OnChanged();
}

void xEnvironment::_OnLoadScene(Event * sender)
{
	Environment::Instance()->InitEv();
}

void xEnvironment::_OnUnloadScene(Event * sender)
{
	Environment::Instance()->DeInitEv();
}

void _SavePropertyObj(IPropertyObj * obj, xSerializer & Serializer)
{
	Serializer << obj;
}

void _LoadPropertyObj(IPropertyObj * obj, xSerializer & Serializer)
{
	Serializer >> obj;
}

void xEnvironment::_OnSerialize(Event * sender)
{
	const int K_ChunkId = 'xEnv';
	int K_Version = 0;

	int chunkId = *(int *)sender->GetParam(0);
	xSerializer & Serializer = *(xSerializer *)sender->GetParam(1);

	if (Serializer.IsSave())
	{
		Serializer << K_ChunkId;
		Serializer << K_Version;

		EvKeyFrame * kNight = Environment::Instance()->GetKeyFrame(EVKT_Night);
		EvKeyFrame * kMorning = Environment::Instance()->GetKeyFrame(EVKT_Morning);
		EvKeyFrame * kNoon = Environment::Instance()->GetKeyFrame(EVKT_Noon);
		EvKeyFrame * kEvening = Environment::Instance()->GetKeyFrame(EVKT_Evening);
		EvGlobalParam * kGlobal = Environment::Instance()->GetGlobalParam();

		_SavePropertyObj(kNight, Serializer);
		_SavePropertyObj(kMorning, Serializer);
		_SavePropertyObj(kNoon, Serializer);
		_SavePropertyObj(kEvening, Serializer);
		_SavePropertyObj(kGlobal, Serializer);

	}
	else if (K_ChunkId == chunkId)
	{
		Serializer >> K_Version;

		d_assert (K_Version == 0);

		EvKeyFrame * kNight = Environment::Instance()->GetKeyFrame(EVKT_Night);
		EvKeyFrame * kMorning = Environment::Instance()->GetKeyFrame(EVKT_Morning);
		EvKeyFrame * kNoon = Environment::Instance()->GetKeyFrame(EVKT_Noon);
		EvKeyFrame * kEvening = Environment::Instance()->GetKeyFrame(EVKT_Evening);
		EvGlobalParam * kGlobal = Environment::Instance()->GetGlobalParam();
		
		_LoadPropertyObj(kNight, Serializer);
		_LoadPropertyObj(kMorning, Serializer);
		_LoadPropertyObj(kNoon, Serializer);
		_LoadPropertyObj(kEvening, Serializer);
		_LoadPropertyObj(kGlobal, Serializer);

		xScene::Instance()->DirtLoadChunk();

		OnChanged();
	}
}


}