/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SERIALIZABLE_H
#define __SERIALIZABLE_H

#include "StandardHeaders.h"
#include "SourceDeclaration.h"
#include "Stream.h"


namespace MCore
{

/**
 * The serializable base class.
 */
class Serializable
{
	public:
		/**
		 * Default constructor.
		 */
		Serializable() {}

		/**
		 * Destructor.
		 */
		virtual ~Serializable() {}

        /**
		* Serializes the object into the stream.
		* @param out The stream into the object should be serialized.
		*/
		virtual void Serialize(Stream& outputStream) const = 0;
};


}	// namespace MCore


#endif