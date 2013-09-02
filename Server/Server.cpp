#include "StdAfx.h"

#include "Server.h"

Server * Server::msInstance = NULL;

Server::Server()
{
	msInstance = this;

    System::Init();
    Memory::Init();
    Math::Init();
    Socket::Init();

	RegistNetPacket();
}

Server::~Server()
{
    Socket::Shutdown();
	Math::DeInit();
	Memory::DeInit();
    System::DeInit();

	msInstance = NULL;
}

void Server::LoadConfig()
{
}

void Server::Run()
{
    Init();

	mTimer.Start();

	mInternalTime = mTimer.ElapsedTime();
	mLastSyncTime = mInternalTime;
	mFrameTime = 0.001f;

    while (!IsQuit())
    {
        Loop();
    }

    Shutdown();
}

void Server::Init()
{
	printf("Server Init..\n");

    mQuit = false;

	LoadConfig();

    Socket::Open(&mMySocket);
    Socket::EnableNoBlock(&mMySocket, TRUE);
    if (!Socket::Bind(&mMySocket, SERVER_PORT, 3))
    {
        printf("bind server error.\n");
        Quit();
    }

	mWorld.Init();

	printf("Server Init OK.\n\n\n");
}

void Server::Shutdown()
{
	mWorld.Shutdown();

	mUserManager.DestroyAllUser();

	Socket::Close(&mMySocket);
}

void Server::Loop()
{
    if (!Socket::IsValid(&mMySocket))
    {
        mcmd << "door server error, must be quit." << mendl;
        Quit();
        return;
    }

    // update user message
    if (!_USSelect() || !_USExcept() || !_USInput() || !_USOutput() || !_USProcess())
    {
        d_assert(0);
        mcmd << "server must be quit." << mendl;
        Quit();
        return;
    }

	// 同步
	{
		float time = mTimer.ElapsedTime();
		float dtime = time - mLastSyncTime;

		if (dtime > 0.1f)
		{
			mLastSyncTime = time;

			Sync(dtime);
		}
	}
	
    // accept client
    if (!_SVSelect() || !_SVExcept() || !_SVAccept())
    {
        d_assert(0);
        printf("Server Socket error, must be quit!\n");
        Quit();
        return;
    }

	// update frame time
	{
		float time = mTimer.ElapsedTime();
		mFrameTime = time - mInternalTime;
		mInternalTime = time;
	}

	Sleep(1);
}

bool Server::IsQuit()
{
    return mQuit;
}

void Server::Quit()
{
    mQuit = true;
}

bool Server::_SVSelect()
{
    Socket::FD_Clear();
    Socket::FD_SetRD(&mMySocket);
    Socket::FD_SetEX(&mMySocket);

    if (!Socket::FD_Select(0, 5))
    {
        d_assert(0);
        return false;
    }

    return true;
}

bool Server::_SVAccept()
{
    if (!Socket::FD_IsSetRD(&mMySocket))
        return true;

    User * user = mUserManager.CreateUser();

    if (user)
    {
        Socket * cl = user->GetSocket();
        Socket::Accept(cl, &mMySocket);
        Socket::EnableNoBlock(cl, TRUE);
        Socket::SetLinger(cl, 0);

        user->SetState(User::CONNECTED);

        // 暂不处理
		printf("User connect: id \"%d\", ip \"%s\".\n", user->GetUserId(), user->GetUserIp());
    }

    return true;
}

bool Server::_SVExcept()
{
    if (Socket::FD_IsSetEX(&mMySocket))
    {
        return false;
    }

    return true;
}

bool Server::_USSelect()
{
    if (mUserManager.GetUserSize() == 0)
        return true;

    Socket::FD_Clear();

    for (int i = 0; i < MAX_USER; ++i)
    {
        User * user = mUserManager.GetUser(i);

        if (user)
        {
            Socket * sock = user->GetSocket();

            Socket::FD_SetRD(sock);
            Socket::FD_SetWT(sock);
            Socket::FD_SetEX(sock);
        }
    }

    if (!Socket::FD_Select(0, 100))
        d_assert(0);

    return true;

}

bool Server::_USInput()
{
    for (int i = 0; i < MAX_USER; ++i)
    {
        User * user = mUserManager.GetUser(i);

        if (user)
        {
            Socket * sock = user->GetSocket();

            if (Socket::FD_IsSetRD(sock))
            {
                if (!user->_Input())
                    _RemoveUser(i);
            }
        }
    }

    return true;
}

bool Server::_USOutput()
{
    for (int i = 0; i < MAX_USER; ++i)
    {
        User * user = mUserManager.GetUser(i);
        if (user)
        {
            Socket * sock = user->GetSocket();
            if (Socket::FD_IsSetWT(sock))
            {
                if (!user->_Output())
                    _RemoveUser(i);
            }
        }
    }

    return true;
}

bool Server::_USExcept()
{
    for (int i = 0; i < MAX_USER; ++i)
    {
        User * user = mUserManager.GetUser(i);
        if (user)
        {
            Socket * sock = user->GetSocket();

            if (Socket::FD_IsSetEX(sock))
            {
                _RemoveUser(i);
            }
        }
    }

    return true;
}

bool Server::_USProcess()
{
    for (int i = 0; i < MAX_USER; ++i)
    {
        User * user = mUserManager.GetUser(i);
        if (user)
        {
            user->_Process();
        }
    }

    return true;
}

void Server::_RemoveUser(int user)
{
    User * u = mUserManager.GetUser(user);

    if (u)
    {
		printf("User Removed : id \"%d\", ip \"%s\".\n", u->GetUserId(), u->GetUserIp());
        mUserManager.DestroyUser(u);
    }
}

void Server::Sync(float elapsedTime)
{
	mWorld.Update(elapsedTime);
}

