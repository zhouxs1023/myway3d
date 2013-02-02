/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "WinNetwork.h"
#include "NetworkSystem.h"
#include "LogManager.h"


#if PLATFORM == PLATFORM_WINDOWS


namespace MCore
{


// the network message processing proc
LRESULT WINAPI WinNetworkMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{ 
	// if we are receiving a socket message
	if (msg == SOCKET_MSG)
	{
		// get the socket and network
		SOCKET		winSocket= (SOCKET)wParam;
		Socket*		socket	 = NULL;

		// find the socket
		int numSockets = NETWORK.GetNumSockets();
		for (int i=0; i<numSockets && (socket==NULL); i++)
		{
			Socket* curSocket = NETWORK.GetSocket(i);
			if (winSocket == curSocket->GetWindowsSocket())
				socket = curSocket;
		}

		// if we don't have a socket, we can't continue
		if (socket == NULL)
			return TRUE;

		// in case of an error
		if (WSAGETSELECTERROR(lParam)) 
		{
			//MCore::LOG("Error, something weird happened (removing connection)");
			socket->GetNetworkSystem()->OnClose(socket);
			return FALSE;
		}

		// get the socket message
		switch (WSAGETSELECTEVENT(lParam)) 
		{
			// there is data ready to be read
			case FD_READ:
			{
				//MCore::LOG("FD_READ");
				socket->GetNetworkSystem()->OnRead(socket);
			}
			break;


			// we are ready to write
			case FD_WRITE:
			{
				//MCore::LOG("FD_WRITE");
				socket->GetNetworkSystem()->OnWrite(socket);
			}
			break;


			// connection closed
			case FD_CLOSE:
			{
				//MCore::LOG("FD_CLOSE");	
				socket->GetNetworkSystem()->OnClose(socket);
			}
			break;


			// close connection
			case FD_CONNECT:
			{
				//MCore::LOG("FD_CONNECT");
				//socket->GetNetworkSystem()->OnConnect(socket);
				socket->GetNetworkSystem()->OnClose(socket);
			}
			break;


			// accept connection
			case FD_ACCEPT:
			{
				//MCore::LOG("FD_ACCEPT");
				socket->GetNetworkSystem()->OnAccept(socket);
			}
			break;
		}

		// successfull, no quit message
		return TRUE;
	}
	else
		return DefWindowProc(hWnd, msg, wParam, lParam); 
}




// the network message processing thread loop
DWORD WINAPI NetworkThreadProc(LPVOID lpParameter)
{
	// keep it going forever, until we terminate it
	while (1)
	{
		// process the network messages
		NETWORK.ProcessMessages();

		// avoid eating up all CPU power
		Sleep(40);
	}

	return 0;
}


//----------------------------------------------------------------------------------

Network::Network()
{
	mThread = NULL;
	mWnd	= NULL;

	// try to init winsock
	unsigned short version = MAKEWORD(1, 1);
	if (WSAStartup(version, &mInfo) == SOCKET_ERROR) 
	{
		LOG("MCore::Network - *** Failed to init WinSock! ****");
		return;
	}
	//else
		//LOG("Winsock initialized");


	// try to create the window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WinNetworkMsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, "WinNetwork", NULL };
	mWC = wc;
	RegisterClassEx( &wc );
	HWND hwnd = CreateWindow("WinNetwork", "WinNetwork", WS_POPUP | WS_MINIMIZE, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, (HWND)NULL, NULL, GetModuleHandle(NULL), NULL);

	// if we have no window, something went wrong
	if (!hwnd)
	{
		UnregisterClass("WinNetwork", wc.hInstance);
		MCore::LOG("MCore::Network - *** Failed to create message window! ***");
		return;
	}

	// show the parent window
	ShowWindow(hwnd, false);
	UpdateWindow(hwnd);

	mWnd = hwnd;
}


Network::~Network()
{
	TerminateThread(mThread, 0);
	CloseHandle(mThread);
	UnRegisterAllSockets();
	DestroyWindow(mWnd);
	UnregisterClass("WinNetwork", mWC.hInstance);
	WSACleanup();
}


void Network::Init()
{
	// if the thread is already running, bail out
	if (mThread != NULL)
		return;

	DWORD threadID;
	mThread = CreateThread(NULL, 0, NetworkThreadProc, NULL, 0, &threadID);
}


void Network::ProcessMessages()
{
	// process the window (and socket) messages
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	while (PeekMessage(&msg, mWnd, 0, 0, PM_NOREMOVE))
	{
		GetMessage(&msg, mWnd, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void Network::RegisterSocket(Socket* socket)
{
	if (!mSockets.Contains(socket))
		mSockets.Add( socket );
}


void Network::UnRegisterSocket(Socket* socket)
{
	int nr = mSockets.Find( socket );
	if (nr == -1)
		return;

	mSockets.Remove( nr );
	//delete socket;
}


void Network::UnRegisterAllSockets()
{
	while (mSockets.GetLength())
	{
		//delete mSockets.GetLast();
		mSockets.RemoveLast();
	}
}



}	// namespace MCore

#endif	// #if PLATFORM == PLATFORM_WINDOWS