#pragma once

#ifndef FD_SETSIZE
#define FD_SETSIZE 2048
#endif

#include <winsock.h>

#include "MWAllocObj.h"

#ifdef MW_PLATFORM_WIN32
// ...
#endif

#define NET_ERROR -1
#define NET_ERROR_WOULDBLOCK -2

namespace Myway
{

struct MW_ENTRY Socket : public AllocObj
{
    sockaddr_in addr;
    SOCKET sock;

public:

    Socket() : sock(INVALID_SOCKET) {};

    static bool Init();
    static void Shutdown();

    static void Open(Socket * s);
    static void Close(Socket * s);
    static bool IsValid(Socket * s);

    static void SetReuseAddr(Socket * s, bool enable);
    static void SetKeepActive(Socket * s, bool enable);
    static void SetSendBufferSize(Socket * s, int size);
    static void SetRecvBufferSize(Socket * s, int size);
    static void EnableNoBlock(Socket * s, bool enable);
    static void SetLinger(Socket * s, int time);
    static int GetReadSize(Socket * s);

    static void FD_Clear();
    static void FD_SetRD(Socket * s);
    static void FD_SetWT(Socket * s);
    static void FD_SetEX(Socket * s);
    static void FD_CLRRD(Socket * s);
    static void FD_CLRWT(Socket * s);
    static void FD_CLREX(Socket * s);
    static bool FD_IsSetRD(Socket * s);
    static bool FD_IsSetWT(Socket * s);
    static bool FD_IsSetEX(Socket * s);
    static bool FD_Select(int seconds, int microseconds);

    static bool Connect(Socket * s, const char * ip, int port);
    static bool Bind(Socket * s, int port, int clients);
    static bool Accept(Socket * cl, Socket * sv);
    static void GetIp(char * ip, int size, Socket * s);

    static int Send(Socket * s, const void * buffer, int size);
    static int Recv(Socket * s, void * buffer, int size);
};

}