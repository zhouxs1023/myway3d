/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __POP3CLIENT_H
#define __POP3CLIENT_H


// include Core related headers
#include "Macros.h"
#include "SourceDeclaration.h"
#include "Array.h"
#include "String.h"
#include "NetworkHeaders.h"


namespace MCore
{

// check if we have found a network implementation for this platform
#if HAS_NETWORK_IMPLEMENTATION == 1

/**
 * The POP3 mail client class.
 * With this class it is possible to receive messages from a POP3 server.
 * Functionalities offered by this class are: receiving mails, receiving message headers, receiving the number of 
 * messages on the server and deleting messages from the server.
 */
class POP3Client : public NetworkSystem
{
	public:
		/**
		 * The POP3 message header class.
		 * This contains some parsed information about the message, such as the identification of the sender,
		 * the receiver of the message, the subject and the time and date of the message.
		 */
		class MessageHeader
		{
			friend class POP3Client;

			public:
				/**
				 * The constructor.
				 */
				MessageHeader()  {}

				/**
				 * The destructor.
				 */
				~MessageHeader() {}

				/**
				 * Get the identification of the sender. This can be name or a name followed by an email address.
				 * It depends on the software the mail has been written in.
				 * @result The string containing the identification of the sender, so who wrote the mail.
				 */
				const MCore::String&	GetSender()	const		{ return mFrom; }

				/**
				 * Get the identification of the sender. So for who is this mail for?
				 * This can be either a name or email address, or a list of email addresses or a combination of them.
				 * It depends on the software the mail has been written in.
				 * @result The string containing the identification of the receiver, so who this mail is for.
				 */
				const MCore::String& GetReceiver() const		{ return mTo; }

				/**
				 * Get the subject of the mail.
				 * @result The string containing the subject of the mail.
				 */
				const MCore::String& GetSubject() const		{ return mSubject; }

				/**
				 * Get the date and time of the mail.
				 * @result The string containing the date and time when this mail was written.
				 */
				const MCore::String& GetDate() const			{ return mDate; }


			protected:
				MCore::String	mFrom;		/**< The sender of this mail. */
				MCore::String	mTo;		/**< The receiver of this mail. */
				MCore::String	mSubject;	/**< The subject of the mail. */
				MCore::String	mDate;		/**< The date and time the mail has been written. */
		};

		//-----------------------------------------------------------------------

		/**
		 * The POP3 message class.
		 * This class contains a parsed message header and the raw data (in a string) of the message as it was stored
		 * on the mail server.
		 */
		class Message
		{
			friend class POP3Client;

			public:
				/**
				 * The constructor.
				 */
				Message()  {}

				/**
				 * The destructor.
				 */
				~Message() {}

				/**
				 * Get the string containing the raw data of the mail.
				 * This raw data includes the unparsed header of the mail. Basically this is the complete message as
				 * how it is stored on the POP3 mail server.
				 * @result The string containing the complete message.
				 */
				const MCore::String& GetData() const			{ return mData; }

				/**
				 * Get the parsed header information of this message.
				 * This header contains some extracted information from the string returned by GetData().
				 * Using the MessageHeader class it is easy to get things such as the sender, subject and date of the message.
				 */
				const MessageHeader& GetHeader() const		{ return mHeader; }


			protected:
				MessageHeader	mHeader;	/**< The parsed message header. */
				MCore::String	mData;		/**< The complete message, including unparsed header. */	
		};

		//-----------------------------------------------------------------------

		/**
		 * The constructor.
		 */
		POP3Client();

		/**
		 * The destructor.
		 * This does not automatically logout of the server on a nice way (when connected).
		 * However, it makes sure the socket will disconnect from the server. Users should call the Disconnect() method
		 * in order to disconnect from a server though.
		 */
		~POP3Client();

		/**
		 * Get the last error in form of a string.
		 * This will contain a reason returned by the POP3 server, when a given operation fails.
		 * @result The string containing the reason for the last error.
		 */
		const MCore::String& GetLastError() const;

		/**
		 * Disconnect from the POP3 server according to the POP3 protocol.
		 * You should never just close the connection by closing the socket. So always use this method to disconnect
		 * from the server.
		 */
		void Disconnect();

		/**
		 * Try to connect to a specified POP3 mail server.
		 * @param server The string containing the server address, for example "pop.somedomain.com".
		 * @param portNr The port number to connect to, which is usually 110 for a POP3 server.
		 * @result Returns true when a connection has been established or false when not.
		 */
		bool Connect(const MCore::String& server, const int portNr=110);

		/**
		 * Try to login to the POP3 mail server. You first have to be connected (using the Connect() method) before
		 * trying to login. You have to specify a username and password.
		 * @param username The username to login with.
		 * @param password The password to login with.
		 * @result Returns true when successfully logged in, otherwise false.
		 */
		bool Login(const MCore::String& username, const MCore::String& password);

		/**
		 * Get the number of messages which are stored on the server.
		 * You have to be connected and logged in before using this method.
		 * Do not put this method inside a for-all-messages loop, because it needs to download some information from the server
		 * everytime it is being called. Fortunately the number of messages cannot change during your session, even if you receive new mail
		 * when you are connecting to the mail server.
		 * @result The number of messages on the server, or -1 when there was an error.
		 */
		int GetNumMessagesOnServer();

		/**
		 * Download the message header of a given message.
		 * This is the fastest way to find out the details about the messages stored on the server, without actually
		 * downloading the whole message.
		 * @param outHeader A pointer to the message header you want to fill with the message information.
		 * @param messageNr The message number, which must be in range of [0..GetNumMessagesFromServer()-1].
		 * @result Returns true when the header has been successfully downloaded, or false when it didn't work out good.
		 */
		bool DownloadMessageHeader(MessageHeader* outHeader, const int messageNr);

		/**
		 * Download a complete message from the server. This includes the message header as well.
		 * @param outMessage A pointer to the message object, which you want to fill with the message data.
		 * @param messageNr The message number, which must be in range of [0..GetNumMessagesFromServer()-1].
		 * @result Returns true when the message has been successfully downloaded, or false when it didn't work out good.
		 */
		bool DownloadMessage(Message* outMessage, const int messageNr);

		/**
		 * Delete a given message from the server.
		 * The message will not be deleted immediately, but will only be marked for deletion.
		 * After you successfully logged out from the server (using the Disconnect()) method, the messages which are marked
		 * for deletion will be deleted for real.
		 * @param messageNr The message number, which must be in range of [0..GetNumMessagesFromServer()-1].
		 * @result Returns true when we successfully marked the message to be deleted, otherwise false is returned.
		 */
		bool DeleteMessage(const int messageNr);

		/**
		 * Try to download all the messages on the server.
		 * @param outMessages A pointer to the array to which we this method should add all the downloaded message objects.
		 * @param delFromServer When set to true, all messages will be marked for deletion, for once you successfully logoff from the server, using the Disconnect() method.
		 * @result Returns true when all messages have been downloaded, otherwise false is returned.
		 */
		bool DownloadAllMessages(MCore::Array<Message>* outMessages, bool delFromServer=false);

		/**
		 * Try to download all the message headers on the server.
		 * @param outMessages A pointer to the array to which we this method should add all the downloaded message header objects.
		 * @result Returns true when all message headers have been downloaded, otherwise false is returned.
		 */
		bool DownloadAllMessageHeaders(MCore::Array<MessageHeader>* outHeaders);

		//-----------------------

		// overloaded virtual methods from the NetworkSystem base class.
		// they don't do anything though, since we do not use any asynchronous sending and receiving.
		void OnRead(Socket* socket)		{}
		void OnWrite(Socket* socket)	{}
		void OnClose(Socket* socket)	{}
		void OnAccept(Socket* socket)	{}


	private:
		String		mLastError;		/**< The last error which occured. */
		Socket*		mSocket;		/**< The socket we use to communicate with the POP3 server. */

		/**
		 * Extract the header from a given string.
		 * This will fill in the members of the MessageHeader class.
		 * @param messageOrHeader The string containing the message or header as stored on the POP3 server.
		 * @param outHeader A pointer to the header object of which this method should write the extracted information to.
		 */
		void ExtractHeader(const MCore::String& messageOrHeader, MessageHeader* outHeader);

		/**
		 * Read incoming data from the mail server.
		 * @result The string containing the received data.
		 */
		String Read();

		/**
		 * Check if the last sent command to the server has been executed successfully or not.
		 * @param input The input, which is the received responds from the server, after we sent the command.
		 * @result Returns true when the command has been successfully executed, otherwise false is returned.
		 */
		bool IsOK(const MCore::String& input) const;

		/**
		 * Send a command to the server.
		 * @param command The command to send to the server.
		 * @result Returns true when we have successfully sent the command to the server.
		 */
		bool Send(const MCore::String& command);

		/**
		 * Wait for the responds of the server after we have sent a command to it.
		 * In case the respond indicates something went wrong, we extract the returned reason/error message, which is 
		 * then stored inside the string which is returned by GetLastError().
		 * @param Returns true when the command is successfully executed, otherwise false is returned.
		 */
		bool WaitForOK();

		/**
		 * Extracts the error message from a server responds when something went wrong.
		 * @param input The input string, which is the responds from the server.
		 * @result A string containing the error message, which has been extracted from the server responds.
		 */
		String ExtractError(const MCore::String& input);
};

#endif	// if HAS_NETWORK_IMPLEMENTATION == 1

}	// namespace MCore

#endif