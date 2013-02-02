/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Exception.h"


namespace MCore
{


Exception::Exception() throw()
{
	mMessage = NULL;
	mSource  = NULL;
	SetMessageAndSource("", "");
}


Exception::Exception(const Exception& exception) throw()
{
	mMessage = NULL;
	mSource  = NULL;
	SetMessageAndSource(exception.mMessage, exception.mSource);
}


Exception::Exception(const char* message) throw()
{
	mMessage = NULL;
	mSource  = NULL;
	SetMessageAndSource(message, "");
}


Exception::Exception(const char* message, const char* source) throw()
{
	mMessage = NULL;
	mSource  = NULL;
	SetMessageAndSource(message, source);
}


Exception::~Exception() throw()
{
	// get rid of the data
	free(mMessage);	mMessage=NULL;
	free(mSource);	mSource =NULL;
}


Exception& Exception::operator=(const Exception& exception) throw()
{
	SetMessageAndSource(exception.mMessage, exception.mSource);
	return *this;
}


const char* Exception::GetMessage() const throw()
{
	return mMessage;
}


const char* Exception::GetSource() const throw()
{
	return mSource;
}


void Exception::SetMessageAndSource(const char* message, const char* source)
{
	mMessage = (char*)realloc(mMessage, strlen(message)+1 );
	strcpy(mMessage, message);
	
	mSource = (char*)realloc(mSource, strlen(source)+1 );
	strcpy(mSource, source);
}


}	// namespace MCore
