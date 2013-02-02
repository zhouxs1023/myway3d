/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __WINNETWORK_H
#define __WINNETWORK_H

#include "Macros.h"
#include "SourceDeclaration.h"

#if PLATFORM == PLATFORM_WINDOWS

#include "WinSocket.h"
#include "Singleton.h"
#include <winsock.h>


namespace MCore
{


/// A shortcut to the network singleton
#define NETWORK Network::GetInstance()

/**
 * The network singleton.
 * This master class is the core of all networking activities.
 * It is responsible for catching all network related messages, and transporting them to the right place.
 * A network thread runs in the background, which updates processes the networking messages every 40 milliseconds, which
 * is exactly 25 times per second, which should be fast enough for any type of application.
 * It is very important that you call NETWORK.Init() before you use any network related classes, such as Socket, NetworkAddress and NetworkSystem.
 */
class Network
{
	DECLARE_CLASS( Network )
	SINGLETON( Network )

	friend class Socket;

	public:
		/**
		 * The constructor, which automatically creates the message window and initializes winsock.
		 */
		Network();

		/**
		 * The destructor, which automatically destroys the created thread and window, etc.
		 */
		~Network();

		/**
		 * Initialize the network system. You need to call this as first thing, since it won't happen automatically.
		 * This needs to be called before you do anything network related. If you do not call this, sockets will not be
		 * created successfully, and messages will not arrive the network systems.
		 */
		void Init();

		/**
		 * Process all the network messages.
		 * This is automatically done by the thread which is created during the Init() method.
		 */
		void ProcessMessages();

		/**
		 * Get the number of sockets, which have been created and are still alive.
		 * @result The number of sockets inside the network.
		 */
		inline int GetNumSockets() const	{ return mSockets.GetLength(); }

		/**
		 * Get a given socket.
		 * @param nr The socket number, which must be in range of [0..GetNumSockets()-1].
		 * @result A pointer to the socket.
		 */
		inline Socket* GetSocket(const int nr)	{ return mSockets[nr]; }


	private:
		WNDCLASSEX						mWC;		/**< The window class description, which receives the messages. */
		WSADATA							mInfo;		/**< The winsock information. */
		MCore::Array< MCore::Socket* >	mSockets;	/**< The sockets that have been created. */
		HANDLE							mThread;	/**< The message thread handle. */
		HWND							mWnd;		/**< The window which receives the messages. */

		/**
		 * Register a given socket.
		 * Do not manually call this. This is all been done automatically.
		 * @param socket The socket to register.
		 */
		void RegisterSocket(Socket* socket);

		/**
		 * Unregister a socket from the network.
		 * This is all done automatically, so don't make any calls to this.
		 * @param socket The socket to unregister.
		 */
		void UnRegisterSocket(Socket* socket);

		/**
		 * Unregister all created sockets.
		 * This is automatically called by the destructor.
		 */
		void UnRegisterAllSockets();

		/**
		 * Get the handle of the message processing window.
		 * @result The HWND of the message processing window.
		 */
		inline HWND GetWindow() const	{ return mWnd; }

};


/// The network message processing function.
extern LRESULT WINAPI WinNetworkMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// The network message processing thread loop.
extern DWORD WINAPI NetworkThreadProc(LPVOID lpParameter);


} // namespace MCore


#endif	// #if PLATFORM == PLATFORM_WINDOWS

#endif