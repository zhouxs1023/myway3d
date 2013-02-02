/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LOGFILE_H
#define __LOGFILE_H

#include "DiskFile.h"
#include "String.h"


namespace MCore
{

/// Incomplete class declaration.
class LogManager;

/**
 * LogFile class for writing debug/log data to files.
 * Should not be used directly, but through the LogManager class.
 */
class LogFile : public DiskFile
{
	DECLARE_CLASS(LogFile)

	public:
		/**
		 * The importance of a logged message.
		 */
		enum ELogMessageLevel
		{
			LOW		= 0x00,	/**< Low logging priority */
			NORMAL	= 0x01, /**< Normal logging priority (default) */
			HIGH	= 0x02	/**< High logging priority */
		};

		/**
		 * Constructor. Called by LogManager.
		 * @param name The filename of the log file.
		 */
		LogFile(const String& name);

		/**
		 * Destructor.
		 */
		~LogFile();

		/**
		 * Logs a message to log file.
		 * @param message The message to write into the log file.
		 * @param logMessageLevel The log message priority.
		 * @param source The message source.
		 */
		void LogMessage(const String& message, const ELogMessageLevel& logMessageLevel=LogFile::NORMAL, const String& source="");

		/// Friend class declaration.
		friend class LogManager;
};

}	// namespace MCore


#endif