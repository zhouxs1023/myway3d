/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Macros.h"
#include "String.h"
#include "Array.h"
#include "Algorithms.h"
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include "MemoryManager.h"


namespace MCore
{


// destructor
String::~String()
{ 
	MEMMGR.Delete(mData); 
}


// construct this string from two strings
String::String(const char* sA, int lengthA, const char *sB, int lengthB)
{
	assert(sA);
	assert(sB);
	assert(lengthA>=0 && lengthB>=0);

	mData		= NULL;
	mMaxLength	= 0;
	mLength		= 0;

	Alloc(lengthA+lengthB);
	memcpy(mData, sA, lengthA);
	memcpy(mData+lengthA, sB, lengthB);
}


// allocate character buffer
void String::Alloc(int length, int extraChars)		
{ 
	if ((length+extraChars > mMaxLength) || mData==NULL)
	{
		mLength		= length;
		mMaxLength	= mLength+extraChars;

//		mData		= (char*)realloc(mData, mMaxLength + 1);
		mData		= (char*)MEMMGR.ReAlloc(mData, mMaxLength + 1, String::MEMORYCATEGORY, 16, String::MEMORYBLOCK_ID);

		if (mData == NULL) return;

		assert(mData);
		mData[length] = '\0';	
	}
	else
	{
		mLength		= length;
		mData[length] = '\0';	
	}
}


// concatenate two strings
String& String::Concat(const char *what, int length)
{
	assert(mLength>=0);
	assert(length>=0);
	assert(what);
	
	int oldLength = mLength;

	// realloc new
	if (mMaxLength <= length+mLength)
		Alloc(mLength+length);
	else
		mLength += length;

	memcpy(mData+oldLength, what, length);
	mData[mLength] = '\0';
	
	return *this;
}


// copy some data
String& String::Copy(const char *what, int length)
{
	if (what == NULL)
	{
		MEMMGR.Delete(mData);
		mData=NULL;
		return *this;
	}

	assert(length>=0);

	// remove existing data
	MEMMGR.Delete(mData); 
	mData=NULL;
	
	// alloc new
	Alloc(length);

	// copy new data
	if (mData)
		memcpy(mData, what, length);

	return *this;
}



// returns the number of words inside this string
int String::CalcNumWords() const
{
	int numWords= 0;
	int curPos	= 0;

	// read away all spaces in the beginning
	while ((mData[curPos]==' ' || mData[curPos]=='\n' || mData[curPos]=='\t') && mData[curPos]!='\0') curPos++;

	// count the number of words
	while (mData[curPos] != '\0')
	{
		// move cursor to next space
		while (mData[curPos]!=' ' && mData[curPos]!='\0' && mData[curPos]!='\n' && mData[curPos]!='\t') curPos++;

		numWords++;

		// move cursor to next word
		while ((mData[curPos]==' ' || mData[curPos]=='\n' || mData[curPos]=='\t') && mData[curPos]!='\0') curPos++;
	}

	return numWords;
}


// returns word number <wordNr>
String String::ExtractWord(int wordNr) const
{
	assert(wordNr>=0 && wordNr<CalcNumWords());	// slow in debugmode, but more safe

	String result;
	int numWords = 0;
	int curPos = 0;

	// read away all spaces in the beginning
	while ((mData[curPos]==' ' || mData[curPos]=='\n' || mData[curPos]=='\t') && mData[curPos]!='\0') curPos++;

	// count the number of words
	while (mData[curPos] != '\0')
	{
		// move cursor to next space
		while (mData[curPos]!=' ' && mData[curPos]!='\0' && mData[curPos]!='\n' && mData[curPos]!='\t')
		{
			if (numWords == wordNr)
				result += mData[curPos];

			curPos++;
		}

		if (numWords == wordNr)
			return result;

		numWords++;

		// move cursor to next word
		while ((mData[curPos]==' ' || mData[curPos]=='\n' || mData[curPos]=='\t') && mData[curPos]!='\0') curPos++;
	}

	return result;
}



// find a given substring, returns the position, or -1 when not found
int String::Find(const String& subString) const
{
	char* strResult = strstr(mData, subString.GetReadPtr());
	
	// if the string has not been found, return -1
	if (strResult==NULL) 
		return -1;
	
	// else return the position
	return strResult - mData;
}


// removes a given part from the string (the first found one)
bool String::RemoveFirstPart(const String& part)
{
	int pos = Find(part);

	// substring not found, so exit
	if (pos == -1) return false;

	// remove the part
	memmove(mData+pos, mData+pos+part.GetLength(), mLength-(pos+part.GetLength()) );

	// resize the memory amount
	Alloc(mLength - part.GetLength());

	return true;
}



// removes all given parts from a string (all occurences)
bool String::RemoveAllParts(const String& part)
{
	bool result = false;

	while (RemoveFirstPart(part)) 
		result = true;

	return result;
}



// removes a given set of characters from the string
bool String::RemoveChars(Array<char> &charSet)
{
	bool result = false;

	// for all characters in the set
	for (int i=0; i<charSet.GetLength(); i++)
	{
		if (RemoveAllParts(charSet[i]))
			result = true;
	}

	return result;
}



// removes a given set of characters from the string
bool String::RemoveChars(const String& charSet)
{
	bool result = false;

	// for all characters in the set
	for (int i=0; i<charSet.GetLength(); i++)
	{
		if (RemoveAllParts(charSet[i]))
			result = true;
	}

	return result;
}



// returns an uppcase version of this string
String String::Uppered() const
{
	String newString( *this );
	newString.ToUpper();
	return newString;
}


// returns a lowercase version of this string
String String::Lowered() const
{
	String newString( *this );
	newString.ToLower();
	return newString;
}


// uppercase this string
const String& String::ToUpper()
{
	strupr(mData);
	return *this;
}


// lowercase this string
const String& String::ToLower()
{
	strlwr(mData);
	return *this;
}


// returns true if this string is equal to the given other string (case sensitive)
bool String::IsEqual(const String& other) const
{
	// if the length of the strings isn't equal, they are not equal ofcourse
	if (mLength != other.GetLength()) return false;

	// compare
	return (Compare(other) == 0);
}


// returns true if this string is equal to the given other string (not case sensitive)
bool String::IsEqualNoCase(const String& other) const
{
	// length not equal
	if (mLength != other.GetLength()) return false;

	// compare
	return (CompareNoCase(other) == 0);
}



// compares two strings (case sensitive) returns 0 when equal, -1 when this string is bigger and 1 when other is bigger
int String::Compare(const String& other) const
{
	return strcmp(mData, other.GetReadPtr());
}


// compares two strings (non case sensitive) returns 0 when equal, -1 when this string is bigger and 1 when other is bigger
int String::CompareNoCase(const String& other) const
{
	#ifdef _WIN32
		return stricmp(mData, other.GetReadPtr());
	#else
		return strcasecmp(mData, other.GetReadPtr());
	#endif
}



// remove all given trimChars on the left of the string
void String::TrimLeft(char trimChar)
{
	String removeString;

	for (int i=0; i<mLength; i++)
	{
		// return when another character found
		if (mData[i] != trimChar)
		{
			if (i != 0)
				RemoveFirstPart(removeString);

			return;
		}

		removeString += trimChar;
	}

	// in case we have for example a string of only spaces
	Clear();
}



// remove all given trimChars on the right of the string
void String::TrimRight(char trimChar)
{
	for (int i=mLength-1; i>=0; i--)
	{
		// return when another character found
		if (mData[i] != trimChar)
		{
			// remove the last chars
			if (i != mLength-1)				
				Alloc(i+1);

			return;
		}
	}

	// seems like we have a string of only spaces for example
	Clear();
}



// remove all given trimChars on both left and right side of the string
void String::Trim(char trimChar)
{
	TrimLeft(trimChar);
	TrimRight(trimChar);
}




// format a string, returns itself
String& String::Format(const char *text, ...)
{
	Alloc(2048);

	va_list args;
	va_start(args, text);
	int len = vsprintf(mData, text, args);
	assert(len >= 0);
	va_end(args);

	Alloc(len);

	return *this;
}



// add a formatted string to the current string
String& String::FormatAdd(const char *text, ...)
{
	mData = (char*)MEMMGR.ReAlloc(mData, mLength+2048+1, String::MEMORYCATEGORY, 16, String::MEMORYBLOCK_ID);

	va_list args;
	va_start(args, text);
	int len = vsprintf(mData+mLength, text, args);
	assert(len >= 0);
	va_end(args);

	Alloc(mLength + len);

	return *this;
}



// convert the string to an int
int String::ToInt() const
{
	return atoi(mData);
}



// convert the string to a float
float String::ToFloat() const
{
	return atof(mData);
}



// split the string into substrings. "a;b;c" with splitchar ';' would result in an array of 3 strings ("a", "b", "c")
Array< String > String::Split(const char splitChar) const
{
	Array<String> result;
	if (mData == NULL) return result;

	String cuString;

	int		i = 0;
	char	curChar;

	for(;;)
	{
		curChar = mData[i];
		
		if (curChar == '\0')
		{
			if (cuString.GetLength() > 0)
			{
				result.Add(cuString);
			}

			break;
		}

		if (curChar == splitChar)
		{
			result.Add(cuString);
			cuString.Clear();
		}
		else
		{
			cuString += curChar;
		}

		i++;
	}

	if (result.GetLength() == 0)
		if (Find(splitChar) != -1)
			result.Add(*this);

	return result;
}



// convert the string to a boolean. string may be "1", "0", "true", "false") (non case sensitive)
bool String::ToBool() const
{
	// trim away all possible spaces on the left and right side
	//Trim(' ');	

	// if we have only 1 character left it must be a 0 or 1
	if (GetLength() == 1)
	{
		int value = ToInt();
		if (value == 0) return false;
		if (value == 1) return true;
		return false;
	}

	// check if it's true, else return false
	return IsEqualNoCase("true");
}



// calculate the CRC32
unsigned int String::CalcCRC32() const
{
	if (mData == NULL) return 0;

	// calculate the CRC
	unsigned int CRC = 0xFFFFFFFF;

	int pos = 0;
	char curChar = mData[pos];
	while (curChar != '\0')
	{	
		CRC = ((CRC) >> 8) ^ MCore::CRC32Table[curChar ^ ((CRC) & 0x000000FF)];
		pos++;
		curChar = mData[pos];
	}

	CRC = ~CRC;

	// return the generated CRC code
	return CRC;
}


bool String::IsValidFloat() const
{
	char validChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', 'e', '+', '-' };

	// check if all characters are valid
	for (int i=0; i<mLength; i++)
	{
		char c = mData[i];

		// if the + or - is not the first character
		if (c == '+' || c =='-')
			if (i != 0)
				return false;

		// check if the characters are correct values
		bool found = false;
		for (int a=0; a<14 && !found; a++)
		{
			if (c == validChars[a])
				found = true;
		}

		// if we found a non-valid character, we cannot convert this to a float
		if (!found)
			return false;
	}

	return true;
}


bool String::IsValidInt() const
{
	char validChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-' };

	// check if all characters are valid
	for (int i=0; i<mLength; i++)
	{
		char c = mData[i];

		// if the + or - is not the first character
		if (c == '+' || c =='-')
			if (i != 0)
				return false;

		// check if the characters are correct values
		bool found = false;
		for (int a=0; a<12 && !found; a++)
		{
			if (c == validChars[a])
				found = true;
		}

		// if we found a non-valid character, we cannot convert this to an int
		if (!found)
			return false;
	}

	return true;
}


// encrypt the string
String& String::Encrypt(const String& key)
{
	const int length = mLength;
	for (int i=0; i<length; i++)
	{
		int keyIndex = i % key.GetLength();
		mData[i] = mData[i] ^ key[keyIndex];
	}

	return *this;
}


// decrypt the string
String& String::Decrypt(const String& key)
{
	// encrypting it again with the same key will invert (decrypt) the encryption
	return Encrypt(key);
}


// return an encrypted version
String String::Encrypted(const String& key) const
{
	String result(*this);
	result.Encrypt(key);
	return result;
}


// return a decrypted version
String String::Decrypted(const String& key) const
{
	String result(*this);
	result.Decrypt(key);
	return result;
}


}	// namespace MCore
