/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "LogManager.h"
#include "WinSocket.h"
#include "WinNetwork.h"
#include "NetworkSystem.h"


namespace MCore
{

#if PLATFORM == PLATFORM_WINDOWS

// constructor
Socket::Socket(NetworkSystem* parentSystem) : mSocket(NULL)
{
	Init();

	mNetworkSystem = parentSystem;
	NETWORK.RegisterSocket( this );
}



// destructor
Socket::~Socket() 
{
	Close();
	NETWORK.UnRegisterSocket(this);
}


void Socket::Init()
{
	// close the socket first, in case there is something still open
	Close();

	// initialize the socket
	mSocket = socket(	AF_INET,		// internet protocol
						SOCK_STREAM,	// a stream, so connection oriented (safe)
						IPPROTO_TCP	);	// TCP/IP protocol

	// check if it went right
	if (mSocket == INVALID_SOCKET)
		MCore::LOG("MCore::Socket - *** Failed to create socket! ***");
}



int Socket::Read(void* data, int length)
{
	assert(mSocket != NULL);
	if (mSocket == NULL) return 0;
	
	// try to receive
	int bytes = recv(mSocket, (char*)data, length, 0);
	if (bytes == SOCKET_ERROR)
		return 0;

	// voilla!
	return bytes;
}



int Socket::Write(void* data, int length)
{
	if (mSocket == NULL) return 0;

	// try to send
	int bytes = send(mSocket, (char*)data, length, 0);
	if (bytes == SOCKET_ERROR) 
		return 0;

	// success! :)
	return bytes;
}



bool Socket::Bind(const MCore::NetworkAddress& localAddress)
{
	unsigned char ip[4];
	ip[0] = localAddress.GetAddressAsInts()[0];
	ip[1] = localAddress.GetAddressAsInts()[1];
	ip[2] = localAddress.GetAddressAsInts()[2];
	ip[3] = localAddress.GetAddressAsInts()[3];

	// setup the server address
	SOCKADDR_IN local;
	local.sin_family	= AF_INET;
	local.sin_addr		= *((LPIN_ADDR)ip);
	local.sin_port		= htons( localAddress.GetPort() );

	if (bind(mSocket, (LPSOCKADDR)&local, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		//MCore::LOG("MCore::Socket - *** Failed to bind socket *** (%d)", WSAGetLastError());
		Init();
		return false;
	}
	
	return true;
}



bool Socket::Connect(const MCore::NetworkAddress& targetAddress)
{
	//MCore::LOG("Connecting socket to '%s' at port %d...", targetAddress.GetAddressAsString().AsChar(), targetAddress.GetPort());

	unsigned char ip[4];
	ip[0] = targetAddress.GetAddressAsInts()[0];
	ip[1] = targetAddress.GetAddressAsInts()[1];
	ip[2] = targetAddress.GetAddressAsInts()[2];
	ip[3] = targetAddress.GetAddressAsInts()[3];

	// setup the server address
	SOCKADDR_IN server;
	server.sin_family	= AF_INET;
	server.sin_addr		= *((LPIN_ADDR)ip);
	server.sin_port		= htons( targetAddress.GetPort() );

	// try to connect
	if (connect(mSocket, (LPSOCKADDR)&server, sizeof(struct sockaddr)) == SOCKET_ERROR) 
	{
		//MCore::LOG("Socket() - *failed to connect* (%d)", WSAGetLastError());
		Init();
		return false;
	}

	// setup async
	ASyncSelect( EVENT_ALL );

	return true;
}



bool Socket::Connect(const MCore::String& targetAddress, const int portNumber)
{
	NetworkAddress address(targetAddress, portNumber);
	if (!address.IsValid())
		return false;

	return Connect( address );
}



void Socket::Close()
{
	if (mSocket == NULL) return;
	closesocket(mSocket);
	mSocket = NULL;
}



Socket* Socket::Accept()
{
	// the incoming call address info
	SOCKADDR_IN address;
	int size = sizeof(address);

	// look if we have incoming calls
	SOCKET client = accept( mSocket, (LPSOCKADDR)&address, &size);

	// we're getting no call
	if (client == INVALID_SOCKET) 
		return NULL;

	// create the socket and return it
	Socket* socket = new Socket( mNetworkSystem );
	socket->mSocket = client;

	// setup async
	ASyncSelect( EVENT_ALL );

	// if we want to know who is connecting with us
	unsigned char ip[4];
	ip[0] = address.sin_addr.S_un.S_un_b.s_b1;
	ip[1] = address.sin_addr.S_un.S_un_b.s_b2;
	ip[2] = address.sin_addr.S_un.S_un_b.s_b3;
	ip[3] = address.sin_addr.S_un.S_un_b.s_b4;

	NetworkAddress inAddress(ip, address.sin_port);
	socket->mTargetAddress = inAddress;

	return socket;
}



bool Socket::Listen(const int portNr, const int maxConnections)
{
	ASyncSelect( EVENT_ALL );

	// setup the server address
	SOCKADDR_IN server;
	server.sin_family		= AF_INET;
	server.sin_addr.s_addr	= INADDR_ANY;
	server.sin_port			= htons( portNr );

	// bind the socket to our local server address
	if (bind( mSocket, (LPSOCKADDR)&server, sizeof(struct sockaddr)) == SOCKET_ERROR) 
	{
		//MCore::LOG("Socket::Listen() - *failed to bind socket* (%d)", WSAGetLastError());
		Init();
		return false;
	}

	// listen
	if (listen(mSocket, maxConnections) == SOCKET_ERROR)
	{
		//MCore::LOG("Socket::Listen() - *failed to listen* (%d)", WSAGetLastError());
		Init();
		return false;
	}

	return true;
}



bool Socket::ASyncSelect(EASyncEvent events) const
{
	if (NETWORK.GetWindow())
	{
		long winEvent = 0;
		if (events & EVENT_READ)	winEvent |= FD_READ;
		if (events & EVENT_WRITE)	winEvent |= FD_WRITE;
		if (events & EVENT_ACCEPT)	winEvent |= FD_ACCEPT;
		if (events & EVENT_CONNECT)	winEvent |= FD_CONNECT;
		if (events & EVENT_CLOSE)	winEvent |= FD_CLOSE;

		if (WSAAsyncSelect(mSocket, NETWORK.GetWindow(), SOCKET_MSG, winEvent) == SOCKET_ERROR)
		{
			//MCore::LOG("Socket::ASyncSelect() - *WSAAsyncSelect failed* (%d)", WSAGetLastError());
			return false;
		}
		else

			return true;
	}

	return false;
}

#endif	// #if PLATFORM == PLATFORM_WINDOWS

}	// namespace MCore



