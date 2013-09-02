#pragma once

class cNpc : public GmActor
{
	DeclareRTTI();

public:
	cNpc(int _typeId);
	virtual ~cNpc();

	virtual const TString128 & GetName();

	int GetTypeId() { return mTypeId; }

	void SetHP(int _hp) { mHP = _hp; }
	int GetHP() { return mHP; }

	void SetMP(int _mp) { mMP = _mp; }
	int GetMP() { return mMP; }

protected:
	const GmNpcInfo * mNpcInfo;

	int mTypeId;
	int mHP;
	int mMP;
};