#include "stdafx.h"

#include "Pack_AddNpcSV.h"
#include "MainMode.h"
#include "Npc.h"

Pack_AddNpcSV::Pack_AddNpcSV()
	: mNpcId(-1)
	, mTypeId(-1)
	, mHP(100)
	, mMP(100)
	, mPosition(0, 0, 0)
	, mDirection(0)
	, mState(STATE_Idle)
{
}

Pack_AddNpcSV::~Pack_AddNpcSV()
{
}

void Pack_AddNpcSV::In(PkIStream & stream)
{
	stream.Read(&mNpcId, sizeof(int));
	stream.Read(&mTypeId, sizeof(int));
	stream.Read(&mHP, sizeof(int));
	stream.Read(&mMP, sizeof(int));
	stream.Read(&mPosition, sizeof(Vec3));
	stream.Read(&mDirection, sizeof(float));
	stream.Read(&mState, sizeof(int));
}

void Pack_AddNpcSV::Execute(void * param)
{
	GmMode * mode = GmRoot::Instance()->GetMode();

	d_assert (RTTI_TypeOf(cMainMode, mode));

	cNpc * npc = new cNpc(mTypeId);

	npc->SetId(mNpcId);
	npc->SetPosition(mPosition);
	npc->SetDirection(mDirection);
	npc->SetHP(mHP);
	npc->SetMP(mMP);

	npc->Idle();

	mode->AddObject(npc);
}