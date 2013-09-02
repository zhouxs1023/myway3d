


#include "MWSocket.h"

using namespace Myway;

#define safe_colose(s)     { if (s != INVALID_SOCKET) _closesocket(s); s = INVALID_SOCKET; }



SOCKET _socket(int domain , int type , int protocol)
{
    SOCKET s = ::socket(domain,type,protocol);

    if ( s == INVALID_SOCKET ) 
    {

#ifdef MW_PLATFORM_LINUX

        switch ( errno ) 
        {
        case EPROTONOSUPPORT:
            d_assert(0);
            break;

        case EMFILE: 
            d_assert(0);
            break;

        case ENFILE: 
            d_assert(0);
            break;

        case EACCES: 
            d_assert(0);
            break;

        case ENOBUFS:
            d_assert(0);
            break;

        default: 
            d_assert(0);
            break;
        }

#else

    #ifdef MW_PLATFORM_WIN32
        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED:
            d_assert(0);
            break;

        case WSAENETDOWN:
            d_assert(0);
            break;

        case WSAEAFNOSUPPORT:
            d_assert(0);
            break;

        case WSAEINPROGRESS: 
            d_assert(0);
            break;

        case WSAEMFILE:
            d_assert(0);
            break;

        case WSAENOBUFS:
            d_assert(0);
            break;

        case WSAEPROTONOSUPPORT:
            d_assert(0);
            break;

        case WSAEPROTOTYPE:
            d_assert(0);
            break;

        case WSAESOCKTNOSUPPORT:
            d_assert(0);
            break;

        default:
            d_assert(0);
            break;
        };
    #endif

#endif
    }

    return s;
}

int _bind(SOCKET s , const struct sockaddr * addr , UINT addrlen)
{
    int ret = bind ( s , addr , addrlen );

    if (ret == SOCKET_ERROR ) 
    {
#ifdef MW_PLATFORM_LINUX
        switch ( errno ) 
        {
        case EADDRINUSE:
            d_assert(0);
            break;

        case EINVAL:
            d_assert(0);
            break;

        case EACCES:
            d_assert(0);
            break;

        case ENOTSOCK:
            d_assert(0);
            break;

        case EBADF:
            d_assert(0);
            break;

        case EROFS:
            d_assert(0);
            break;

        case EFAULT:
            d_assert(0);
            break;

        case ENAMETOOLONG:
            d_assert(0);
            break;

        case ENOENT:
            d_assert(0);
            break;

        case ENOMEM:
            d_assert(0);
            break;

        case ENOTDIR:
            d_assert(0);
            break;

        case ELOOP:
            d_assert(0);
            break;

        default:
            d_assert(0);
            break;
        }

#else
    #ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED: 
            d_assert(0);
            break;

        case WSAENETDOWN: 
            d_assert(0);
            break;

        case WSAEADDRINUSE: 
            d_assert(0);
            break;

        case WSAEADDRNOTAVAIL: 
            d_assert(0);
            break;

        case WSAEFAULT: 
            d_assert(0);
            break;

        case WSAEINPROGRESS: 
            d_assert(0);
            break;

        case WSAEINVAL: 
            d_assert(0);
            break;

        case WSAENOBUFS: 
            d_assert(0);
            break;

        case WSAENOTSOCK: 
            d_assert(0);
            break;

        default:
            d_assert(0);
            break;
        };
    #endif
#endif
    }

    return ret;
}

int _select(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, const timeval * timeout)
{
    int ret = select(nfds, readfds, writefds, exceptfds, timeout);

    if (ret == SOCKET_ERROR)
    {

#ifdef MW_PLATFORM_LINUX

#else
#ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError();
        switch (err) 
        {
        case WSANOTINITIALISED:
            d_assert(0);
            break;
        case WSAEFAULT:
            d_assert(0);
            break;
        case WSAENETDOWN:
            d_assert(0);
            break;
        case WSAEINVAL:
            d_assert(0);
            break;
        case WSAEINTR:
            d_assert(0);
            break;
        case WSAEINPROGRESS:
            d_assert(0);
            break;
        case WSAENOTSOCK:
            d_assert(0);
            break;
        default :
            break;
        };

#endif
#endif
    }

    return ret;
}



int _connect( SOCKET s, const struct sockaddr * addr, int addrlen)
{
    int ret = connect(s,addr,addrlen);
    if (ret == SOCKET_ERROR ) 
    {
#ifdef MW_PLATFORM_LINUX

        switch (errno)
        {
        case EALREADY: 
        case EINPROGRESS: 
        case ECONNREFUSED: 
        case EISCONN: 
        case ETIMEDOUT: 
        case ENETUNREACH: 
        case EADDRINUSE: 
        case EBADF: 
        case EFAULT: 
        case ENOTSOCK: 
        default:
            break;
        }

#else
    #ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED: 
            d_assert(0);
            break;
        case WSAENETDOWN: 
            d_assert(0);
            break;
        case WSAEADDRINUSE: 
            d_assert(0);
            break;
        case WSAEINTR: 
            d_assert(0);
            break;
        case WSAEINPROGRESS: 
            d_assert(0);
            break;
        case WSAEALREADY: 
            d_assert(0);
            break;
        case WSAEADDRNOTAVAIL: 
            d_assert(0);
            break;
        case WSAEAFNOSUPPORT: 
            d_assert(0);
            break;
        case WSAECONNREFUSED: 
            d_assert(0);
            break;
        case WSAEFAULT: 
            break ;
        case WSAEINVAL: 
            d_assert(0);
            break;
        case WSAEISCONN: 
            d_assert(0);
            break;
        case WSAENETUNREACH: 
            d_assert(0);
            break;
        case WSAENOBUFS: 
            d_assert(0);
            break;
        case WSAENOTSOCK: 
            d_assert(0);
            break;
        case WSAETIMEDOUT: 
            d_assert(0);
            break;
        case WSAEWOULDBLOCK: 
            d_assert(0);
            break;
        default:
            d_assert(0);
            break;
        };

    #endif

#endif
    }

    return ret;
}


int _listen(SOCKET s, int backlog)
{
    int ret = listen(s, backlog);

    if (ret == SOCKET_ERROR)
    {

#ifdef MW_PLATFORM_LINUX

        switch (errno) 
        {
        case EBADF:
        case ENOTSOCK:
        case EOPNOTSUPP:
        default:
            d_assert(0);
            break;
        }
#else
    #ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED: 
            d_assert(0);
            break;
        case WSAENETDOWN: 
            d_assert(0);
            break;
        case WSAEADDRINUSE: 
            break ;
        case WSAEINPROGRESS: 
            d_assert(0);
            break;
        case WSAEINVAL: 
            d_assert(0);
            break;
        case WSAEISCONN: 
            d_assert(0);
            break;
        case WSAEMFILE: 
            d_assert(0);
            break;
        case WSAENOBUFS: 
            d_assert(0);
            break;
        case WSAENOTSOCK: 
            d_assert(0);
            break;
        case WSAEOPNOTSUPP: 
            d_assert(0);
            break;
        default:
            d_assert(0);
            break;
        };

    #endif
#endif
    }

    return ret;
}

SOCKET _accept(SOCKET s, sockaddr * addr, int * addrlen)
{
    SOCKET c = accept(s, addr, addrlen);

    if (c == INVALID_SOCKET)
    {
#ifdef MW_PLATFORM_LINUX
        switch ( errno ) 
        {
        case EWOULDBLOCK: 
        case ECONNRESET:
        case ECONNABORTED:
        case EPROTO:
        case EINTR:
            // from UNIX Network Programming 2nd, 15.6
            // with nonblocking-socket, ignore above errors
        case EBADF: 
        case ENOTSOCK: 
        case EOPNOTSUPP: 
        case EFAULT: 
        default:
            d_assert(0);
            break;
        }

#else
    #ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED: 
            d_assert(0);
            break;
        case WSAENETDOWN: 
            d_assert(0);
            break;
        case WSAEFAULT: 
            d_assert(0);
            break;
        case WSAEINTR: 
            d_assert(0);
            break;
        case WSAEINPROGRESS: 
            d_assert(0);
            break;
        case WSAEINVAL: 
            d_assert(0);
            break;
        case WSAEMFILE: 
            d_assert(0);
            break;
        case WSAENOBUFS: 
            d_assert(0);
            break;
        case WSAENOTSOCK: 
            d_assert(0);
            break;
        case WSAEOPNOTSUPP: 
            d_assert(0);
            break;
        case WSAEWOULDBLOCK: 
            d_assert(0);
            break;
        default:
            d_assert(0);
            break;
        };
    #endif
#endif
    }

    return c;
}


int _getsockopt(SOCKET s, int level, int optname, void * optval, int * optlen)
{

    int ret = getsockopt(s , level , optname , (char*)optval , (int*)optlen);

    if (ret == SOCKET_ERROR)
    {
#ifdef MW_PLATFORM_LINUX

        switch (errno) 
        {
        case EBADF: 
        case ENOTSOCK: 
        case ENOPROTOOPT: 
        case EFAULT: 
        default:
            break;
        };

#else
    #ifdef MW_PLATFORM_WIN32
        
        int err = WSAGetLastError() ;
        switch ( err ) 
        {
        case WSANOTINITIALISED: 
            d_assert(0);
            break;
        case WSAENETDOWN: 
            d_assert(0);
            break;
        case WSAEFAULT: 
            d_assert(0);
            break;
        case WSAEINPROGRESS: 
            d_assert(0);
            break;
        case WSAEINVAL: 
            d_assert(0);
            break;
        case WSAENOPROTOOPT: 
            d_assert(0);
            break;
        case WSAENOTSOCK: 
            d_assert(0);
            break;
        default: 
            d_assert(0);
            break;
        };

    #endif

#endif
    }

    return ret;
}



int _setsockopt(SOCKET s, int level, int optname, const void * optval, int optlen)
{
    int ret = setsockopt(s, level, optname, (const char *)optval, optlen);

    if (ret == SOCKET_ERROR)
    {
#ifdef MW_PLATFORM_LINUX

        switch (errno) 
        {
        case EBADF: 
        case ENOTSOCK: 
        case ENOPROTOOPT: 
        case EFAULT: 
        default:
            break;
        }

#else

    #ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError();
        switch (err) 
        {
        case WSANOTINITIALISED: 
            d_assert(0);
            break;
        case WSAENETDOWN: 
            d_assert(0);
            break;
        case WSAEFAULT: 
            d_assert(0);
            break;
        case WSAEINPROGRESS: 
            d_assert(0);
            break;
        case WSAEINVAL: 
            d_assert(0);
            break;
        case WSAENETRESET: 
            d_assert(0);
            break;
        case WSAENOPROTOOPT: 
            d_assert(0);
            break;
        case WSAENOTCONN: 
            d_assert(0);
            break;
        case WSAENOTSOCK: 
            d_assert(0);
            break;
        default:
            d_assert(0);
            break;
        }

    #endif

#endif
    }

    return ret;
}


int _send(SOCKET s, const void * buf, int len, int flags)
{
    int ret = send(s, (const char *)buf, len, flags);

    if (ret == SOCKET_ERROR) 
    {
#ifdef MW_PLATFORM_LINUX

        switch (errno) 
        {
        case EWOULDBLOCK : 
            return 0;
        case ECONNRESET :
        case EPIPE :
        case EBADF : 
        case ENOTSOCK : 
        case EFAULT : 
        case EMSGSIZE : 
        case ENOBUFS : 
        default : 
             break;
        }
#else
    #ifdef MW_PLATFORM_WIN32

        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED :
            d_assert(0);
            break;
        case WSAENETDOWN : 
            d_assert(0);
            break;
        case WSAEACCES : 
            d_assert(0);
            break;
        case WSAEINTR : 
            d_assert(0);
            break;
        case WSAEINPROGRESS : 
            d_assert(0);
            break;
        case WSAEFAULT : 
            d_assert(0);
            break;
        case WSAENETRESET : 
            d_assert(0);
            break;
        case WSAENOBUFS : 
            d_assert(0);
            break;
        case WSAENOTCONN : 
            d_assert(0);
            break;
        case WSAENOTSOCK : 
            d_assert(0);
            break;
        case WSAEOPNOTSUPP : 
            d_assert(0);
            break;
        case WSAESHUTDOWN : 
            d_assert(0);
            break;
        case WSAEWOULDBLOCK: 
            return 0;
        case WSAEMSGSIZE : 
            d_assert(0);
            break;
        case WSAEHOSTUNREACH : 
            d_assert(0);
            break;
        case WSAEINVAL : 
            d_assert(0);
            break;
        case WSAECONNABORTED : 
            d_assert(0);
            break;
        case WSAECONNRESET : 
            d_assert(0);
            break;
        case WSAETIMEDOUT : 
            d_assert(0);
            break;
        default :
            d_assert(0);
            break;
        }
    #endif
#endif
    }

    return ret;
}

int _sendto(SOCKET s, const void * buf, int len, int flags, const sockaddr * to, int tolen)
{
    int ret = sendto(s, (const char *)buf, len, flags, to, tolen);

    if (ret == SOCKET_ERROR)
    {
#ifdef MW_PLATFORM_LINUX
        switch (errno) 
        {
        case EWOULDBLOCK : 
            return 0;
        case ECONNRESET :
        case EPIPE :
        case EBADF : 
        case ENOTSOCK : 
        case EFAULT : 
        case EMSGSIZE : 
        case ENOBUFS : 
        default : 
            break;
        }
#else
    #ifdef MW_PLATFORM_WIN32
        // ...
    #endif
#endif
    }

    return ret;
}


int _recv(SOCKET s, void * buf, int len , int flags)
{
    int ret = recv(s, (char *)buf, len, flags);

    if (ret == SOCKET_ERROR)
    {
#ifdef MW_PLATFORM_LINUX

        switch (errno) 
        {
        case EWOULDBLOCK : 
            return 0;

        case ECONNRESET :
        case EPIPE :
        case EBADF : 
        case ENOTCONN : 
        case ENOTSOCK : 
        case EINTR : 
        case EFAULT : 

        default : 
            break;
        }

#else
#ifdef MW_PLATFORM_WIN32
        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED : 
            d_assert(0);
            break;
        case WSAENETDOWN : 
            d_assert(0);
            break;
        case WSAEFAULT : 
            d_assert(0);
            break;
        case WSAENOTCONN : 
            d_assert(0);
            break;
        case WSAEINTR : 
            d_assert(0);
            break;
        case WSAEINPROGRESS : 
            d_assert(0);
            break;
        case WSAENETRESET : 
            d_assert(0);
            break;
        case WSAENOTSOCK : 
            d_assert(0);
            break;
        case WSAEOPNOTSUPP : 
            d_assert(0);
            break;
        case WSAESHUTDOWN : 
            d_assert(0);
            break;
        case WSAEWOULDBLOCK : 
            return 0;
            break;
        case WSAEMSGSIZE : 
            d_assert(0);
            break;
        case WSAEINVAL : 
            d_assert(0);
            break;
        case WSAECONNABORTED : 
            d_assert(0);
            break;
        case WSAETIMEDOUT : 
            d_assert(0);
            break;
        case WSAECONNRESET : 
            d_assert(0);
            break;
        default :
            d_assert(0);
            break;
        };
#endif
#endif
    }

    return ret;
}


int _recvfrom(SOCKET s, void * buf, int len, int flags, sockaddr * from , int * fromlen)
{
    int ret = recvfrom(s, (char *)buf, len, flags, from, fromlen);

    if (ret == SOCKET_ERROR)
    {
#ifdef MW_PLATFORM_LINUX
        switch (errno) 
        {
        case EWOULDBLOCK: 
            return 0;

        case ECONNRESET :
        case EPIPE :

        case EBADF : 
        case ENOTCONN : 
        case ENOTSOCK : 
        case EINTR : 
        case EFAULT : 

        default : 
            break;
        }
#else
    #ifdef MW_PLATFORM_WIN32
    #endif
#endif
    }

    return ret;
}

int _closesocket(SOCKET s)
{
#ifdef MW_PLATFORM_LINUX
    close(s);
    return 0;
#else

#ifdef MW_PLATFORM_WIN32
    int ret = closesocket(s);

    if (ret == SOCKET_ERROR)
    {
        int err = WSAGetLastError() ;
        switch ( err ) 
        {
        case WSANOTINITIALISED : 
            d_assert(0);
            break;
        case WSAENETDOWN : 
            d_assert(0);
            break;
        case WSAENOTSOCK : 
            d_assert(0);
            break;
        case WSAEINPROGRESS : 
            d_assert(0);
            break;
        case WSAEINTR : 
            d_assert(0);
            break;
        case WSAEWOULDBLOCK : 
            d_assert(0);
            break;
        default : 
            d_assert(0);
            break;
        };
    }

    return ret;
#endif

#endif
    
}

int _ioctlsocket(SOCKET s, int cmd, int * argp)
{
#ifdef MW_PLATFORM_LINUX

#error 0

#else

#ifdef MW_PLATFORM_WIN32
    int ret = ioctlsocket(s, cmd, (u_long*)argp);
    if (ret == SOCKET_ERROR) 
    {
        int err = WSAGetLastError() ;
        switch ( err ) 
        {
        case WSANOTINITIALISED : 
            d_assert(0);
            break;
        case WSAENETDOWN : 
            d_assert(0);
            break;
        case WSAEINPROGRESS : 
            d_assert(0);
            break;
        case WSAENOTSOCK : 
            d_assert(0);
            break;
        case WSAEFAULT : 
            d_assert(0);
            break;
        default :
            d_assert(0);
            break;
        };
    }
    
    return ret;
#endif

#endif
}



int _shutdown(SOCKET s, int how)
{
    int ret = shutdown(s, how);

    if (ret < 0) 
    {
#ifdef MW_PLATFORM_LINUX

        switch ( errno )
        {
        case EBADF : 
        case ENOTSOCK : 
        case ENOTCONN : 
        default : 
            break;
        }

#else
#ifdef MW_PLATFORM_WIN32
        int err = WSAGetLastError() ;
        switch (err) 
        {
        case WSANOTINITIALISED : 
            d_assert(0);
            break;
        case WSAENETDOWN :
            d_assert(0);
            break;
        case WSAEINVAL : 
            d_assert(0);
            break;
        case WSAEINPROGRESS : 
            d_assert(0);
            break;
        case WSAENOTCONN : 
            d_assert(0);
            break;
        case WSAENOTSOCK : 
            d_assert(0);
            break;
        default :
            d_assert(0);
            break;
        };
#endif
#endif

    }

    return ret;
}













static fd_set s_rd, s_wt, s_ex;

bool Socket::Init()
{
    WORD version = MAKEWORD(2, 1);
    WSADATA data;

    if (WSAStartup(version, &data) != 0)
        return FALSE;

    if (LOBYTE(data.wVersion) != LOBYTE(version) || 
        HIBYTE(data.wVersion) != HIBYTE(version))
    {
        WSACleanup();
        return FALSE;
    }

    return TRUE;
}

void Socket::Shutdown()
{
    WSACleanup();
}


void Socket::Open(Socket * s)
{
    assert (s->sock == INVALID_SOCKET);

    s->sock = _socket(AF_INET, SOCK_STREAM, 0);

    assert (s->sock != INVALID_SOCKET);
}


void Socket::Close(Socket * s)
{
    safe_colose(s->sock);
}

bool Socket::IsValid(Socket * s)
{
    return s->sock != INVALID_SOCKET;
}

void Socket::SetReuseAddr(Socket * s, bool enable)
{
    assert (s->sock != INVALID_SOCKET);

    _setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(bool));
}

void Socket::SetKeepActive(Socket * s, bool enable)
{
    assert (s->sock != INVALID_SOCKET);

    _setsockopt(s->sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&enable, sizeof(bool));
}

void Socket::SetSendBufferSize(Socket * s, int size)
{
    assert (s->sock != INVALID_SOCKET);

    _setsockopt(s->sock, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(int));
}

void Socket::SetRecvBufferSize(Socket * s, int size)
{
    assert (s->sock != INVALID_SOCKET);

    _setsockopt(s->sock, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int));
}

void Socket::EnableNoBlock(Socket * s, bool enable)
{
    assert (s->sock != INVALID_SOCKET);

    _ioctlsocket(s->sock, FIONBIO, (int *)&enable);
}

void Socket::SetLinger(Socket * s, int time)
{
    assert (s->sock != INVALID_SOCKET);

    linger l;
    l.l_onoff = time > 0 ? 1 : 0;
    l.l_linger = time;

    _setsockopt(s->sock, SOL_SOCKET, SO_LINGER, (const char*)&l, sizeof(linger));
}

int Socket::GetReadSize(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    int size;

    if (_ioctlsocket(s->sock, FIONREAD, (int *)&size) == SOCKET_ERROR)
        size = 0;

    return size;
}

void Socket::FD_Clear()
{
    FD_ZERO(&s_rd);
    FD_ZERO(&s_wt);
    FD_ZERO(&s_ex);
}

void Socket::FD_SetRD(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    FD_SET(s->sock, &s_rd);
}

void Socket::FD_SetWT(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    FD_SET(s->sock, &s_wt);
}

void Socket::FD_SetEX(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

     FD_SET(s->sock, &s_ex);
}

void Socket::FD_CLRRD(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    FD_CLR(s->sock, &s_rd);
}

void Socket::FD_CLRWT(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    FD_CLR(s->sock, &s_wt);
}

void Socket::FD_CLREX(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    FD_CLR(s->sock, &s_ex);
}

bool Socket::FD_IsSetRD(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    return FD_ISSET(s->sock, &s_rd) == TRUE;
}

bool Socket::FD_IsSetWT(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    return FD_ISSET(s->sock, &s_wt) == TRUE;
}

bool Socket::FD_IsSetEX(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

    return FD_ISSET(s->sock, &s_ex) == TRUE;
}

bool Socket::FD_Select(int seconds, int microseconds)
{
    TIMEVAL val;
    val.tv_sec = seconds;
    val.tv_usec = microseconds;

    return _select(0, &s_rd, &s_wt, &s_ex, &val) != SOCKET_ERROR;
}


bool Socket::Connect(Socket * s, const char * ip, int port)
{
    assert (s->sock != INVALID_SOCKET);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if (connect(s->sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        safe_colose(s->sock);
        return FALSE;
    }

    return TRUE;
}

bool Socket::Bind(Socket * s, int port, int clients)
{
    assert (s->sock != INVALID_SOCKET);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);

    int ret = TRUE;
    
    ret = _bind(s->sock, (sockaddr*)&addr, sizeof(addr));

    if (ret == SOCKET_ERROR)
        return FALSE;

    ret = _listen(s->sock, clients);

    if (ret == SOCKET_ERROR)
        return FALSE;

    return TRUE;
}

bool Socket::Accept(Socket * cl, Socket * sv)
{
    assert (cl->sock == INVALID_SOCKET && sv->sock != INVALID_SOCKET);

    int len = sizeof(cl->addr);

    cl->sock = _accept(sv->sock, (sockaddr*)&cl->addr, &len);

    return cl->sock != INVALID_SOCKET;
}

const char * Socket::GetIp(Socket * s)
{
    assert (s->sock != INVALID_SOCKET);

	static char ip[128];

    in_addr n;
    n.S_un.S_addr = s->addr.sin_addr.S_un.S_addr;

    Strcpy(ip, 128, inet_ntoa(n));

	return ip;
}

int Socket::Send(Socket * s, const void * buffer, int size)
{
    assert (s->sock != INVALID_SOCKET);

    int count = _send(s->sock, (const char *)buffer, size, 0);

    return count;
}

int Socket::Recv(Socket * s, void * buffer, int size)
{
    assert (s->sock != INVALID_SOCKET);

    int count = _recv(s->sock, (char*)buffer, size, 0);

    return count;
}
