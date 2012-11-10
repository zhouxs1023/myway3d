#pragma once

#include "MWFile.h"
#include "MWSingleton.h"


namespace Myway
{

class MW_ENTRY LogSystem
{
    DECLARE_SINGLETON (LogSystem);

public:
    LogSystem();
    ~LogSystem();

    void Init(const TString128 & filename);
    void Shutdown();

    void Print(const String & infomation);
    void PrintFormat(const char * format, ...);

protected:
    File    mFile;
};

#define MW_ENABLE_LOG_SYSTEM

#ifdef MW_ENABLE_LOG_SYSTEM

    #define LOG_PRINT(s)        Myway::LogSystem::Instance()->Print(s)

    #define LOG_PRINT_FORMAT    Myway::LogSystem::Instance()->PrintFormat

#else 
  
    #define LOG_PRINT(s)
    #define LOG_PRINT_FORMAT

#endif
}