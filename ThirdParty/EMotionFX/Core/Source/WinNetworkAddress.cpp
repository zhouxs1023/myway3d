/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Macros.h"
#include "SourceDeclaration.h"
#include "LogManager.h"
#include "WinNetworkAddress.h"

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock.h>


namespace MCore
{


NetworkAddress::NetworkAddress() : mIsValid(false)
{ 
	mPort = -1;
}


NetworkAddress::NetworkAddress(unsigned char ip[4], const int portNr) : mIsValid(true)
{
	mAddress.Resize(4);
	for (int i=0; i<4; i++)
		mAddress[i] = ip[i];

	assert(portNr>=0 && portNr<=65535); // only valid portnumbers
	mPort = portNr;
}


NetworkAddress::NetworkAddress(const MCore::String& ipAddress, const int portNr) : mIsValid(true)
{ 
	// try to get the hostname
	hostent *h = gethostbyname(ipAddress.AsChar());
	if (h == NULL)
	{
		mIsValid = false;
		MCore::LOG("MCore::NetworkAddress - *** Failed to convert hostname to IP address! ***");
		return;
	}

	// make sure we have only 4 values in our address IPv4
	assert(h->h_length == 4);

	// copy our address
	IN_ADDR a = *((LPIN_ADDR)*h->h_addr_list);
	unsigned char ip[4];
	ip[0] = a.S_un.S_un_b.s_b1;
	ip[1] = a.S_un.S_un_b.s_b2;
	ip[2] = a.S_un.S_un_b.s_b3;
	ip[3] = a.S_un.S_un_b.s_b4;
	
	// add the IP to the array
	mAddress.Resize(4);
	for (int i=0; i<4; i++)
		mAddress[i] = ip[i];

	// store the port number
	assert(portNr>=0 && portNr<=65535); // only valid portnumbers
	mPort = portNr;
}


NetworkAddress::~NetworkAddress()
{
	mAddress.Clear();
}



int NetworkAddress::GetPort() const
{ 
	return mPort; 
}


void NetworkAddress::SetPort(const int portNr)
{ 
	mPort = portNr; 
}


void NetworkAddress::SetAddress(const String& ipAddress)
{
	// split the network address string into seperate values
	MCore::Array<String> ipValues = ipAddress.Split('.');

	// convert all values to integers and store them into our network address array
	for (int i=0; i<ipValues.GetLength(); i++)
		mAddress.Add( ipValues[i].ToInt() );
}


void NetworkAddress::SetAddress(const String& ipAddress, const int portNr)
{
	mIsValid = true;
	SetAddress( ipAddress );
	SetPort( portNr );
}


String NetworkAddress::GetAddressAsString() const
{ 
	// construct the ip address string from the ip address values
	MCore::String finalString;
	for (int i=0; i<mAddress.GetLength(); i++)
	{
		// if it's not the last number to add, add the number plus a dot
		if (i < mAddress.GetMaxIndex())
			finalString.FormatAdd("%d.", mAddress[i]);
		else	// add only the number, since it's the last one, and we don't want the dot at the end
			finalString.FormatAdd("%d", mAddress[i]);
	}

	// return the result
	return finalString;
}


const MCore::Array<unsigned char>& NetworkAddress::GetAddressAsInts() const
{ 
	return mAddress; 
}


}	// namespace MCore

#endif // #if PLATFORM == PLATFORM_WINDOWS

