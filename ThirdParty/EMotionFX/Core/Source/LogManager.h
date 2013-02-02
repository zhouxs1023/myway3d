/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LOGMANAGER_H
#define __LOGMANAGER_H

#include "Exception.h"
#include "LinkedList.h"
#include "LogFile.h"
#include "Singleton.h"
#include "String.h"


namespace MCore
{

// forward declaration
class LogManager;

/**
 * The log callback interface.
 * Classes inherited from the callback interface can be added to the log manager's stack
 * through which it iterates each time you log something. The Log() method of the callback
 * is called when logging. It can be used to write into a combo box in a mfc environment or into
 * a in-game console.
 */
class LogCallback
{
	public:
		/**
		 * Default constructor.
		 */
		LogCallback(){}

		/**
		 * Destructor.
		 */
		virtual ~LogCallback(){}

		/**
		 * The abstract callback logging method.
		 * @param text The text to log.
		 */
		virtual void Log(const String& text) = 0;
};

/**
 * Concrete implementation of the log callback interface.
 * This log file callback writes the logging information into
 * a log file on hard disk.
 */
class LogFileCallback : public LogCallback
{
	public:
		/**
		 * Constructor.
		 * @param filename The filename of the log file including extension
		 */
		LogFileCallback(const String& filename);

		/**
		 * Destructor.
		 */
		~LogFileCallback();

		/**
		 * The concrete callback logging method.
		 * @param text The text to log.
		 */
		void Log(const String& text);

		/**
		 * Return the log file.
		 * @return A pointer to the log file.
		 */
		LogFile* GetLogFile() const;

	private:
		LogFile* mLog; /**< A pointer to the log file. */
};

/**
 * Logging macros that use the HERE macro from SourceDeclaration.h.
 * Make sure that you are inside a class and that you have declared
 * the class via DECLARE_CLASS, in addition you have to ensure that you
 * have declared your function. You will get a compile error if you do not
 * follow these instructions. If you are in global space or if you do not
 * want to declare your classes you can use the LOG function.
 */
#define LOG_INFORMATION(what) MCore::LogManager::GetInstance().LogMessage(what, LogFile::LOW, HERE);
#define LOG_WARNING(what) MCore::LogManager::GetInstance().LogMessage(what, LogFile::NORMAL, HERE);
#define LOG_ERROR(what) MCore::LogManager::GetInstance().LogMessage(what, LogFile::HIGH, HERE);

/**
 * Generalised logging functions.
 * Use these functions if you are in global space or if you do not
 * want to declare your classes you can use the LOG function.
 */
void LOG(const String& what);
void LOG(const char* what, ...);

/// Singleton instance macro.
#define LOG_MANAGER MCore::LogManager::GetInstance()

/**
 * The log manager handles the creation and retrieval of logs for the
 * application. This class is responsible for managing the log callbacks
 * and for logging. Each time you call the log function the manager will
 * iterate through all log callbacks and call their Log() functions so that you
 * can create and add your own log callbacks to the manager. This can be used to for
 * instance log into your in-game console or to some mfc combo box.
 */
class LogManager
{
	DECLARE_CLASS(LogManager)
	SINGLETON(LogManager)

	private:
		/**
		 * Default constructor.
		 */
		LogManager();

		/**
		 * Destructor.
		 */
		~LogManager();

	public:
		/**
		 * Create a new log callback which creates a log file with the given name and 
		 * add the callback to the stack. This function is a helper so that you dont have to
		 * create callback instances and add them to the manager all the time.
		 * @param filename The filename to give the log e.g. "Core.log".
		 */
		LogFile* CreateLogFile(const String& filename);

		/**
		 * Add the given callback to the stack.
		 * Don't delete the callbacks yourself, the manager will keep
		 * track of the callbacks in the stack.
		 * @param callback The callback to add.
		 */
		void AddLogCallback(LogCallback* callback);

		/**
		 * Remove the given callback from the stack.
		 * @param index The index of the callback to remove.
		 */
		void RemoveLogCallback(const int index);

		/**
		 * Remove all log callbacks from the stack.
		 */
		void ClearLogCallbacks();

		/**
		 * Return a pointer to the given log callback.
		 * @param index The index of the callback.
		 * @return A pointer to the callback.
		 */
		LogCallback* GetLogCallback(const int index);

		/**
		 * Return the number of log callbacks managed by this class.
		 * @return Number of log callbacks.
		 */
		int GetNumLogCallbacks() const;

		/**
		 * Log message. Iterates through all callback intances and log each of them.
		 * @param message The message to write into the log file.
		 * @param logMessageLevel The log message priority.
		 * @param source The message source.
		 */
		void LogMessage(const String& message, const LogFile::ELogMessageLevel& logMessageLevel=LogFile::NORMAL, const String& source="");

	protected:
		Array<LogCallback*>		mLogCallbacks;			/**< A collection of log callback instances. */
};

} // namespace MCore


#endif