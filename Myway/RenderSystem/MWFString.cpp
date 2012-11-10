#include "MWFString.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error current not support.
#endif

#pragma warning(push)

#pragma warning(disable : 4018)

namespace Myway
{


FString::FString()
{
    m_length = 0;
    mMax_size = DEFAULT_INCREASE_SIZE;
    mncrease_size = mMax_size;
    m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    m_data[m_length] = 0;
}

FString::FString(const ftchar * str)
{
    int length = 0;
    while (str[length])
        ++length;

    mMax_size = length + 1;
    m_length = length;
    mncrease_size = mMax_size;

    m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    Memcpy(m_data, str, m_length * sizeof(ftchar));
    m_data[m_length] = 0;
}

FString::FString(const FString & r)
{
    mMax_size = r.mMax_size;
    m_length = r.m_length;
    mncrease_size = r.mncrease_size;

    m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    Memcpy(m_data, r.m_data, m_length * sizeof(ftchar));
    m_data[m_length] = 0;
}

FString::~FString()
{
    safe_free(m_data);
}

void FString::FromAnsiString(const char * str)
{

#ifdef MW_PLATFORM_WIN32

    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

    wchar_t * wstr = (wchar_t*)alloc_mem(sizeof(wchar_t) * len);

    MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len);

    FromUnicoderString(wstr);

    free_mem(wstr);

#else

#error current not support.

#endif

}

void FString::FromUnicoderString(const wchar_t * str)
{
    size_t len = wcslen(str);

    if (len >= mMax_size)
    {
        do
        {
            mMax_size += mncrease_size;
            mncrease_size = mMax_size;
        } while (len >= mMax_size);

        safe_free(m_data);
        m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    }

    for (size_t i = 0; i < len; ++i)
    {
        m_data[i] = str[i];
    }

    m_length = (int)len;
    m_data[m_length] = 0;
}

void FString::FromUTF8String(const char * str)
{

#ifdef MW_PLATFORM_WIN32

    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL,0);

    wchar_t * wstr = (wchar_t*)alloc_mem(sizeof(wchar_t) * len);

    MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, len);

    FromUnicoderString(wstr);

    free_mem(wstr);

#else

#error current not support.

#endif

}

void FString::Clear()
{
    m_data[0] = 0;
    m_length = 0;
}

bool FString::Empty() const
{
    return m_length == 0;
}

int FString::Length() const
{
    return m_length;
}

FString FString::operator +(const FString & r) const
{
    FString ret;
    int len = m_length + r.m_length + 1;

    if (len >= ret.mMax_size)
    {
        do
        {
            ret.mMax_size += mncrease_size;
            ret.mncrease_size = mMax_size;
        } while (len >= ret.mMax_size);

        safe_free(ret.m_data);
        ret.m_data = (ftchar*)alloc_mem(ret.mMax_size * sizeof(ftchar));
    }

    Memcpy(ret.m_data, m_data, m_length * sizeof(ftchar));
    Memcpy(ret.m_data + m_length, r.m_data, r.m_length * sizeof(ftchar));

    ret.m_length = len;
    ret.m_data[m_length] = 0;

    return ret;
}

FString & FString::operator +=(const FString & r)
{
    int len = m_length + r.m_length + 1;

    if (len >= mMax_size)
    {
        do
        {
            mMax_size += mncrease_size;
            mncrease_size = mMax_size;
        } while (len >= mMax_size);

        
        ftchar * data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
        Memcpy(data, m_data, m_length * sizeof(ftchar));
        safe_free(m_data);
        m_data = data;
    }

    Memcpy(m_data + m_length, r.m_data, r.m_length * sizeof(ftchar));

    m_length = len;
    m_data[m_length] = 0;

    return *this;
}

FString & FString::operator =(const FString & r)
{
    if (r.m_length >= mMax_size)
    {
        do 
        {
            mMax_size += mncrease_size;
            mncrease_size = mMax_size;
        } while (r.m_length >= mMax_size);

        safe_free(m_data);
        m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    }

    Memcpy(m_data, r.m_data, r.m_length * sizeof(ftchar));

    m_length = r.m_length;
    m_data[m_length] = 0;

    return *this;
}

bool FString::operator ==(const FString & r) const
{
    const ftchar * left = m_data;
    const ftchar * right = r.m_data;
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret == 0;
}

bool FString::operator !=(const FString & r) const
{
    const ftchar * left = m_data;
    const ftchar * right = r.m_data;
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret != 0;
}

ftchar & FString::operator [](size_t i)
{
    assert(i < m_length);

    return m_data[i];
}

const ftchar & FString::operator [](size_t i) const
{
    assert(i < m_length);

    return m_data[i];
}

FString & FString::operator =(const ftchar * r)
{
    int length = 0;
    while (r[length])
        ++length;

    if (length >= mMax_size)
    {
        do 
        {
            mMax_size += mncrease_size;
            mncrease_size = mMax_size;
        } while (length >= mMax_size);

        safe_free(m_data);
        m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    }

    m_data = (ftchar*)alloc_mem(mMax_size * sizeof(ftchar));
    Memcpy(m_data, r, length * sizeof(ftchar));

    m_length = length;
    m_data[m_length] = 0;

    return *this;
}

bool FString::operator ==(const ftchar * right) const
{
    const ftchar * left = m_data;
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret == 0;
}

bool FString::operator !=(const ftchar * right) const
{
    const ftchar * left = m_data;
    int ret = 0;

    while (!(ret = (*left - *right)) && (*left))
        ++left, ++right;

    return ret != 0;
}

bool Myway::operator ==(const ftchar * left, const FString & right)
{
    return right == left;
}

bool Myway::operator !=(const ftchar * left, const FString & right)
{
    return right != left;
}

}


#pragma warning(pop)
