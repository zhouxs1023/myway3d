#include "MWString.h"
#include "MWMemory.h"

using namespace Myway;

#pragma warning(push)
#pragma warning(disable : 4999)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)


String::String()
{
    mLength = 0;
    mMaxSize = msCacheSize;
    mIncreaseSize = mMaxSize;
    mStr = NULL;

    mUseCache = TRUE;
    mCache[mLength] = 0;
}

String::~String()
{
	safe_free(mStr);
    mLength = 0;
    mMaxSize = msCacheSize;
    mIncreaseSize = mMaxSize;
    mUseCache = TRUE;
    mCache[mLength] = 0;
}

String::String(const String & str)
{
    mLength = str.mLength;
    mIncreaseSize = str.mIncreaseSize;
    mStr = NULL;
    mCache[0] = 0;

    if (str.mLength < msCacheSize)
    {
        mUseCache = TRUE;
        mMaxSize = msCacheSize;

        Memcpy(mCache, str.c_str(), mLength * sizeof(char));
        mCache[mLength] = 0;
    }
    else
    {
        mUseCache = FALSE;
        mMaxSize = str.mMaxSize;

        mStr = (char*)alloc_mem(mMaxSize);
        Memcpy(mStr, str.c_str(), mLength * sizeof(char));
        mStr[mLength] = 0;
    }
}

String::String(const char * str)
{
    mLength = Strlen(str);
    mStr = NULL;
    mCache[0] = 0;

    if (mLength < msCacheSize)
    {
        mMaxSize = msCacheSize;
        mIncreaseSize = mMaxSize;
        Memcpy(mCache, str, mLength * sizeof(char));
        mCache[mLength] = 0;
        mUseCache = TRUE;
    }
    else
    {
        mMaxSize = mLength + 1;
        mIncreaseSize = mMaxSize;
        mStr = (char*)alloc_mem(mMaxSize);
        Memcpy(mStr, str, mLength * sizeof(char));
        mStr[mLength] = 0;
        mUseCache = FALSE;
    }
}

String::String(char x)
{
    mLength = 1;
    mMaxSize = msCacheSize;
    mIncreaseSize = mMaxSize;
    mStr = NULL;

    mUseCache = TRUE;
    mCache[0] = x;
    mCache[1] = 0;
}

String::String(int x)
{
	itoa(x, mCache, 10);

	mLength = Strlen(mCache);
	mMaxSize = msCacheSize;
	mIncreaseSize = mMaxSize;
	mStr = NULL;
	mUseCache = TRUE;
	mCache[mLength] = 0;
}

String::String(float x)
{
    _gcvt(x, 10, mCache);

    mLength = Strlen(mCache);
    mMaxSize = msCacheSize;
    mIncreaseSize = mMaxSize;
    mStr = NULL;
    mUseCache = TRUE;
    mCache[mLength] = 0;
}

String::String(double x)
{
    _gcvt(x, 10, mCache);

    mLength = Strlen(mCache);
    mMaxSize = msCacheSize;
    mIncreaseSize = mMaxSize;
    mStr = NULL;
    mUseCache = TRUE;
    mCache[mLength] = 0;
}

void String::Lowercase()
{
    char * str = c_str();

    for (int i = 0; i < mLength; ++i)
    {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] |= 0x20;
    }
}

void String::Uppercase()
{
    char * str = c_str();

    for (int i = 0; i < mLength; ++i)
    {
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] &= ~0x20;
    }
}

void String::XorLowerUpper()
{
    char * str = c_str();

    for (int i = 0; i < mLength; ++i)
    {
        str[i] ^= 0x20;
    }
}

void String::Trim()
{
    if (mLength == 0)
        return;

    char * str = c_str();

    //remove back
    int i = mLength - 1;
    while (i >= 0 && (str[i] == ' ' ||
                      str[i] == '\t' ||
                      str[i] == '\r' ||
                      str[i] == '\n'))
    {
        --mLength;
        --i;
    }

    //remove front
    i = 0;
    while (i < mLength && (str[i] == ' ' ||
                            str[i] == '\t' ||
                            str[i] == '\r' ||
                            str[i] == '\n'))
    {
        ++i;
    }

    //copy data
    int iPos = 0;
    int iCopyPos = i;
    while (iCopyPos < mLength)
        str[iPos++] = str[iCopyPos++];

    mLength -= i;
    str[mLength] = '\0';
}

void String::Erase(int iPos, int iCount)
{
    debug_assert(iPos >= 0 && iPos + iCount <= mLength, "invalid param.");

    char * str = c_str();

    int iCopyPos = iPos + iCount;

    while (iCopyPos < mLength)
        str[iPos++] = str[iCopyPos++];

    mLength -= iCount;
    str[mLength] = '\0';
}

void String::Format(const char * fmt, ...)
{
	const int TEXT_BUFFER_SIZE = 2048;
	char text[TEXT_BUFFER_SIZE];
	va_list arglist;

	va_start(arglist, fmt);
	vsprintf(text, fmt, arglist);
	va_end(arglist);

	(*this) = text;
}

int String::Find(int pos, char c, bool bForward) const
{
	if (pos > Length())
		return NPOS;

    const char * str = c_str();

	if (bForward)
	{
		for (int i = pos; i < mLength; ++i)
		{
			if (str[i] == c)
				return i;
		}

		return NPOS;
	}
	else
	{
		for (int i = pos; i >= 0; --i)
		{
			if (str[i] == c)
				return i;
		}

		return NPOS;
	}
}

int String::Find(const String & sKey, int pos) const
{
    const char * dst = c_str();
    const char * src = sKey.c_str();
    int index = pos;
    char eq = 0;

    while (*dst)
    {
        dst = c_str() + index;
        src = sKey.c_str();

        while (*src && !(eq = (*src - *dst)))
            ++src, ++dst;

        if (!eq)
            break;

        ++index;
    }

    return eq ? NPOS : index;
}


int String::Compare(const String & str) const
{
    const char * left = c_str();
    const char * right = str.c_str();
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret;
}

int String::_Compare(const char * str) const
{
    const char * left = c_str();
    const char * right = str;
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret;
}

void String::SplitFileName(String & base, String & path) const
{
    String name = *this;

    name.Replace('\\', '/');

    int len = name.Length();
    int split = name.Find(len, '/', FALSE);

    if (split != NPOS)
    {
        base = name.SubStr(split + 1, len - split);
        path = name.SubStr(0, split);
    }
    else
    {
        base = name;
        path = ".";
    }
}

void String::SplitFileNameReplace(String & base, String & path)
{
    String & name = *this;

    name.Replace('\\', '/');

    int len = name.Length();
    int split = name.Find(len, '/', FALSE);

    if (split != NPOS)
    {
        base = name.SubStr(split + 1, len - split);
        path = name.SubStr(0, split);
    }
    else
    {
        base = name;
        path = ".";
    }
}

void String::Replace(char oldc, char newc)
{
    char * str = c_str();

    for (int i = 0; i < mLength; ++i)
    {
        if (str[i] == oldc)
            str[i] = newc;
    }
}

String String::Add(const String & rk) const
{
    String sOut;
    int iSize = Length() + rk.Length() + 1;

    if (sOut.mMaxSize <= iSize)
    {
        safe_free(sOut.mStr);

        do 
        {
            sOut.mMaxSize += sOut.mIncreaseSize;
            sOut.mIncreaseSize = sOut.mMaxSize;
        } while (sOut.mMaxSize < iSize);

        sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
        sOut.mUseCache = FALSE;
        sOut.mCache[0] = 0;
    }

    Memcpy(sOut.c_str(), c_str(), Length());
    Memcpy(sOut.c_str() + mLength, rk.c_str(), rk.Length() * sizeof(char));
    sOut.mLength = iSize - 1;
    sOut.c_str()[sOut.mLength] = 0;

    return sOut;
}

String & String::AddSelf(const String & str)
{
    int iSize = Length() + str.Length() + 1;

    if (mUseCache)
    {
        if (mMaxSize < iSize)
        {
            do 
            {
                mMaxSize += mIncreaseSize;
                mIncreaseSize = mMaxSize;
            } while (mMaxSize < iSize);

            mStr = (char*)alloc_mem(mMaxSize);
            Memcpy(mStr, mCache, mLength);
            Memcpy(&mStr[mLength], str.c_str(), str.mLength * sizeof(char));
            mUseCache = FALSE;
            mCache[0] = 0;
        }
        else
        {
            Memcpy(&mCache[mLength], str.c_str(), str.Length() * sizeof(char));
        }
    }
    else
    {
        if (mMaxSize < iSize)
        {
            do 
            {
                mMaxSize += mIncreaseSize;
                mIncreaseSize = mMaxSize;
            } while (mMaxSize < iSize);

            char * tmp = (char*)alloc_mem(mMaxSize);
            Memcpy(tmp, mStr, mLength);
            safe_free(mStr);
            mStr = tmp;
        }

        Memcpy(&mStr[mLength], str.c_str(), str.mLength * sizeof(char));
    }

    mLength = iSize - 1;
    c_str()[mLength] = 0;

    return *this;
}

String & String::Copy(const String & str)
{
    int iSize = str.Length() + 1;

    if (mUseCache)
    {
        if (mMaxSize < iSize)
        {
            do 
            {
                mMaxSize += mIncreaseSize;
                mIncreaseSize = mMaxSize;
            } while (mMaxSize < iSize);

            mStr = (char*)alloc_mem(mMaxSize);
            Memcpy(mCache, str.c_str(), str.Length() * sizeof(char));
            mUseCache = FALSE;
            mCache[0] = 0;
        }
        else
        {
            Memcpy(mCache, str.c_str(), str.Length() * sizeof(char));
        }
    }
    else
    {
        if (mMaxSize < iSize)
        {
            do 
            {
                mMaxSize += mIncreaseSize;
                mIncreaseSize = mMaxSize;

            } while (mMaxSize < iSize);

            safe_free(mStr);
            mStr = (char*)alloc_mem(mMaxSize);
        }

        Memcpy(mStr, str.c_str(), str.Length() * sizeof(char));
    }

    mLength = str.Length();
    c_str()[mLength] = 0;

    return *this;
}

String String::Append(int pos, const String & str) const
{
    String sOut;
    int iSize = mLength + str.mLength + 1;

    if (sOut.mMaxSize < iSize)
    {
        do 
        {
            sOut.mMaxSize += sOut.mIncreaseSize;
            sOut.mIncreaseSize = sOut.mMaxSize;
        } while (sOut.mMaxSize < iSize);

        sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
        sOut.mUseCache = FALSE;
        sOut.mCache[0] = 0;
    }

    Memcpy(sOut.c_str(), c_str(), pos);
    Memcpy(sOut.c_str() + pos, str.c_str(), str.mLength);
    Memcpy(sOut.c_str() + pos + str.mLength, c_str() + pos, mLength - pos);

    sOut.mLength = iSize - 1;
    sOut.c_str()[sOut.mLength] = 0;

    return sOut;
}

String String::_Append(int pos, const char * str) const
{
    String sOut;
    int iLen = Strlen(str);
    int iSize = mLength + iLen + 1;

    if (sOut.mMaxSize < iSize)
    {
        do 
        {
            sOut.mMaxSize += sOut.mIncreaseSize;
            sOut.mIncreaseSize = sOut.mMaxSize;
        } while (sOut.mMaxSize < iSize);

        sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
        sOut.mUseCache = FALSE;
        sOut.mCache[0] = 0;
    }


    Memcpy(sOut.c_str(), c_str(), pos);
    Memcpy(sOut.c_str() + pos, str, iLen);
    Memcpy(sOut.c_str() + pos + iLen, c_str() + pos, mLength - pos);

    sOut.mLength = iSize - 1;
    sOut.c_str()[sOut.mLength] = 0;

    return sOut;
}

int String::_Find(const char * str, int pos) const
{
    const char * dst = c_str();
    const char * src = str;
    int index = pos;
    char eq = 0;

    while (*dst)
    {
        dst = c_str() + index;
        src = str;

        while (*src && !(eq = (*src - *dst)))
            ++src, ++dst;

        if (!eq)
            break;

        ++index;
    }

    return eq ? NPOS :index;
}

String & String::_AddSelf(const char * str)
{
    int iLen = Strlen(str);
    int iSize = mLength + iLen + 1;

    if (mMaxSize < iSize)
    {
        do 
        {
            mMaxSize += mIncreaseSize;
            mIncreaseSize = mMaxSize;
        } while (mMaxSize < iSize);

        char * temp = (char*)alloc_mem(mMaxSize);
        Memcpy(temp, c_str(), mLength);
        Memcpy(temp + mLength, str, iLen * sizeof(char));

        safe_free (mStr);
        mStr = temp;
        mUseCache = FALSE;
        mCache[0] = 0;
    }
    else
    {
        Memcpy(c_str() + mLength, str, iLen * sizeof(char));
    }

    mLength = iSize - 1;
    c_str()[mLength] = 0;

    return *this;
}

String String::_Add(const char * str) const
{
    String sOut;
    int iLen = Strlen(str);
    int iSize = mLength + iLen + 1;

    if (sOut.mMaxSize <= iSize)
    {
        safe_free(sOut.mStr);

        do 
        {
            sOut.mMaxSize += sOut.mIncreaseSize;
            sOut.mIncreaseSize = sOut.mMaxSize;
        } while (sOut.mMaxSize < iSize);

        sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
        sOut.mUseCache = FALSE;
        sOut.mCache[0] = 0;
    }

    Memcpy(sOut.c_str(), c_str(), mLength);
    Memcpy(sOut.c_str() + mLength, str, iLen* sizeof(char));
    sOut.mLength = iSize - 1;
    sOut.c_str()[sOut.mLength] = 0;

    return sOut;
}

String & String::_Copy(const char * str)
{
    int iLen = Strlen(str);
    int iSize = iLen + 1;

    if (mMaxSize < iSize)
    {
        do 
        {
            mMaxSize += mIncreaseSize;
            mIncreaseSize = mMaxSize;

        } while (mMaxSize < iSize);

        safe_free(mStr);
        mStr = (char*)alloc_mem(mMaxSize);
        mUseCache = FALSE;
        mCache[0] = 0;
    }
    
    mLength = iLen;
    Memcpy(c_str(), str, mLength * sizeof(char));
    c_str()[mLength] = 0;

    return *this;
}

String String::SubStr(int pos, int size) const
{
    if (pos >= mLength && pos + size > mLength)
        return "";

   String sOut;

   sOut.mLength = size;
   int iSize = sOut.mLength + 1;

    if (sOut.mMaxSize <= iSize)
    {
        safe_free(sOut.mStr);

        do 
        {
            sOut.mMaxSize += sOut.mIncreaseSize;
            sOut.mIncreaseSize = sOut.mMaxSize;
        } while (sOut.mMaxSize < iSize);

        sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
        sOut.mUseCache = FALSE;
        sOut.mCache[0] = 0;
    }

    Memcpy(sOut.c_str(), c_str() + pos, sOut.mLength * sizeof(char));
    sOut.c_str()[sOut.mLength] = 0;

    return sOut;
}

String String::SubStr(int offset, char cStart, char cEnd) const
 {
	String sOut;
	int is, ie;
	if (offset >= mLength)
		return sOut;

    const char * str = c_str();
	is = offset;
	if (cStart != 0)
	{
		for (is = offset; is < mLength; is++)
		{
			if (str[is] == cStart)
				break;
		}

		is++;
		offset = is;
	}

	ie = mLength;
	if (cEnd != 0)
	{
		for (ie = offset; ie < mLength; ie++)
		{
			if (str[ie] == cEnd)
				break;
		}

		if (ie >= mLength)
			return sOut;
	}

	if (is < ie)
	{
		sOut.mLength = ie - is;
        int iSize = sOut.mLength + 1;

        if (sOut.mMaxSize <= iSize)
        {
            safe_free(sOut.mStr);

            do 
            {
                sOut.mMaxSize += sOut.mIncreaseSize;
                sOut.mIncreaseSize = sOut.mMaxSize;
            } while (sOut.mMaxSize < iSize);

            sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
            sOut.mUseCache = FALSE;
            sOut.mCache[0] = 0;
        }
        
        Memcpy(sOut.c_str(), c_str() + is, sOut.mLength * sizeof(char));
		sOut.c_str()[sOut.mLength] = 0;
	}

	return sOut;
}

String String::Split(char c, bool front) const
{
    String sOut;
    int mid = NPOS;

    for (int i = 0; i < mLength; ++i)
    {
        if (mStr[i] == c)
        {
            mid = i;
            break;
        }
    }

    if (mid != NPOS)
    {
        if (front)
        {
            sOut.mLength = mid;

            int iSize = sOut.mLength + 1;

            if (sOut.mMaxSize <= iSize)
            {
                safe_free(sOut.mStr);

                do 
                {
                    sOut.mMaxSize += sOut.mIncreaseSize;
                    sOut.mIncreaseSize = sOut.mMaxSize;
                } while (sOut.mMaxSize < iSize);

                sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
                sOut.mUseCache = FALSE;
                sOut.mCache[0] = 0;
            }

            Memcpy(sOut.c_str(), c_str(), sOut.mLength * sizeof(char));
            sOut.c_str()[sOut.mLength] = 0;
        }
        else
        {
            sOut.mLength = mLength - (mid + 1);
            int iSize = sOut.mLength + 1;

            if (sOut.mMaxSize <= iSize)
            {
                safe_free(sOut.mStr);

                do 
                {
                    sOut.mMaxSize += sOut.mIncreaseSize;
                    sOut.mIncreaseSize = sOut.mMaxSize;
                } while (sOut.mMaxSize < iSize);

                sOut.mStr = (char *)alloc_mem(sOut.mMaxSize);
                sOut.mUseCache = FALSE;
                sOut.mCache[0] = 0;
            }

            Memcpy(sOut.c_str(), c_str() + mid + 1, sOut.mLength * sizeof(char));
            sOut.c_str()[sOut.mLength] = 0;
        }
    }

    return sOut;
}

std::ostream & operator <<(std::ostream & os, const String & str)
{
	os << str.c_str();
	return os;
}

/*
    operator >>
*/
String & String::operator >>(String & x)
{
    x.Clear();

    if (Length() > 0)
    {
        int iSplit = mLength - 1;
        char * str = c_str();
        bool skip = false;

        while (iSplit > 0)
        {
            if (str[iSplit] == ' ' ||
                str[iSplit] == '\t' ||
                str[iSplit] == '\n')
            {
                skip = true;
                break;
            }

            --iSplit;
        }

        int iSize = mLength - iSplit + 1;
        if (x.mMaxSize < iSize)
        {
            do 
            {
                x.mMaxSize += x.mIncreaseSize;
                x.mIncreaseSize = x.mMaxSize;

            } while (x.mMaxSize < iSize);

            safe_free(x.mStr);
            x.mStr = (char*)alloc_mem(x.mMaxSize);
            mUseCache = FALSE;
            mCache[0] = 0;
        }

        x.mLength = iSize - 1;
        Memcpy(x.c_str(), &str[iSplit + (skip ? 1 : 0)], iSize * sizeof(char));
        str[iSplit] = 0;
        mLength = iSplit;
    }
    
    return *this;
}

String & String::operator >>(char * x)
{
    if (Length() == 0)
    {
        x[0] = 0;
    }
    else
    {
        int iSplit = mLength - 1;
        x[0] = 0;
        bool skip = false;

        char * str = c_str();
        while (iSplit > 0)
        {
            if (str[iSplit] == ' ' ||
                str[iSplit] == '\t' ||
                str[iSplit] == '\n')
            {
                skip = true;
                break;
            }

            --iSplit;
        }

        int iSize = mLength - iSplit + 1;
        Memcpy(x, &str[iSplit + (skip ? 1 : 0)], iSize * sizeof(char));
        str[iSplit] = 0;
        mLength = iSplit;
    }

    return *this;
}

String & String::operator >>(char & x)
{
    x = 0;

    if (mLength > 0)
    {
        --mLength;

        char * str = c_str();

        x = str[mLength];
        str[mLength] = 0;
    }

    return *this;
}

String & String::operator >>(int & x)
{
    x = 0;

    if (Length() > 0)
    {
        char * str = c_str();
        int iSplit = mLength - 1;
        bool number = false;

        while (iSplit > 0)
        {
            if (str[iSplit] < '0' ||
                str[iSplit] > '9')
                break;

            --iSplit;
            number = true;
        }

        if (number)
        {
            int y = atoi(&str[iSplit]);
            x = y;

            str[iSplit + 1] = 0;
            mLength = iSplit + 1;
        }
    }

    return *this;
}

String & String::operator >>(float & x)
{
    x = 0;

    if (Length() > 0)
    {
        bool point = false;
        bool number = false;
        int iSplit = mLength - 1;
        char * str = c_str();

        while (iSplit > 0)
        {
            if (str[iSplit] < '0' ||
                str[iSplit] > '9')
            {
                if (str[iSplit] == '.' && !point)
                    point = true;
                else
                    break;
            }

            number = true;
            --iSplit;
        }

        if (number)
        {
            x = (float)strtod(&str[iSplit + 1], (char**)NULL);
            str[iSplit + 1] = 0;
            mLength = iSplit + 1;
        }

        
    }
    
    return *this;
}

String & String::operator >>(double & x)
{
    x = 0;

    if (Length() > 0)
    {
        bool point = false;
        bool number = false;
        int iSplit = mLength - 1;
        char * str = c_str();

        while (iSplit > 0)
        {
            if (str[iSplit] < '0' ||
                str[iSplit] > '9')
            {
                if (str[iSplit] == '.' && !point)
                    point = true;
                else
                    break;
            }

            number = true;
            --iSplit;
        }

        if (number)
        {
            x = strtod(&str[iSplit + 1], (char**)NULL);
            str[iSplit + 1] = 0;
            mLength = iSplit + 1;
        }
    }

    return *this;
}

#pragma warning(pop)
