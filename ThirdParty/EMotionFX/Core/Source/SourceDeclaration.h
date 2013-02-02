/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SOURCEDECLARATION_H
#define __SOURCEDECLARATION_H

#include "StandardHeaders.h"


namespace MCore
{

// forward declaration
class String;

/**
 * Exposes the current class to the runtime.
 * @param name The class name.
 */
#define DECLARE_CLASS(name) \
	static char* Internal_GetCurrentClass() \
	{ \
		return #name; \
	}

/**
 * Exposes the current function to the runtime.
 * @param name The function name.
 */
#ifdef WIN32
#define DECLARE_FUNCTION(name) \
	static char* Internal_GetCurrentFunction = #name ## "()";
#else //MACOS
#define DECLARE_FUNCTION(name) \
	static char* Internal_GetCurrentFunction = #name "()";
#endif //MACOS

/**
 * Returns the name of the current function (including class name).
 * @result The name of the current function (including class name).
 */
#define THIS_FUNCTION \
	Internal_GetCurrentFunction

/**
 * Returns the name of the current class.
 * @result The name of the current class.
 */
#define THIS_CLASS \
	Internal_GetCurrentClass()

/**
 * Macro that returns the current function name excluding namespace, class name.
 * ::FunctionName().
 */
#define NOCLASS_HERE \
	MCore::String(MCore::String("Global::") + MCore::String(THIS_FUNCTION))

/**
 * Macro that returns the current location including namespace, class name
 * and function name. ClassName::FunctionName().
 */
#ifdef WIN32
#define HERE \
	MCore::String(MCore::String(THIS_CLASS) + MCore::String("::") + MCore::String(THIS_FUNCTION))
#else //MACOS
#define HERE "doesnt work on macos"
#endif //MACOS

/**
 * Macro that returns the current location. Global::.
 */
#define GLOBAL_HERE \
	MCore::String("Global::")

}


#endif