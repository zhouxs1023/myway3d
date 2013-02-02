/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __STRING_H
#define __STRING_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include <assert.h>
#include "Array.h"
#include <string.h>


namespace MCore
{

/**
 * The string class.
 * Basically a dynamic array of characters with a set of specific operations which can be performed on it.
 * Definitely something you will use very often in development. It makes the life of a coder a lot more easy.
 */
class String
{
	DECLARE_CLASS(String);

	public:
		/**
		 * The memory block ID, used inside the memory manager.
		 * This will make all strings remain in the same memory blocks, which is more efficient in a lot of cases.
		 */
		enum { MEMORYBLOCK_ID = 1 };

		/**
		 * The memory category for strings.
		 */
		enum { MEMORYCATEGORY = 1 };

		/**
		 * Default constructor. No data is being allocated here, even not for the '\0';
		 */
		String()																	: mData(NULL), mLength(0), mMaxLength(0)	{ Alloc(0); }

		/**
		 * Creates a string from a single character.
		 * @param c The character to put in the string.
		 */
		String(char c)																: mData(NULL), mLength(0), mMaxLength(0)	{ Alloc(1); if (mData) mData[0]=c; }

		/**
		 * Creates a string from a buffer with characters.
		 * @param text The buffer to put in the string. Beware that this buffer must end on '\0'.
		 */
		String(const char *text)													: mData(NULL), mLength(0), mMaxLength(0)	{ Alloc((int)strlen(text)); if (mData) memcpy(mData, text, mLength); }

		/**
		 * Copy constructor. Creates a string from another string.
		 * @param other The string to copy the data from.
		 */
		String(const String& other)													: mData(NULL), mLength(0), mMaxLength(0)	{ if (other.mData==NULL) return; Alloc(other.GetLength()); if (mData) memcpy(mData, other.mData, other.GetLength()+1); }

		/**
		 * Creates a string from two character buffers.
		 * @param sA The first buffer with characters.
		 * @param lenghtA The length of the first buffer. So the number of characters in it.
		 * @param sB The second buffer with characters.
		 * @param lengthB The length of the second buffer. So the number of characters in it.
		 */
		String(const char* sA, int lengthA, const char *sB, int lengthB);

		/**
		 * Creates a string from a given integer.
		 * @param value The integer to convert into a string.
		 */
		explicit String(int value)													: mData(NULL), mLength(0), mMaxLength(0)	{ Alloc(32); sprintf(mData, "%d", value); Alloc((int)strlen(mData)); }

		/**
		 * Creates a string from a given float.
		 * @param value The float to convert into a string.
		 */
		explicit String(float value)												: mData(NULL), mLength(0), mMaxLength(0)	{ Alloc(32); sprintf(mData, "%f", value); Alloc((int)strlen(mData)); }

		/**
		 * Creates a string from a given double.
		 * @param value The double to convert into a string.
		 */
		explicit String(double value)												: mData(NULL), mLength(0), mMaxLength(0)	{ Alloc(32); sprintf(mData, "%f", value); Alloc((int)strlen(mData)); }

		/**
		 * Destructor. Gets rid of all allocations made by this String.
		 */
		virtual ~String();

		/**
		 * Returns the length of the string. So the number of characters inside the string, excluding the '\0'
		 * So for example the string containing "Mystic" would return 6.
		 * @result The number of characters inside the string, excluding the '\0' at the end.
		 */
		inline int GetLength() const												{ return mLength; }

		/**
		 * Returns the length of the string. So the number of characters inside the string, excluding the '\0'
		 * So for example the string containing "Mystic" would return 6.
		 * @result The number of characters inside the string, excluding the '\0' at the end.
		 */
		inline int Length() const													{ return mLength; }

		/**
		 * Check if the string is empty or not.
		 * @result Returns true if the string contains no characters at all. So if it's empty. Otherwise false will be returned.
		 */
		inline bool IsEmpty() const													{ return GetLength() == 0; }

		/**
		 * Returns the maximum length of the string character buffer. Note that this is NOT the actual number of characters inside the string.
		 * A string can pre-allocate memory to store additional characters. This will reduce the number of allocations or re-allocations being made
		 * which increases the performance on strings which are highly dynamic.
		 * @result The maximum length of the character buffer excluding the '\0'. If you want to know how many characters the string contains do not use this method, but use the method GetLength()
		 */
		inline int GetMaxLength() const												{ return mMaxLength; }

		/**
		 * Get the const char* version of this string. So a pointer to the actual buffer containing the character data.
		 * @result The const pointer to the buffer containing the character data. This buffer is terminated with a '\0', but can also be NULL!
		 */
		inline const char* GetReadPtr() const										{ return (const char*)mData; }	

		/**
		 * Get the const char* version of this string. So a pointer to the actual buffer containing the character data.
		 * @result The const pointer to the buffer containing the character data. This buffer is terminated with a '\0', but can also be NULL!
		 */
		inline const char* AsChar() const											{ return (const char*)mData; }	

		/**
		 * Get the char* version of this string. So a pointer to the actual buffer containing the character data.
		 * @result The pointer to the buffer containing the character data. This buffer is terminated with a '\0', but can also be NULL!
		 */
		inline char* GetPtr()														{ return mData; }

		/**
		 * Clears all the character data out of the string. This can be done on two ways: by keeping the memory but making the length of the string 0, or deleting all memory of the string as well.
		 * @param keepMemory If this parameter is set to 'true', the memory won't be released. However, when 'false' is specified, all allocated memory will be released and the methods GetReadPtr() and GetPtr() will return NULL.
		 */
		inline void	Clear(bool keepMemory=true)										{ if (keepMemory) { mLength=0; mData[0]='\0'; } else { free(mData); mData=NULL; mLength=0; mMaxLength=0; } }

		/**
		 * Checks if this string contains a given substring. The search is case sensitive!
		 * @param subString The substring you want to look for inside this string.
		 * @result Returns true when the substring is located inside the string, otherwise false us returned.
		 */
		inline bool	Contains(const String& subString)								{ return (Find(subString) != -1); }

		/**
		 * Calculates the number of words inside this string. Both spaces and tabs can be the seperators.
		 * @result The number of words this string contains.
		 */
		int	CalcNumWords() const;

		/**
		 * Extracts a given word number.
		 * @param wordNr The word number to extract. This must be in range of [0..CalcNumWords()-1]
		 * @result Returns the extracted word.
		 */
		String ExtractWord(int wordNr) const;

		/**
		 * Removes the first occurrence of a given substring. This can be used if you want to remove for example "super" from "Very supercool thing". 
		 * In that case, the remaining string after executing this method, would be "Very cool thing".
		 * @param part The part of the string you want to remove. Note that it will try to remove only the first occurrence of this substring.
		 * @result Returns true when successfully removed the part from the string, otherwise returns false (will happen when the given substring wasn't found, so nothing could be removed).
		 */
		bool RemoveFirstPart(const String& part);

		/**
		 * Removes all occurring substrings from the string. This works the same as the method RemoveFirstPart, but with the difference that it will not
		 * only remove the first occurrence, but all occurrences. Implemented as iterative algorithm, which keeps trying to remove the given substring until
		 * no occurrences are found anymore.
		 * For an example:
		 * @see RemoveFirstPart
		 * @param part The substring you want to remove. Note, that ALL occurrences will be removed!
		 * @result Returns true when all substrings have been removed, otherwise false is returned. This will happen when the given substring cannot be found in the string.
		 */
		bool RemoveAllParts(const String& part);

		/**
		 * Removes a given set of characters from the string.
		 * Example: a charSet containing the characters 'y', 'c' and 'e' performed on the string "Mystic Game Development" will result in "Msti Gam Dvlopmnt"
		 * @param charSet the set of characters to remove from the string. All occurrences will be removed.
		 * @result Returns true when successfully performed the opertion, otherwise false is returned. This happens when none of the characters in the set could be found.
		 */
		bool RemoveChars(Array<char> &charSet);

		/**
		 * Removes a given set of characters from the string.
		 * Example: a charSet being "yce" performed on the string "Mystic Game Development" will result in "Msti Gam Dvlopmnt"
		 * @param charSet The string containing a set of characters which will be removed from the string. So every character in charSet will be removed from this string.
		 * @result Returns true when successfully performed the opertion, otherwise false is returned. This happens when none of the characters in the set could be found.
		 */
		bool RemoveChars(const String& charSet);

		/**
		 * Find a substring inside the current string and return it's position (starting at 0). The search is case sensitive!
		 * Example: Searching for "Game" in "Mystic Game Development" will result in a return value of 7, because M=0 +"ystic "=6, which means
		 * that the G of game starts at position 7.
		 * @param subString The substring to search for.
		 * @result Returns the offset in the string (starting at 0) of the subString inside the string. If the subString cannot be found, -1 is returned.
		 */
		int	Find(const String& subString) const;

		/**
		 * Returns an uppercase version of this string.
		 * Example: performing this operation on the string "Mystic Game Development" would return a string containing "MYSTIC GAME DEVELOPMENT".
		 * @result The uppercase version of the string.
		 */
		String Uppered() const;

		/**
		 * Returns a lowercase version of this string.
		 * Example: performing this operation on the string "Mystic Game Development" would return a string containing "mystic game development".
		 * @result The lowercase version of the string.
		 */
		String Lowered() const;

		/**
		 * Convert this string to uppercase.
		 * @result The same string, but now in uppercase.
		 */
		const String& ToUpper();

		/**
		 * Convert this string to lowercase.
		 * @result The same string, but now in lowercase.
		 */
		const String& ToLower();

		/**
		 * Checks if this string is exactly the same as a given other string. The check is case sensitive!
		 * @param other The string to compare with.
		 * @result Returns true when both strings are identical, otherwise false is returned.
		 */
		bool IsEqual(const String& other) const;

		/**
		 * Checks if this string is the same as a given other string. The check is NOT case sensitive.
		 * @param other The string to compare with.
		 * @result Returns true when the lowercase version of both strings are identical, otherwise false is returned.
		 */
		bool IsEqualNoCase(const String& other) const;

		/**
		 * Compare two strings on a case sensitive way. This method works the same way as strcmp of standard C.
		 * @param other The other string to compare with.
		 * @result This method will return 0 when both strings are equal, -1 when this string is less than the other string, and +1 when this string is greater than the other string. With less and greater we mean the alphabetic order.
		 */
		int Compare(const String& other) const;

		/**
		 * Compare two strings on a NON case sensitive way. This method works the same way as strcmp of standard C. But then NON case sensitive!
		 * @param other The other string to compare with.
		 * @result This method will return 0 when both strings are equal, -1 when this string is less than the other string, and +1 when this string is greater than the other string. With less and greater we mean the alphabetic order.
		 */
		int	CompareNoCase(const String& other) const;

		/**
		 * Removes all the first spaces (unless another character is specified) on the left of the string.
		 * Example: Performing this operation on the string "     Mystic     " would result in the string "Mystic     ".
		 * @param trimChar The character to trim. The default is an empty space.
		 */
		void TrimLeft(char trimChar=' ');

		/**
		 * Removes all the first spaces (unless another character is specified) on the right of the string.
		 * Example: Performing this operation on the string "     Mystic     " would result in the string "     Mystic".
		 * @param trimChar The character to trim. The default is an empty space.
		 */
		void TrimRight(char trimChar=' ');

		/**
		 * Removes all the first spaces (unless another character is specified) on both the left and right side of the string.
		 * Example: Performing this operation on the string "     Mystic     " would result in the string "Mystic".
		 * @param trimChar The character to trim. The default is an empty space.
		 */
		void Trim(char trimChar=' ');

		/**
		 * Fills the string on the way printf works.
		 * Example: String text; text.Format("And the winning number is: %d !!!", winningNumber);
		 * @param text The printf style arguments.
		 * @result The current string, containing the text you specified.
		 */
		String& Format(const char *text, ...);

		/**
		 * Adds a given formatted string to the current string data. The parameters of this method work exactly like printf.
		 * Example: String text("The winning number "); text.FormatAdd("is: %d", winningNumber);
		 * @param text The printf style arguments.
		 * @result The current string, with the specified text added to it.
		 */
		String&	FormatAdd(const char *text, ...);

		/**
		 * Convert the string to an integer.
		 * Can return weird values when the string cannot be converted correctly!
		 * @result The integer value.
		 */
		int	ToInt() const;

		/**
		 * Convert the string to a float.
		 * Can return weird values when the string cannot be converted correctly!
		 * @result The float value.
		 */
		float ToFloat() const;

		/**
		 * Convert the string to a boolean.
		 * In case the text in the string cannot be converted correctly, false will be the returned value.
		 * Valid strings can be any of the following (non case sensitive): "0", "1", "true", "false"
		 * @result The boolean value.
		 */
		bool ToBool() const;

		/**
		 * Check if the string can be converted to a valid integer or not.
		 * The only requirement is that the string has no empty spaces in it, so the string must be trimmed before conversion.
		 * @result Returns true when the string can be converted to an integer, otherwise false is returned.
		 */
		bool IsValidInt() const;

		/**
		 * Check if the string can be converted to a valid float or not.
		 * The only requirement is that the string has no empty spaces in it, so the string must be trimmed before conversion.
		 * @result Returns true when the string can be converted to a float, otherwise false is returned.
		 */
		bool IsValidFloat() const;

		/**
		 * Split the string in seperated parts. The splitting will be done on a given character. The splitting character itself will not be included in any of the resulting parts.
		 * Example: String s("Mystic;Game;Development"); s.Split(';'); will result in an array of 3 strings, being: "Mystic", "Game", "Development"
		 * @param splitChar The character to split on.
		 * @result An array with strings containing the seperated parts. This will be an empty array in case no splitting has been performed (so an array where Array::GetLength() returns 0).
		 */
		Array<String> Split(const char splitChar=';') const;

		/**
		 * Generates a CRC32 value from string.
		 * @result The CRC32 of this string.
		 */
		unsigned int CalcCRC32() const;

		/**
		 * Encrypt the string with a given key.
		 * The length of the string will not be changed. So an unencrypted string of 4 characters will result
		 * in a encrypted string of 4 characters as well.
		 * @param key The encryption key.
		 * @result The encrypted string.
		 */
		String& Encrypt(const String& key);

		/**
		 * Decrypt a string that has been encrypted with the Encrypt function before.
		 * @param key The key that was used to encrypt the string.
		 * @result The decrypted string.
		 */
		String& Decrypt(const String& key);

		/**
		 * Return an encrypted version of this string.
		 * The length of the string will not be changed. So an unencrypted string of 4 characters will result
		 * in a encrypted string of 4 characters as well.
		 * @param key The encryption key.
		 * @param The encrypted version of this string.
		 */
		String Encrypted(const String& key) const;

		/**
		 * Return a decrypted version of this string, which has been encrypted with the Encrypt method.
		 * @param key The key that was used to encrypt the string.
		 * @result The decrypted version of this string.
		 */
		String Decrypted(const String& key) const;


		// conversion operators
		inline operator char*()														{ return mData; }
		inline operator const char*() const											{ return ((const char*)mData); }

		// element access operators
		inline char  operator[](const int nr)										{ return mData[nr]; }
		inline char* operator()(const int nr)										{ return (char*)(mData + nr); }
		inline const char operator[](const int nr) const							{ return mData[nr]; }

		// copy operators
		inline const String&	operator=(const String& str)						{ return Copy(str.GetReadPtr(), str.GetLength()); }
		inline const String&	operator=(char c)									{ return Copy(&c, 1); }
		inline const String&	operator=(const char* str)							{ return Copy(str, (int)strlen(str)); }

		// append operators
		inline String	operator+(const char *str)									{ String s(*this); return s.Concat(str, (int)strlen(str)); }
		inline String	operator+(const String& str)								{ String s(*this); return s.Concat(str.GetReadPtr(), str.GetLength()); }
		inline String	operator+(int val)											{ String s(*this); String str(val); return s.Concat(str.GetReadPtr(), str.GetLength()); }
		inline String	operator+(float val)										{ String s(*this); String str(val); return s.Concat(str.GetReadPtr(), str.GetLength()); }
		inline String	operator+(double val)										{ String s(*this); String str(val); return s.Concat(str.GetReadPtr(), str.GetLength()); }
		inline String	operator+(char c)											{ String s(*this); return s.Concat(&c, 1); }

		inline const String& operator+=(const char *str)							{ return Concat(str, (int)strlen(str)); }
		inline const String& operator+=(const String &str)							{ return Concat(str.GetReadPtr(), str.GetLength()); }
		inline const String& operator+=(char c)										{ return Concat(&c, 1); }
		inline const String& operator+=(int val)									{ String str(val); return Concat(str.GetReadPtr(), str.GetLength()); }
		inline const String& operator+=(float val)									{ String str(val); return Concat(str.GetReadPtr(), str.GetLength()); }
		inline const String& operator+=(double val)									{ String str(val); return Concat(str.GetReadPtr(), str.GetLength()); }

		// compare operators
		inline bool	operator< (const String&	str)								{ return (strcmp(mData, str.GetReadPtr())	<  0); }
		inline bool	operator< (const char*		str)								{ return (strcmp(mData, str)				<  0); }
		inline bool	operator> (const String&	str)								{ return (strcmp(mData, str.GetReadPtr())	>  0); }
		inline bool	operator> (const char*		str)								{ return (strcmp(mData, str)				>  0); }
		inline bool	operator<=(const String&	str)								{ return (strcmp(mData, str.GetReadPtr())	<= 0); }
		inline bool	operator<=(const char*		str)								{ return (strcmp(mData, str)				<= 0); }
		inline bool	operator>=(const String&	str)								{ return (strcmp(mData, str.GetReadPtr())	>= 0); }
		inline bool	operator>=(const char*		str)								{ return (strcmp(mData, str)				>= 0); }
		inline bool	operator==(const String&	str)								{ return (strcmp(mData, str.GetReadPtr())	== 0); }
		inline bool	operator==(const char*		str)								{ return (strcmp(mData, str)				== 0); }
		inline bool	operator!=(const String&	str)								{ return (strcmp(mData, str.GetReadPtr())	!= 0); }
		inline bool	operator!=(const char*		str)								{ return (strcmp(mData, str)				!= 0); }


	protected:
		char*		mData;				// pointer to character data
		int			mLength;			// the length of the string
		int			mMaxLength;			// the maximum length

		// protected methods
		void		Alloc	(int length, int extraChars=0);		// allocate character buffer
		String&		Copy	(const char *what, int length);		// copy some data
		String&		Concat	(const char *what, int length);		// concatenate two strings
};


// external operators
inline String operator+ (const String&	strA, const String&	strB)					{ return String(strA.GetReadPtr(), strA.GetLength(), strB.GetReadPtr(), strB.GetLength()); }
inline String operator+ (const String&	strA, char			strB)					{ return String(strA.GetReadPtr(), strA.GetLength(), &strB, 1); }
inline String operator+ (char			strA, const String&	strB)					{ return String(&strA, 1, strB.GetReadPtr(), strB.GetLength()); }
inline String operator+ (const String&	strA, const char*	strB)					{ return String(strA.GetReadPtr(), strA.GetLength(), strB, (int)strlen(strB)); }
inline String operator+ (const char*	strA, const String&	strB)					{ return String(strA, (int)strlen(strA), strB.GetReadPtr(), strB.GetLength()); }

inline bool	operator< (const String&	strA, const String&	strB)					{ return (strcmp(strA, strB)<0); }
inline bool	operator< (const String&	strA, const char*	strB)					{ return (strcmp(strA, strB)<0); }
inline bool operator< (const char*		strA, const String&	strB)					{ return (strcmp(strA, strB)<0); }
inline bool operator> (const String&	strA, const String&	strB)					{ return (strcmp(strA, strB)>0); }
inline bool	operator> (const String&	strA, const char*	strB)					{ return (strcmp(strA, strB)>0); }
inline bool	operator> (const char*		strA, const String&	strB)					{ return (strcmp(strA, strB)>0); }
inline bool	operator<=(const String&	strA, const String&	strB)					{ return (strcmp(strA, strB)<=0); }
inline bool	operator<=(const String&	strA, const char*	strB)					{ return (strcmp(strA, strB)<=0); }
inline bool	operator<=(const char*		strA, const String&	strB)					{ return (strcmp(strA, strB)<=0); }
inline bool	operator>=(const String&	strA, const String&	strB)					{ return (strcmp(strA, strB)>=0); }
inline bool	operator>=(const String&	strA, const char*	strB)					{ return (strcmp(strA, strB)>=0); }
inline bool	operator>=(const char*		strA, const String&	strB)					{ return (strcmp(strA, strB)>=0); }
inline bool	operator==(const String&	strA, const String&	strB)					{ return ((strA.GetLength() == strB.GetLength()) && (memcmp(strA, strB, strA.GetLength())==0)); }
inline bool	operator==(const String&	strA, const char*	strB)					{ return ((strA.GetLength() == (int)strlen(strB)) && (memcmp(strA, strB, strA.GetLength())==0)); }
inline bool	operator==(const char*		strA, const String&	strB)					{ return (((int)strlen(strA) == strB.GetLength()) && (memcmp(strA, strB, strB.GetLength())==0)); }
inline bool	operator!=(const String&	strA, const String&	strB)					{ return ((strA.GetLength() != strB.GetLength()) || (memcmp(strA, strB, strA.GetLength())!=0)); }
inline bool	operator!=(const String&	strA, const char*	strB)					{ return (memcmp(strA, strB, strA.GetLength()+1)!=0); }
inline bool	operator!=(const char*		strA, const String&	strB)					{ return (memcmp(strA, strB, strB.GetLength()+1)!=0); }

}	// namespace MCore

#endif