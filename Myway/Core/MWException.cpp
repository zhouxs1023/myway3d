#include "MWException.h"

using namespace Myway;

Exception::Exception(int number,
                     const TString128 & desc,
                     const char * func,
                     const char * file,
                     int line)
: mNumber(number),
  mDescription(desc),
  mFunction(func),
  mType("GenericException"),
  mFile(file),
  mLine(line)
{
}

Exception::~Exception()
{
}

int Exception::GetNumber() const
{ 
    return mNumber;
}

const TString128 & Exception::GetDescricption() const
{ 
    return mDescription;
}

const TString128 & Exception::GetFunction() const
{ 
    return mFunction;
}

const TString128 & Exception::GetType() const
{ 
    return mType; 
}

const TString128 & Exception::GetFile() const
{
    return mFile;
}

const char * Exception::What() const
{
    static char message[2048];

    sprintf_s(message, 2048, "  File:   %s\n"
                             "  Func:   %s\n"
                             "  Desc:   %s", 
                             mFile.c_str(),
                             mFunction.c_str(),
                             mDescription.c_str());

    return message;
}