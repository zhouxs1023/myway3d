#pragma once

#include "NetPack.h"
#include "Player.h"

class User
{
	friend class UserManager;

public:
	static const int INVALIDE_USER_ID = -1;

public:
	enum State
	{
		UNKNOWN,
		CONNECTED,
		WAIT_FOR_VERIFY,
		LOGIN,
		PLAY,
	};

public:
	User();
	~User();

	void        Init();
	void        Shutdown();

	void        SetAccount(const char * number);
	Socket *    GetSocket();

	void        SetState(int state);
	int         GetState();

	int         GetUserId();

	void		SetUserPower(int power) { mUserPower = power; }
	int			GetUserPower() { return mUserPower; }

	const char * GetUserIp() const;

	bool        SendPacket(NetPacket * pack);

	bool        _Input();
	bool        _Output();
	bool        _Process();

	bool        IsValid();

	void		SetPlayer(svPlayer * _player) { mPlayer = _player; }
	svPlayer *	GetPlayer() { return mPlayer; }

protected:
	TString128  mAccount;
	TString128	mIp;

	int mUserId;
	int mUserPower;

	Socket mSock;
	NetIStream * mIStream;
	NetOStream * mOStream;

	int mState;

	svPlayer * mPlayer;
};

