#pragma once

#include "User.h"
#include "UserManager.h"
#include "World.h"

#define SV_CLIENT       256

class Server
{
    DECLARE_SINGLETON(Server);

public:
    Server();
    ~Server();

    void Run();

protected:
    void Init();
    void Shutdown();

	void LoadConfig();

    void Loop();

    bool IsQuit();
    void Quit();

    bool _SVSelect();
    bool _SVAccept();
    bool _SVExcept();

    bool _USSelect();
    bool _USInput();
    bool _USOutput();
    bool _USExcept();
    bool _USProcess();

    void _RemoveUser(int index);

protected:
	void Sync(float elapsedTime);

protected:
    Socket mMySocket;
    UserManager mUserManager;
    bool mQuit;
	Timer mTimer;

	float mInternalTime;
	float mLastSyncTime;
	float mFrameTime;

	svWorld mWorld;
};