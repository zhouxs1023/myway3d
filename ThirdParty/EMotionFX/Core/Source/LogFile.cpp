/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "LogFile.h"


namespace MCore
{

LogFile::LogFile(const String& name)
{
	Open(name.GetReadPtr(), WRITE, false);
/*
	String logHeader("Log: (here comes the date)\nMysticGD - http://www.mysticgd.com\nVersion: 0.01a\n=====================================\n\n");

	if (IsOpen())
	{
		Write((void*)logHeader.GetReadPtr(), logHeader.GetLength());
		Flush();
	}*/
}

LogFile::~LogFile()
{
	Close();
}

void LogFile::LogMessage(const String& message, const ELogMessageLevel& logMessageLevel, const String& source)
{
	String buffer;

	buffer.Format("%s", (logMessageLevel==HIGH) ? "HIGH: " : (logMessageLevel==NORMAL) ? "" : "");
	buffer += message + '\n';

#if PLATFORM == PLATFORM_WINDOWS
	OutputDebugString(buffer.GetReadPtr());
#endif

	if (IsOpen())
	{
		Write((void*)buffer.GetReadPtr(), buffer.GetLength());
		Flush();
	}
/*
	// Write time into log
	//struct tm *pTime;
	//time_t ctTime; time(&ctTime);
	//pTime = gmtime( &ctTime );
	if(logMessageLevel == Log::HIGH)
		*this<<"HIGH: ";

	*this<<source;
	*this<<": ";
	*this<<message;*/
}

}	// namespace MCore