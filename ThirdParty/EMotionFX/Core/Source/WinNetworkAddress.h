/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __WINNETWORKADDRESS_H
#define __WINNETWORKADDRESS_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Array.h"
#include "String.h"


namespace MCore
{

#if PLATFORM == PLATFORM_WINDOWS

/**
 * The network address class, which represents an IP address.
 * It is for example used inside the socket class to describe the address the socket is connecting with.
 */
class NetworkAddress
{
	DECLARE_CLASS( NetworkAddress )

	public:
		/**
		 * The default constructor.
		 * If you use this, don't forget to call the SetPort and SetAddress methods.
		 */
		NetworkAddress();

		/**
		 * Extended constructor, which takes the ip address as four byte values, and a port number.
		 * @param ip The array with the IP values.
		 * @param portNr The port number.
		 */
		NetworkAddress(unsigned char ip[4], const int portNr);

		/**
		 * The extended constructor, which takes a string and port number.
		 * @param address The address in form of a string. Examples are "192.168.0.1" or "www.mysticgd.com". 
		 *                In case of a name the IP will automatically be tried to be resolved.
		 * @param portNr The port number to connect to.
		 */
		NetworkAddress(const MCore::String& ipAddress, const int portNr);

		/**
		 * The destructor.
		 */
		~NetworkAddress();

		/**
		 * Get the port number.
		 * @result The port number.
		 */
		int GetPort() const;

		/**
		 * Set the port number.
		 * @param portNr The port number.
		 */
		void SetPort(const int portNr);

		/**
		 * Set the IP address.
		 * @param ipAddress The IP address in form of a string. For example "192.168.0.1". Names are NOT allowed!
		 *                  So it is NOT valid to pass for example "ftp.mysticgd.com" to this.
		 */
		void SetAddress(const String& ipAddress);

		/**
		 * Set the IP address and port number.
		 * @param ipAddress The IP address in form of a string. For example "192.168.0.1". Names are NOT allowed!
		 *                  So it is NOT valid to pass for example "ftp.mysticgd.com" to this.
		 * @param portNr The port number.
		 */
		void SetAddress(const String& ipAddress, const int portNr);

		/**
		 * Get the address.
		 * @result A string containing the ip address, for example "192.168.0.1"
		 */
		String GetAddressAsString() const;
		/**
		 * Get the ip address as integer numbers.
		 * @result The array of ip values.
		 */
		const MCore::Array<unsigned char>& GetAddressAsInts() const;

		/**
		 * Check if this address is valid or not.
		 * An address is always valid, with the exception if we failed to convert an address string (like "www.mysticgd.com") into
		 * an ip address.
		 * @result Returns if this is a valid address or not.
		 */
		inline bool IsValid() const		{ return mIsValid; }


	private:
		MCore::Array<unsigned char>	mAddress;		/**< The IP address values. For the address "192.168.0.1" these would be (192, 168, 0, 1). */
		int							mPort;			/**< The port number. */
		bool						mIsValid;		/**< Is this address a valid address? */
};

#endif // #if PLATFORM == PLATFORM_WINDOWS

}	// namespace MCore

#endif