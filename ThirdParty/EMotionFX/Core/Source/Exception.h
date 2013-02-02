/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include "StandardHeaders.h"
#include "SourceDeclaration.h"


namespace MCore
{

/**
 * The exception base class.
 * When thrown, it provides information about the error occured.
 * Functions should be implemented within a try{} catch(Exception& e) {} block.
 */
class Exception
{
	public:
		/**
		 * Default constructor.
		 */
		Exception() throw();

		/**
		 * Copy constructor.
		 * @see operator=
		 * @param exception The exception to copy.
		 */
		Exception(const Exception& exception) throw();

		/**
		 * Constructor.
		 * Initializes the exception with the passed error message.
		 * @param message The error message.
		 */
		Exception(const char* message) throw();

		/**
		 * Constructor.
		 * Initializes the exception with the passed error message and error source.
		 * @param message The error message.
		 * @param source The error source.
		 */
		Exception(const char* message, const char* source) throw();

		/**
		 * Destructor.
		 */
		virtual ~Exception() throw();

		/**
		 * Assignment operator.
		 * @see Exception(const Exception& exception)
		 * @param exception The exception to assign.
		 * @result A reference to *this which allows concatenation.
		 */
		Exception& operator=(const Exception& exception) throw();

		/**
		 * Accessor returns a string containing the error message.
		 * @result A string containing the error message.
		 */
		virtual const char* GetMessage() const throw();

		/**
		 * Accessor returns a string containing the error source.
		 * @result A string containing the error source.
		 */
		virtual const char* GetSource() const throw();


	protected:
		char* mMessage;	/*< The error message. */
		char* mSource;	/*< The source where the exception was thrown, most likely the source code file name. */

		/**
		 * Set the member message and source strings.
		 * @param message The message member value.
		 * @param source The source member value.
		 */
		void SetMessageAndSource(const char* message, const char* source);
};


}	// namespace MCore


#endif