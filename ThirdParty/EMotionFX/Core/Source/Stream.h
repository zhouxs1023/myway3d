/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __STREAM_H
#define __STREAM_H

#include "SourceDeclaration.h"
#include "String.h"
#include "Exception.h"

namespace MCore
{

/**
 * The stream abstract base class.
 * A stream is a source from which can be read and to which can be written.
 * This could be a file, a TCP/IP stream, or anything else you can imagine.
 */
class Stream
{
	DECLARE_CLASS(Stream)

	public:
		/**
		 * The constructor.
		 */
		Stream() {}

		/**
		 * The destructor.
		 */
		virtual ~Stream() {}

		/**
		 * Read a given amount of data from the stream.
		 * @param data The pointer where to store the read data.
		 * @param length The size in bytes of the data to read.
		 * @result Returns the number of bytes read.
		 */
		virtual int Read(void *data, int length)			{ assert(1==0); return true; }	// should never be called, since this method should be overloaded somewhere else

		/**
		 * Writes a given amount of data to the stream.
		 * @param data The pointer to the data to write.
		 * @param length The size in bytes of the data to write.
		 * @result Returns the number of written bytes.
		 */
		virtual int Write(void *data, int length)			{ assert(1==0); return true; }	// should never be called, since this method should be overloaded somewhere else

		// write operators
		virtual Stream& operator<<(bool b)					{ Write(&b, sizeof(bool)); return *this; }
		virtual Stream& operator<<(signed char ch)			{ Write(&ch, sizeof(signed char)); return *this; }
		virtual Stream& operator<<(unsigned char ch)		{ Write(&ch, sizeof(unsigned char)); return *this; }
		virtual Stream& operator<<(signed short number)		{ Write(&number, sizeof(signed short)); return *this; }
		virtual Stream& operator<<(unsigned short number)	{ Write(&number, sizeof(unsigned short)); return *this; }
		virtual Stream& operator<<(signed int number)		{ Write(&number, sizeof(signed int)); return *this; }
		virtual Stream& operator<<(unsigned int number)		{ Write(&number, sizeof(unsigned int)); return *this; }
		virtual Stream& operator<<(signed long number)		{ Write(&number, sizeof(signed long)); return *this; }
		virtual Stream& operator<<(unsigned long number)	{ Write(&number, sizeof(unsigned long)); return *this; }
		virtual Stream& operator<<(float number)			{ Write(&number, sizeof(float)); return *this; }
		virtual Stream& operator<<(double number)			{ Write(&number, sizeof(double)); return *this; }
		virtual Stream& operator<<(const String& text)		{ Write((void*)text.GetReadPtr(), text.GetLength()+1); return *this; }	// +1 to include the '\0'
		virtual Stream& operator<<(const char *text)		{ Write((void*)text, (int)strlen(text)); char c='\0'; Write(&c, 1); return *this; }

		// read operators
		virtual Stream& operator>>(bool& b)					{ Read(&b, sizeof(bool));  return *this; }
		virtual Stream& operator>>(signed char& ch)			{ Read(&ch, sizeof(signed char)); return *this; }
		virtual Stream& operator>>(unsigned char& ch)		{ Read(&ch, sizeof(unsigned char)); return *this; }
		virtual Stream& operator>>(signed short& number)	{ Read(&number, sizeof(signed short)); return *this; }
		virtual Stream& operator>>(unsigned short& number)  { Read(&number, sizeof(unsigned short)); return *this; }
		virtual Stream& operator>>(signed int& number)		{ Read(&number, sizeof(signed int)); return *this; }
		virtual Stream& operator>>(unsigned int& number)	{ Read(&number, sizeof(unsigned int)); return *this; }
		virtual Stream& operator>>(signed long& number)		{ Read(&number, sizeof(signed long)); return *this; }
		virtual Stream& operator>>(unsigned long& number)	{ Read(&number, sizeof(unsigned long)); return *this; }
		virtual Stream& operator>>(float& number)			{ Read(&number, sizeof(float)); return *this; }
		virtual Stream& operator>>(double& number)			{ Read(&number, sizeof(double)); return *this; }
		virtual Stream& operator>>(String& text)			
		{ 
			DECLARE_FUNCTION( operator>>(String &text) )
			char c;			

			for (;;) 
			{
				// check if we can read the character
				if (!Read(&c, 1))
				{
					throw Exception("String cannot be read from stream, something went wrong while reading.\nMight be because we're reading past the end of the stream or something...", HERE);
				}

				// add the character or quit
				if (c != '\0')
					text += c;
				else
					break;
			}

			return *this;
		}
};

} // namespace MCore

#endif