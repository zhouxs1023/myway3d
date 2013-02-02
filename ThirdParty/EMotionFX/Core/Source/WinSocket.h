/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __WINSOCKET_H
#define __WINSOCKET_H

#include "Macros.h"
#include "SourceDeclaration.h"

#if PLATFORM == PLATFORM_WINDOWS

#include "WinNetworkAddress.h"
#include "Stream.h"

#include <winsock.h>


namespace MCore
{

// foreward declarations
class NetworkSystem;


/**
 * The network socket class.
 * A socket is basically a connection between two network addresses.
 * It can be seen as a phone line, where a connection between two network addresses
 * on a port is formed by this socket class. You can send (write) and receive (read) data over
 * the sockets. A network system class is needed to be linked with each socket. This network system will receive and
 * process all network related messages, which are related to this socket.
 */
class Socket : public Stream
{
	DECLARE_CLASS( Socket )

	public:
		/**
		 * Asynchronous socket events, to be used with Socket::ASyncSelect(...).
		 */
		enum EASyncEvent
		{
			EVENT_READ		= 0x00000001,	/**< If you want to receive notifications of readiness for reading. */
			EVENT_WRITE		= 0x00000010,	/**< If you want to receive notification when data is available to be read. */
			EVENT_ACCEPT	= 0x00000100,	/**< If you want to receive notification of incoming connections. */
			EVENT_CONNECT	= 0x00001000,	/**< If you want to receive notification of connection results. */
			EVENT_CLOSE		= 0x00010000,	/**< If you want to receive notification when a socket has been closed by a peer. */
			EVENT_ALL		= ((EASyncEvent) EVENT_READ | EVENT_WRITE | EVENT_ACCEPT | EVENT_CONNECT | EVENT_CLOSE )	/**< Receive notifications on all events. */
		};
		
		/**
		 * The constructor.
		 * Automatically registers the socket inside the network singleton.
		 * @param parentSystem The network system where this socket belongs to. This networks system will be receiving the messages (OnRead, etc).
		 */
		Socket(NetworkSystem* parentSystem);

		/**
		 * The destructor. 
		 * Automatically unregisters the socket from the network singleton.
		 * Also automatically closes the socket in case it hasn't been done yet.
		 */
		~Socket();

		/**
		 * Receive data on the socket.
		 * @param data The pointer where to store the read data.
		 * @param length The size in bytes of the data to read.
		 * @result Returns true when the reading succeeded or false when the data could not be read.
		 */
		int Read(void* data, int length);

		/**
		 * Send data over the socket.
		 * @param data The pointer to the data to send.
		 * @param length The size in bytes of the data to send.
		 * @result Returns the number of sent bytes.
		 */
		int Write(void* data, int length);
		
		/**
		 * Bind the socket to a local address.
		 * @param localAddress The local address to bind the socket with.
		 * @result Returns true when the operation succeeded, otherwise false is returned.
		 */
		bool Bind(const MCore::NetworkAddress& localAddress);

		/**
		 * Connect this socket to a given address.
		 * @param targetAddress The address to connect to.
		 * @result Returns true when the operation succeeded, otherwise false is returned.
		 */
		bool Connect(const MCore::NetworkAddress& targetAddress);

		/**
		 * Connect this socket to a given address.
		 * @param targetAddress The address to connect to, in form of a string.
		 * @param portNumber The port number to connect to at the given address.
		 * @result Returns true when the operation succeeded, otherwise false is returned.
		 */
		bool Connect(const MCore::String& targetAddress, const int portNumber);

		/**
		 * Initializes the socket.
		 * It can be used to re-initialize the socket as well, because it will first close the socket if it is needed.
		 * Initialization will happen afterwards. Currently all sockets use TCP/IP, which guarantees packet delivery.
		 */
		void Init();

		/**
		 * Close the socket.
		 */
		void Close();
		
		/**
		 * Accept an incoming call.
		 * @result A pointer to the socket which contains the new connection, or NULL when something went wrong.
		 */
		Socket* Accept();

		/**
		 * Makes the socket listen for incoming connections.
		 * Performs automatic binding. This basically starts a server, making this socket a server socket.
		 * @param portNr The port number to listen to.
		 * @param maxConnections The maximum number of connections allowed.
		 * @result Returns true when the opertation succeeded, otherwise false is returned.
		 */
		bool Listen(const int portNr, const int maxConnections);

		/**
		 * Setup the socket in asynchronous mode where you specify what events to receive.
		 * @param events The events you want to receive. You can OR them together.
		 * @result Returns true when the operation succeeded, otherwise false is returned.
		 */
		bool ASyncSelect(EASyncEvent events = EVENT_ALL) const;

		/**
		 * Returns a pointer to the network system which will receive and process the socket events/messages.
		 * @result A pointer to the network system.
		 */
		inline NetworkSystem* GetNetworkSystem() const	{ return mNetworkSystem; }

		/**
		 * Returns the winsock version of this socket.
		 * @result The winsock version of this socket.
		 */
		inline SOCKET GetWindowsSocket() const	{ return mSocket; }

		/**
		 * Get the network address where this socket is connected to.
		 * So it is NOT the local address, but the address of the machine where this socket is connected to.
		 * @result The network address where the socket is connected to.
		 */
		inline const MCore::NetworkAddress& GetAddress() const	{ return mTargetAddress; }


	protected:
		MCore::NetworkAddress	mTargetAddress;	/**< The target network address where this sockets is connected to (so the address on another computer). */
		NetworkSystem*			mNetworkSystem;	/**< The network system this socket belongs to. */
		SOCKET					mSocket;		/**< The winsock socket. */
};


// the message ID our window will receive when there is network activity
#define SOCKET_MSG	WM_USER + 69


} // namespace MCore

#endif	// #if PLATFORM == PLATFORM_WINDOWS

#endif