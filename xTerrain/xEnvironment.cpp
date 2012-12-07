#include "stdafx.h"
#include "xEnvironment.h"
#include "xEvent.h"
#include "xEnvironmentPane.h"
#include "xSerializer.h"

xEnvironment gEnvironment;

xEnvironment::xEnvironment()
	: OnNewScene(&xEvent::OnNewScene, this, &xEnvironment::_OnNewScene)
	, OnLoadScene(&xEvent::OnLoadScene, this, &xEnvironment::_OnLoadScene)
	, OnUnloadScene(&xEvent::OnUnloadScene, this, &xEnvironment::_OnUnloadScene)
	, OnSerialize(&xEvent::OnSerialize, this, &xEnvironment::_OnSerialize)
{
}

xEnvironment::~xEnvironment()
{
}

void xEnvironment::_OnNewScene(void * param0, void * param1)
{
	Environment::Instance()->InitEv();

	xEnvironmentPane::Instance()->_Frush();
}

void xEnvironment::_OnLoadScene(void * param0, void * param1)
{
	Environment::Instance()->InitEv();
}

void xEnvironment::_OnUnloadScene(void * param0, void * param1)
{
	Environment::Instance()->DeInitEv();
}

void _SavePropertyObj(IPropertyObj * obj, xSerializer & Serializer)
{
	int iPropertySize = obj->GetPropertySize();

	for (int j = 0; j < iPropertySize; ++j)
	{
		const Property * p = obj->GetProperty(j);
		const void * data = obj->GetPropertyData(p);

		Serializer << p->name;
		Serializer << p->size;
		Serializer.Write(data, p->size);
	}

	Serializer << TString128("_eof");
}

void _LoadPropertyObj(IPropertyObj * obj, xSerializer & Serializer)
{
	char buffer[1024];

	while (1)
	{
		TString128 name;
		int size;

		Serializer >> name;

		if (name == "_eof")
			break;

		Serializer >> size;

		d_assert (size < 1024);

		Serializer.Read(buffer, size);

		const Property * p = obj->GetProperty(name.c_str());

		if (p)
			obj->SetPropertyData(p, buffer);
	}
}

void xEnvironment::_OnSerialize(void * param0, void * param1)
{
	const int K_ChunkId = 'xEnv';
	int K_Version = 0;

	int chunkId = *(int *)param0;
	xSerializer & Serializer = *(xSerializer *)param1;

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

		xEnvironmentPane::Instance()->_Frush();
	}
}


