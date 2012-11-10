#pragma once

#include "MWAllocObj.h"

namespace Myway
{

class MW_ENTRY String : public AllocObj
{
public:
    struct hash_t
    {
        // sbdsm hash

        int operator ()(const String & s) const
        {
            const char * str = s.c_str();
            unsigned int hash = 0;

            while (*str)
            {
                // equivalent to: hash = 65599*hash + (*str++);
                hash = (*str++) + (hash << 6) + (hash << 16) - hash;
            }

            return (hash & 0x7FFFFFFF);
        }
    };

    struct chash_t
    {
        // sbdsm hash

        int operator ()(const char * str) const
        {
            unsigned int hash = 0;

            while (*str)
            {
                // equivalent to: hash = 65599*hash + (*str++);
                hash = (*str++) + (hash << 6) + (hash << 16) - hash;
            }

            return (hash & 0x7FFFFFFF);
        }
    };

    struct cequal_t
    {
        // sbdsm hash

        bool operator ()(const char * left, const char * right) const
        {
            return Strcmp(left, right) == 0;
        }
    };


    struct cless_t
    {
        // sbdsm hash

        bool operator ()(const char * left, const char * right) const
        {
            return Strcmp(left, right) < 0;
        }
    };

    struct hash_rs
    {
        //rs hash
        int operator ()(const String & s) const
        { 
            const char * str = s.c_str();

            unsigned int b = 378551;
            unsigned int a = 63689;
            unsigned int hash = 0;

            while (*str)
            {
                hash = hash * a + (*str++);
                a *= b;
            }

            return (hash & 0x7FFFFFFF);
        }
    };

    struct hash_js
    {
        //js hash

        int operator ()(const String & s) const
        { 
            const char * str = s.c_str();

            unsigned int hash = 1315423911;

            while (*str)
            {
                hash ^= ((hash << 5) + (*str++) + (hash >> 2));
            }

            return (hash & 0x7FFFFFFF);
        }
    };

public:
					String();
	        		String(const String & str);
                    String(const char * str);
    explicit		String(char c);
    explicit		String(int x);
    explicit		String(float x);
    explicit		String(double x);
					~String();

					//operator const char*() const;

    char &          operator [](int i);
    const char &    operator [](int i) const;

    //operator = 
	String &		operator =(const String & str);
	String &		operator =(const char * str);
	String &		operator =(char c);
    String &        operator =(int x);
    String &        operator =(float x);
    String &        operator =(double x);

    //operator +=
	String &		operator +=(const String & str);
	String &		operator +=(const char * str);
	String &		operator +=(char c);
    String &        operator +=(int x);
    String &        operator +=(float x);
    String &        operator +=(double x);

    //operator +
	String			operator +(const String & str) const;
	String			operator +(const char * str) const;
	String			operator +(char c) const;
    String          operator +(int x) const;
    String          operator +(float x) const;
    String          operator +(double x) const;

    //operator ==
	bool			operator ==(const String & str) const;
	bool			operator ==(const char * str) const;

    //operator <
    bool            operator <(const String & str) const;

    //operator >
    bool            operator >(const String & str) const;
  

    //operator !=
	bool			operator !=(const String & str) const;
	bool			operator !=(const char * str) const;

    //operator <<
    String &        operator <<(const String & x);
    String &        operator <<(const char * x);
    String &        operator <<(char x);
    String &        operator <<(int x);
    String &        operator <<(float x);
    String &        operator <<(double x);

    //opertator >>
    String &        operator >>(String & x);
    String &        operator >>(char * x);
    String &        operator >>(char & x);
    String &        operator >>(int & x);
    String &        operator >>(float & x);
    String &        operator >>(double & x);

    void            Trim();
    void            Erase(int iPos, int iCount);
	void			Format(const char * fmt, ...);
    bool            Empty() const;
    void            Clear();
	int             Length() const;
    const char*     c_str() const;
    char *          c_str();
    int             ToInt() const;
    float           ToFloat() const;
    double          ToDouble() const;
    bool            Tobool() const;

    void            Lowercase();
    void            Uppercase();
    void            XorLowerUpper();

    /*
        find a char position
            @Return:
                pos  : found.
                npos : not found.
    */
	int 			Find(int pos, char c, bool bForward = true) const;

    /*
        find a string position
            @Return:
                not -1: found.
                -1: not found.
    */
    int          Find(const String & str, int pos = 0) const;

    /*
        compare two string
            @Return:
                < 0 : string < str
                > 0 : string > str
                = 0 : string = str
    */
    int           Compare(const String & str) const;
    int           _Compare(const char * str) const;

    void            SplitFileName(String & base, String & path) const;
    void            SplitFileNameReplace(String & base, String & path);

    void            Replace(char oldc, char newc);

    String &        AddSelf(const String & str);
    String          Add(const String & str) const;
    String &        Copy(const String & str);
    String          Append(int pos, const String & str) const;

    int             _Find(const char * str, int pos = 0) const;
    String &        _AddSelf(const char * str);
    String          _Add(const char * str) const;
    String &        _Copy(const char * str);
    String          _Append(int pos, const char * str) const;

    String          SubStr(int pos, int size) const;
	String			SubStr(int offset, char cStart, char cEnd) const;
    String          Split(char c, bool front = true) const;

    friend bool     operator ==(const char * l, const String & r);
	friend std::ostream & operator <<(std::ostream & os, const String & str);

    friend String   operator +(const char * x, const String & s);
    friend String	operator +(char x, const String & s);
    friend String   operator +(int x, const String & s);
    friend String   operator +(float x, const String & s);
    friend String   operator +(double x, const String & s);

private:
	char * mStr;
	int mLength;
    int mMaxSize;
    int mIncreaseSize;

    static const int msCacheSize = 64;
    bool mUseCache;
    char mCache[msCacheSize];
};

#include "MWString.inl"

}