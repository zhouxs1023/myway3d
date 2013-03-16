#pragma once

#include "MWAllocator.h"
#include "MWCharSet.h"

namespace Myway
{

#pragma warning(push)
#pragma warning(disable : 4996)

template <int Size>
class TString
{
    DECLARE_ALLOC();

public:
    struct hash_t
    {
        // sbdsm hash

        int operator ()(const TString & rk) const
        {
            const char * str = rk.c_str();
            unsigned int hash = 0;

            while (*str)
            {
                // equivalent to: hash = 65599*hash + (*str++);
                hash = (*str++) + (hash << 6) + (hash << 16) - hash;
            }

            return (hash & 0x7FFFFFFF);
        }
    };

public:
    TString()
    {
        mStr[0] = 0;
    }

    ~TString()
    {
    }

    TString(const TString & rk)
    {
        *this = rk;
    }

    TString(const char * rk)
    {
        *this = rk;
    }

    explicit TString(char x)
    {
        *this = x;
    }

    explicit TString(int x)
    {
        *this = x;
    }

    explicit TString(float x)
    {
        *this = x;;
    }

    explicit TString(double x);

    char & operator [](int i)
    {
        return mStr[i];
    }

    const char & operator [](int i) const
    {
        return mStr[i];
    }

    //operator = 
    TString & operator =(const TString & rk)
    {
        strcpy(mStr, rk.c_str());
        return *this;
    }

    TString & operator =(const char * rk)
    {
        strcpy(mStr, rk);
        return *this;
    }

    TString & operator =(char c)
    {
        mStr[0] = c; mStr[1] = 0;
        return *this;
    }

    TString & operator =(int x)
    {
        itoa(x, mStr, 10);
        return *this;
    }

    TString & operator =(float x)
    {
        _gcvt(x, 10, mStr);
        return *this;
    }

    TString & operator =(double x)
    {
        _gcvt(x, 10, mStr);
        return *this;
    }

    //operator +=
    TString & operator +=(const TString & rk)
    {
        d_assert (Length() + rk.Length() < Size);
        strcat(mStr, rk.c_str());

        return *this;
    }

    TString & operator +=(const char * rk)
    {
        d_assert (Length() + strlen(rk) < Size);
        strcat(mStr, rk);

        return *this;
    }

    TString & operator +=(char x)
    {
        int len = Length();

        d_assert (len + 1 < Size);

        mStr[len] = x;
        mStr[len + 1] = 0;

        return *this;
    }

    TString & operator +=(int x)
    {
        char tmp[32];
        itoa(x, tmp, 10);
        *this += tmp;
        return *this;
    }

    TString & operator +=(float x)
    {
        char tmp[32];
        _gcvt(x, 10, tmp);
        *this += tmp;
        return *this;
    }

    TString & operator +=(double x)
    {
        char tmp[32];
        _gcvt(x, 10, tmp);
        *this += tmp;
        return *this;
    }

    //operator +
    TString operator +(const TString & rk) const
    {
        TString tmp = *this;
        tmp += rk;
        return tmp;
    }

    TString operator +(const char * rk) const
    {
        TString tmp = *this;
        tmp += rk;
        return tmp;
    }

    TString operator +(char c) const
    {
        TString tmp = *this;
        tmp += c;
        return tmp;
    }

    TString operator +(int x) const
    {
        TString tmp = *this;
        tmp += x;
        return tmp;
    }

    TString operator +(float x) const
    {
        TString tmp = *this;
        tmp += x;
        return tmp;
    }

    TString operator +(double x) const
    {
        TString tmp = *this;
        tmp += x;
        return tmp;
    }

    friend TString operator +(const char * lk, const TString & rk)
    {
		char tmp[4094];

		Strcpy(tmp, 4094, lk);
		Strcat(tmp, 4094, rk.c_str());

        return tmp;
    }

    //operator ==
    bool operator ==(const TString & rk) const
    {
        return strcmp(mStr, rk.c_str()) == 0;
    }

    bool operator ==(const char * rk) const
    {
        return strcmp(mStr, rk) == 0;
    }

    friend TString operator ==(const char * lk, const TString & rk)
    {
        return rk == lk;
    }

    //operator <
    bool operator <(const TString & rk) const
    {
        return strcmp(mStr, rk.c_str()) < 0;
    }

    //operator >
    bool operator >(const TString & rk) const
    {
        return strcmp(mStr, rk.c_str()) > 0;
    }


    //operator !=
    bool operator !=(const TString & rk) const
    {
        return strcmp(mStr, rk.c_str()) != 0;
    }

    bool operator !=(const char * rk) const
    {
        return strcmp(mStr, rk) != 0;
    }

    void Trim()
    {
        if (mStr[0] == 0)
            return;

        char * str = c_str();
        int len = Length();

        //remove back
        int i = len - 1;
        while (i >= 0 && (str[i] == ' ' ||
            str[i] == '\t' ||
            str[i] == '\r' ||
            str[i] == '\n'))
        {
            --len;
            --i;
        }

        //remove front
        i = 0;
        while (i < len && (str[i] == ' ' ||
            str[i] == '\t' ||
            str[i] == '\r' ||
            str[i] == '\n'))
        {
            ++i;
        }

        //copy data
        int iPos = 0;
        int iCopyPos = i;
        while (iCopyPos < len)
            str[iPos++] = str[iCopyPos++];

        len -= i;
        str[len] = 0;
    }

    void Erase(int pos, int count)
    {
        int len = Length();

        d_assert(pos >= 0 && pos + count <= len);

        char * str = c_str();

        int iPos = pos;
        int iCopyPos = pos + count;

        while (iCopyPos < len)
            str[iPos++] = str[iCopyPos++];

        len -= count;
        str[len] = '\0';
    }

    void Format(const char * fmt, ...)
    {
        const int TEXT_BUFFER_SIZE = 1024;
        char text[TEXT_BUFFER_SIZE];
        va_list arglist;

        va_start(arglist, fmt);
        vsprintf(text, fmt, arglist);
        va_end(arglist);

        (*this) = text;
    }

    void Clear()
    {
        mStr[0] = 0;
    }

    int Length() const
    {
        return strlen(mStr);
    }

    const char * c_str() const
    {
        return mStr;
    }

	const wchar_t * c_wstr() const
	{
		static wchar_t buffer[Size];

		CharSet::AnsiToUnicode(buffer, Size, mStr);

		return buffer;
	}

    char * c_str()
    {
        return mStr;
    }

    int ToInt() const
    {
        return atoi(mStr);
    }

    float ToFloat() const
    {
        return atof(mStr);
    }

    double ToDouble() const
    {
        return atof(mStr);
    }

    bool ToBool() const
    {
        return *this == "true" || *this == "True";
    }

    void Lower()
    {
        int len = Length();

        for (int i = 0; i < len; ++i)
        {
			if (mStr[i] >= 'A' && mStr[i] <= 'Z')
				mStr[i] |= 0x20;
        }
    }

    void Upper()
    {
        int len = Length();

        for (int i = 0; i < len; ++i)
        {
			if (mStr[i]>= 'a' && mStr[i] <= 'z')
				mStr[i] &= ~0x20;
        }
    }

    TString LowerR() const
    {
        TString r = *this;
        r.Lower();
        return r;
    }

    void UpperR()
    {
        TString r = *this;
        r.Upper();
        return r;
    }

    void XorLowerUpper()
    {
        int len = Length();

        for (int i = 0; i < len; ++i)
        {
            mStr[i] ^= 0x20;
        }
    }

    int Find(int pos, char c, bool bForward = true) const
    {
        int len = Length();

        if (pos > len)
            return NPOS;

        const char * str = c_str();

        if (bForward)
        {
            for (int i = pos; i < len; ++i)
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

	bool IsBegin(const TString & rk) const
	{
		const char * dst = c_str();
		const char * src = rk.c_str();

		while (*dst && *src)
		{
			if (*dst != *src)
				return false;

			++dst, ++src;
		}

		if (*src == 0)
			return true;

		return false;
	}

    int Find(const TString & rk, int pos = 0) const
    {
        const char * dst = c_str();
        const char * src = rk.c_str();
        int index = pos;
        char eq = 0;

        while (*dst)
        {
            dst = c_str() + index;
            src = rk.c_str();

            while (*src && !(eq = (*src - *dst)))
                ++src, ++dst;

            if (!eq)
                break;

            ++index;
        }

        return eq ? NPOS : index;
    }

    void SplitFileName(TString & base, TString & path) const
    {
        TString name = *this;

        name.Replace('\\', '/');

        int len = name.Length();
        int split = name.Find(len, '/', false);

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

    void SplitFileNameR(TString & base, TString & path)
    {
        Replace('\\', '/');

        int len = Length();
        int split = Find(len, '/', false);

        if (split != NPOS)
        {
            base = SubStr(split + 1, len - split);
            path = SubStr(0, split);
        }
        else
        {
            base = *this;
            path = ".";
        }
    }

    void Replace(char oldc, char newc)
    {
        char * str = c_str();
        int len = Length();

        for (int i = 0; i < len; ++i)
        {
            if (str[i] == oldc)
                str[i] = newc;
        }
    }

    TString SubStr(int pos, int size) const
    {
        TString tmp;

        memcpy(tmp.c_str(), c_str() + pos, size * sizeof(char));
        tmp[size] = 0;

        return tmp;
    }

    TString SubStr(int offset, char cStart, char cEnd) const
    {
        int len = Length();
        int is, ie;
        if (offset >= len)
            return "";

        const char * str = c_str();
        is = offset;
        if (cStart != 0)
        {
            for (is = offset; is < len; is++)
            {
                if (str[is] == cStart)
                    break;
            }

            is++;
            offset = is;
        }

        ie = len;
        if (cEnd != 0)
        {
            for (ie = offset; ie < len; ie++)
            {
                if (str[ie] == cEnd)
                    break;
            }

            if (ie >= len)
                return "";
        }

        if (is < ie)
        {
            return SubStr(is, ie - is);
        }

        return "";
    }

    int Split(char c, TString & front, TString & back) const
    {
        front = "";
        back = "";

        int len = Length();
        int mid = NPOS;

        for (int i = 0; i < len; ++i)
        {
            if (mStr[i] == c)
            {
                mid = i;
                break;
            }
        }

        if (mid != NPOS)
        {
            memcpy(front.c_str(), c_str(), mid);
            front[mid] = 0;

            memcpy(back.c_str(), c_str() + mid + 1, len - (mid + 1));
            back[len - (mid + 1)] = 0;
        }

        return mid;
    }

protected:
    char mStr[Size];
};


typedef TString<32>      TString32;
typedef TString<64>      TString64;
typedef TString<128>     TString128;
typedef TString<256>     TString256;
typedef TString<512>     TString512;
typedef TString<1024>    TString1024;
typedef TString<2048>    TString2048;

#pragma warning(pop)

}