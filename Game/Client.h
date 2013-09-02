#pragma once

#include "NetPack.h"

class Client
{
	DECLARE_SINGLETON(Client);

public:
    Client();
    virtual ~Client();

    void Connect();
	void DeConnect();
	bool IsConnect();

    void Update();

    void SendPacket(NetPacket * pack);

protected:
    bool _Select();
    bool _Input();
    bool _Output();
    bool _Except();

    void _Process();

protected:
    Socket       mSock;
    NetIStream * mIStream;
    NetOStream * mOStream;

    TString128 mServerIp;
};