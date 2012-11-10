
/*
    operator cont char *
*/
//inline String::operator const char*() const
//{
//    return mStr;
//}


/*
    operator []
*/
inline char & String::operator [](int i)
{
    debug_assert(i >= 0 && i < (int)mLength, "invalid index, string[].");
    return c_str()[i];
}

inline const char & String::operator [](int i) const
{
    debug_assert(i >= 0 && i < (int)mLength, "invalid index, string[].");
    return c_str()[i];
}

/*
    operator =
*/
inline String & String::operator =(const String & x)
{
    return Copy(x);
}

inline String & String::operator =(const char * x)
{
    return _Copy(x);
}

inline String & String::operator =(char x)
{
    return Copy(String(x));
}

inline String & String::operator =(int x)
{
    return Copy(String(x));
}

inline String & String::operator =(float x)
{
    return Copy(String(x));
}

inline String & String::operator =(double x)
{
    return Copy(String(x));
}

inline String & String::operator +=(const String & x)
{
    return AddSelf(x);
}


/*
    operator +
*/
inline String String::operator +(const String & x) const
{
    return Add(x);
}

inline String String::operator +(const char * x) const
{
    return _Add(x);
}

inline String String::operator +(char x) const
{
    return Add(String(x));
}

inline String String::operator +(int x) const
{
    return Add(String(x));
}

inline String String::operator +(float x) const
{
    return Add(String(x));
}

inline String String::operator +(double x) const
{
    return Add(String(x));
}


/*
    operator +=
*/
inline String & String::operator +=(const char * x)
{
    return _AddSelf(x);
}

inline String & String::operator +=(char x)
{
    return AddSelf(String(x));
}

inline String & String::operator +=(int x)
{
    return AddSelf(String(x));
}

inline String & String::operator +=(float x)
{
    return AddSelf(String(x));
}

inline String & String::operator +=(double x)
{
    return AddSelf(String(x));
}

/*
    operator ==
*/
inline bool String::operator ==(const String & str) const
{
    return Compare(str) == 0;
}

inline bool String::operator ==(const char * str) const
{
    return _Compare(str) == 0;
}

/*
    operator <
*/
inline bool String::operator <(const String & str) const
{
    return Compare(str) < 0;
}

/*
    operator >
*/
inline bool String::operator >(const String & str) const
{
    return Compare(str) > 0;
}

/*
    operator !=
*/
inline bool String::operator !=(const String & str) const
{
    return Compare(str) != 0;
}

inline bool String::operator !=(const char * str) const
{
    return _Compare(str) != 0;
}

/*
    operator <<
*/
inline String & String::operator <<(const String & x)
{
    return AddSelf(String(x));
}

inline String & String::operator <<(const char * x)
{
    return AddSelf(String(x));
}

inline String & String::operator <<(char x)
{
    return AddSelf(String(x));
}

inline String & String::operator <<(int x)
{
    return AddSelf(String(x));
}

inline String & String::operator <<(float x)
{
    return AddSelf(String(x));
}

inline String & String::operator <<(double x)
{
    return AddSelf(String(x));
}


/*
    access member
*/
inline void String::Clear()
{
    mLength = 0;
    c_str()[0] = 0;
}

inline bool String::Empty() const
{
    return mLength == 0;
}

inline int String::Length() const
{
    return mLength;
}

inline const char* String::c_str() const
{
    if (mUseCache)
    {
        return mCache;
    }
    else
    {
        return mStr;
    }
}

inline char* String::c_str()
{
    if (mUseCache)
    {
        return mCache;
    }
    else
    {
        return mStr;
    }
}

inline int String::ToInt() const
{
    return atoi(c_str());
}

inline float String::ToFloat() const
{
    return (float)atof(c_str());
}

inline double String::ToDouble() const
{
    return atof(c_str());
}

inline bool String::Tobool() const
{
    return _Compare("true") == 0;
}

//inline
inline bool operator ==(const char * l, const String & r)
{
    return r == l;
}

inline String operator +(const char * x, const String & s)
{
    return String(x) + s;
}

inline String operator +(char x, const String & s)
{
    return String(x) + s;
}


inline String operator +(int x, const String & s)
{
    return String(x) + s;
}

inline String operator +(float x, const String & s)
{
    return String(x) + s;
}

inline String operator +(double x, const String & s)
{
    return String(x) + s;
}