#pragma once

#include "MWAllocObj.h"

namespace Myway
{

typedef int      ftchar;

class MW_ENTRY FString : public AllocObj
{
public:
                FString();
                FString(const FString & r);
                ~FString();

                FString(const ftchar * str);

    void        FromAnsiString(const char * str);
    void        FromUnicoderString(const wchar_t * str);
    void        FromUTF8String(const char * str);

    void        Clear();
    bool        Empty() const;
    int      Length() const;

    FString     operator +(const FString & r) const;
    FString &   operator +=(const FString & r);

    FString &   operator =(const ftchar * r);
    FString &   operator =(const FString & r);

    bool        operator ==(const FString & r) const;
    bool        operator ==(const ftchar * r) const;

    bool        operator !=(const FString & r) const;
    bool        operator !=(const ftchar * r) const;


    ftchar &     operator [](size_t i);
    const ftchar & operator [](size_t i) const;

    friend bool  operator ==(const ftchar * left, const FString & right);
    friend bool  operator !=(const ftchar * left, const FString & right);
    
protected:
    ftchar *        m_data;
    int          m_length;
    int          mMax_size;
    int          mncrease_size;
};


}
