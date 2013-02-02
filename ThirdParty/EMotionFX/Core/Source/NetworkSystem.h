/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NETWORKSYSTEM_H
#define __NETWORKSYSTEM_H

#include "Macros.h"
#include "SourceDeclaration.h"

namespace MCore
{

// forward declaration
class Socket;


/**
 * The network system class.
 * A network system is a system which processes incoming network messages/events.
 * Every socket is linked to a network system. All messages for the given socket will be forwarded to
 * the network system where the socket is linked to. Example network systems could be server and client classes.
 * The NetworkSystem class acts as a base class, which only describes an interface of the classes to be inherited from this class.
 */
class NetworkSystem
{
	public:
		/**
		 * The constructor.
		 */
		NetworkSystem() {}

		/**
		 * The destructor.
		 */
		virtual ~NetworkSystem() {}

		/**
		 * This method will be called once there is incoming data ready to be read.
		 * @param socket The socket you should read the data from.
		 */
		virtual void OnRead(Socket* socket) = 0;

		/**
		 * This method will be called when the given socket is ready to write data.
		 * @param socket The socket which is ready to write.
		 */
		virtual void OnWrite(Socket* socket) = 0;

		/**
		 * This method will be called when the connection with a given socket is closed.
		 * You are responsible for closing and deleting the socket. This is not done automatically.
		 * @param socket The socket which closed the connection.
		 */
		virtual void OnClose(Socket* socket) = 0;

		/**
		 * This method will be called when someone is connecting to a specific socket.
		 * You have to call socket->Accept() to accept the incomming call. This will also give you
		 * a pointer to the socket which will connect you with the incoming client.
		 * @param socket The socket on which we have an incoming call.
		 */
		virtual void OnAccept(Socket* socket) = 0;
};


}	// namespace MCore


#endif