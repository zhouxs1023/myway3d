#pragma once

#include "MWString.h"
#include "MWTString.h"

namespace Myway
{

class MW_ENTRY Exception : public AllocObj
{
public:
    Exception(int number, const TString128 & desc, const char * func, const char * file, int line);
    virtual ~Exception();

    virtual int GetNumber() const;
    virtual const TString128 & GetDescricption() const;
    virtual const TString128 & GetFunction() const;
    virtual const TString128 & GetType() const;
    virtual const TString128 & GetFile() const;
    virtual const char * What() const;

protected:
    int         mNumber;
    int         mLine;

    TString128  mType;
    TString128  mDescription;
    TString128  mFunction;
    TString128  mFile;
};


#define __EXCEPTION(number, desc)  throw Exception(number, desc, __FUNCTION__, __FILE__, __LINE__)

#ifdef _DEBUG

#define __EXCEPTION_DEBUG(x, number, desc) if (!(x)) { __EXCEPTION(number, desc); } 

#else

#define __EXCEPTION_DEBUG(x, number, desc)

#endif

#define EXCEPTION(desc)                     __EXCEPTION(0, desc)
#define EXCEPTION_COMMENT(x, desc)          if (!(x)) { __EXCEPTION(0, desc); }
#define EXCEPTION_DEBUG(x, desc)            __EXCEPTION_DEBUG(x, 0, desc)

}