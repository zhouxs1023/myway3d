/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MACROS_H
#define __MACROS_H

#include "Platform.h"
#include "StandardHeaders.h"


/**
 * The macro dynamically creates a variable including it's
 * mutator and accessor.
 * @param type The type of the variable.
 * @param variableName The variable name.
 */
#define MAKE_ACCESS(type, variableName) \
	public: \
		void Set##variableName(const type& variableName) \
		{ \
			m##variableName = variableName; \
		} \
		type Get##variableName() const \
		{ \
			return m##variableName; \
		} \
	private: \
		type variableName;
/*
/// Number of elements in an array.
#define ARRAY_COUNT(array) \
	(sizeof(array) / sizeof((array)[0]))

/// Offset of a struct member.
#define STRUCT_OFFSET(struc, member) \
	((INT)&((struc*)NULL)->member)

*/

/*
/// Quoting
//#define _QUOTE(x) #x
//#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

/// For generating compiler warnings - should work on any compiler
#define _QUOTE_INPLACE_(x) #x
#define QUOTE(x) _QUOTE_INPLACE_(x)

#define warn(x) message(__FILE__ "(" QUOTE(__LINE__) ") : " x "\n")

/// File line output
#define NOTE(x) message(x)
#define FILE_LINE warn(__FILE__LINE__)

/// todo macro which outputs a todo as a compiler waring
#define TODO(x) warn("|  TODO :   #x\n")

/// fixme macro which outputs a todo as a compiler waring
//#define FIXME(x) warn(__FILE__LINE__"\n"		\
//						 "|  FIXME :  #x\n"	\
*/

#endif