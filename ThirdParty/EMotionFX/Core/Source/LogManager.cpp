/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "LogManager.h"
#include "LogFile.h"


namespace MCore
{

// constructor
LogFileCallback::LogFileCallback(const String& filename)
{
	mLog = new LogFile(filename);
}


// destructor
LogFileCallback::~LogFileCallback()
{
	if (mLog)
		delete mLog;
}


// log callback function
void LogFileCallback::Log(const String& text)
{
	mLog->LogMessage(text);
}


// return a pointer to the log file
LogFile* LogFileCallback::GetLogFile() const
{
	return mLog;
}




// constructor
LogManager::LogManager()
{

}


// destructor
LogManager::~LogManager()
{
	// get rid of the callbacks
	ClearLogCallbacks();
}



// creates a log file callback and adds it to the stack
LogFile* LogManager::CreateLogFile(const String& filename)
{
	// create log file callback instance
	LogFileCallback* callback = new LogFileCallback(filename);

	// add log file callback to the stack
	AddLogCallback(callback);

	// return pointer to the log file of the callback
	return callback->GetLogFile();
}


// add a log file callback instance
void LogManager::AddLogCallback(LogCallback* callback)
{
	// add the callback to the stack
	if (callback)
		mLogCallbacks.Add(callback);
}


// remove a specific log callback from the stack
void LogManager::RemoveLogCallback(const int index)
{
	// remove the callback from the list
	if (mLogCallbacks.IsValidIndex(index))
	{
		LogCallback* callback = mLogCallbacks[index];

		// get rid of the callback instance
		if (callback)
			delete callback;

		// remove the callback from the stack
		mLogCallbacks.Remove(index);
	}
}


// remove all log callbacks from the stack
void LogManager::ClearLogCallbacks()
{
	// get rid of the callbacks
	for (int i=0; i<mLogCallbacks.GetLength(); i++)
	{
		LogCallback* callback = mLogCallbacks[i];

		if (callback)
			delete callback;
	}
	mLogCallbacks.Clear();
}


// retrieve a pointer to the given log callback
LogCallback* LogManager::GetLogCallback(const int index)
{
	return mLogCallbacks[index];
}


// return number of log callbacks in the stack
int LogManager::GetNumLogCallbacks() const
{
	return mLogCallbacks.GetLength();
}


// the main logging method
void LogManager::LogMessage(const String& message, const LogFile::ELogMessageLevel& logMessageLevel, const String& source)
{
	// iterate through all callbacks
	for (int i=0; i<mLogCallbacks.GetLength(); i++)
	{
		LogCallback* callback = mLogCallbacks[i];

		if (callback)
			callback->Log(message);	
	}
}




// global log function which calls the log manager's log function
void LOG(const String& what)
{
	DECLARE_FUNCTION(LOG)

	LogManager::GetInstance().LogMessage(what, LogFile::NORMAL);
}


// global log function which calls the log manager's log function
void LOG(const char* what, ...)
{
	DECLARE_FUNCTION(LOG)

	char textBuf[4096];
	va_list args;
	va_start(args, what);

	vsprintf(textBuf, what, args);
	va_end(args);

	LogManager::GetInstance().LogMessage(textBuf, LogFile::NORMAL);
}

}