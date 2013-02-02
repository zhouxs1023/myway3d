/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */


#include "POP3Client.h"

namespace MCore
{

#if HAS_NETWORK_IMPLEMENTATION == 1


POP3Client::POP3Client()
{
	// make sure we have initialized the network system already
	NETWORK.Init();

	// create a new socket
	mSocket = new Socket(this);
}



POP3Client::~POP3Client()
{
	// make sure we have closed the connection
	mSocket->Close();
	delete mSocket;
}


const MCore::String& POP3Client::GetLastError() const
{
	return mLastError;
}


void POP3Client::Disconnect()
{
	Send("QUIT\r\n");
	mSocket->Close();
}


bool POP3Client::Connect(const MCore::String& server, const int portNr)
{
	// initialize the socket
	mSocket->Init();

	// try to connect to the server
	if (!mSocket->Connect(server, portNr))
	{
		mLastError = "Cannot make a connection with the server.";
		return false;
	}

	// disable async message handling
	mSocket->ASyncSelect((Socket::EASyncEvent)0);

	// wait for the +OK
	if (!WaitForOK())
	{
		mLastError = "This is not a POP3 server, or the server does not respond correctly.";
		return false;
	}

	return true;
}



bool POP3Client::Login(const MCore::String& username, const MCore::String& password)
{
	String command = "USER " + username + "\r\n";
	Send( command );
	if (!WaitForOK()) 
		return false;

	command = "PASS " + password + "\r\n";
	Send( command );
	if (!WaitForOK()) 
		return false;

	return true;
}



int POP3Client::GetNumMessagesOnServer()
{
	Send("LIST\r\n");
	bool needsRead = true;
	MCore::String incoming = Read();
	if (IsOK(incoming))
		if (!incoming.Find("-ERR") == 0)
			needsRead = false;
		else
			return -1;

	// download all parts of the list message, and stick them together
	MCore::String messageList;
	bool finished = false;
	int times = 0;
	while (!finished)
	{

		MCore::String input;
		if (needsRead)
		{
			input = Read();
		}
		else
			input = incoming;

		if (input.Find(".") != -1)
			finished = true;

		if (times == 0)
			messageList = input;
		else 
			messageList += input;

		needsRead = true;

		times++;
	}

	// split the lines
	MCore::Array<MCore::String> messages = messageList.Split('\n');

	// filter out incorrect messages
	for (int i=0; i<messages.GetLength(); )
	{
		messages[i].Trim();
		messages[i].RemoveChars("\r\n");

		const int numWords = messages[i].CalcNumWords();
		if (messages[i].IsEqual(".") || numWords != 2)
			messages.Remove(i);
		else
			i++;
	}

	return messages.GetLength();
}



bool POP3Client::DownloadMessageHeader(MessageHeader* outHeader, const int messageNr)
{			
	MCore::String command;
	command.Format("TOP %d 0\r\n", messageNr+1);
	Send( command );
	if (!WaitForOK())
		return false;

	// download all parts of the message, and stick them together
	MCore::String outString;
	bool finished = false;
	int times = 0;
	while (!finished)
	{
		MCore::String input = Read();

		int len = input.GetLength();
		if (input[len-3] == '.' && input[len-2] == '\r' && input[len-1] == '\n')
		{
			finished = true;
			*(input.GetPtr() + (len-3)) = '\0';
		}

		if (times == 0)
			outString = input;
		else 
			outString += input;

		times++;
	}

	// extract the message header
	ExtractHeader( outString, outHeader );

	return true;
}



bool POP3Client::DownloadMessage(Message* outMessage, const int messageNr)
{
	String command;
	command.Format("RETR %d\r\n", messageNr+1);
	Send( command );
	if (!WaitForOK())
		return false;

	// download all parts of the message, and stick them together
	MCore::String outString;
	bool finished = false;
	int times = 0;
	while (!finished)
	{
		String input = Read();

		int len = input.GetLength();
		if (input[len-3] == '.' && input[len-2] == '\r' && input[len-1] == '\n')
		{
			finished = true;
			*(input.GetPtr() + (len-3)) = '\0';
		}

		if (times == 0)
			outMessage->mData = input;
		else 
			outMessage->mData += input;

		times++;
	}

	// extract the header from the message
	ExtractHeader(outMessage->mData, &outMessage->mHeader);

	return true;
}



bool POP3Client::DeleteMessage(const int messageNr)
{
	MCore::String command;
	command.Format("DELE %d\r\n", messageNr+1);
	Send( command );
	if (!WaitForOK())
		return false;

	return true;
}



bool POP3Client::DownloadAllMessages(MCore::Array<Message>* outMessages, bool delFromServer)
{
	// clear any existing messages
	outMessages->Clear();

	// get the number of messages on the server
	const int numMessages = GetNumMessagesOnServer();

	// process all messages
	for (int i=0; i<numMessages; i++)
	{
		Message message;
		if (!DownloadMessage(&message, i))
			return false;

		// add the message to the array
		outMessages->Add( message );

		// if we have to delete this message
		if (delFromServer)
			if (!DeleteMessage(i))
				return false;
	}

	return true;
}




bool POP3Client::DownloadAllMessageHeaders(MCore::Array<MessageHeader>* outHeaders)
{
	// clear any existing messages
	outHeaders->Clear();

	// get the number of messages on the server
	const int numMessages = GetNumMessagesOnServer();

	// process all messages
	for (int i=0; i<numMessages; i++)
	{
		MessageHeader header;
		if (!DownloadMessageHeader(&header, i))
			return false;

		// add the header to the array
		outHeaders->Add( header );
	}

	return true;
}



void POP3Client::ExtractHeader(const MCore::String& messageOrHeader, MessageHeader* outHeader)
{
	MCore::Array<MCore::String> parts = messageOrHeader.Split('\n');
	for (int i=0; i<parts.GetLength(); i++)
	{
		parts[i].RemoveChars("\n\r");		

		int pos = parts[i].Find("To:");
		if (pos == 0)
			outHeader->mTo = String(parts[i].GetPtr() + pos + 4);

		pos = parts[i].Find("From:");
		if (pos == 0)
			outHeader->mFrom = String(parts[i].GetPtr() + pos + 6);

		pos = parts[i].Find("Subject:");
		if (pos == 0)
			outHeader->mSubject = String(parts[i].GetPtr() + pos + 9);

		pos = parts[i].Find("Date:");
		if (pos == 0)
			outHeader->mDate = String(parts[i].GetPtr() + pos + 6);
	}
}



String POP3Client::Read()
{
	int len = 0;
	char buffer[2048];
	while ((len = mSocket->Read(buffer, 2047)) == 0);
	buffer[len] = '\0';
	return String(buffer);
}



bool POP3Client::IsOK(const MCore::String& input) const
{
	return (input.Find("+OK") == 0);
}



bool POP3Client::Send(const MCore::String& command)
{
	if (mSocket->Write((void*)command.AsChar(), command.GetLength()) > 0)
		return true;

	return false;
}



bool POP3Client::WaitForOK()
{
	String incoming = Read();
	if (IsOK(incoming))
		return true;

	mLastError = ExtractError( incoming );

	return false;
}



MCore::String POP3Client::ExtractError(const MCore::String& input)
{
	if (input.Find("-ERR") == 0)
	{
		String result(input.AsChar() + 5);
		result.RemoveChars("\r\n");
		return result;
	}
	else
		return "";
}

#endif	// if HAS_NETWORK_IMPLEMENTATION == 1

}	// namespace MCore

