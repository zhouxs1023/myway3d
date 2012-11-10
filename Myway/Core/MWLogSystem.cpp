#include "MWLogSystem.h"
#include "MWException.h"

using namespace Myway;

IMP_SLN(LogSystem);
LogSystem::LogSystem()
{
    INIT_SLN;
}

LogSystem::~LogSystem()
{
    SHUT_SLN;
    Shutdown();
}

void LogSystem::Init(const TString128 & filename)
{
    if (!mFile.Open(filename.c_str()))
        EXCEPTION("Log Error: " + filename + " can't open.");
}

void LogSystem::Shutdown()
{
    mFile.Close();
}

void LogSystem::Print(const String & infomation)
{
    mFile.Write(infomation.c_str(), infomation.Length());

    mFile.Flush();
}

void LogSystem::PrintFormat(const char * format, ...)
{
    const int TEXT_BUFFER_SIZE = 2048;
    char text[TEXT_BUFFER_SIZE];
    va_list arglist;

    va_start(arglist, format);
    vsprintf_s(text, 2048, format, arglist);
    va_end(arglist);

    mFile.Write(text, sizeof(char) * Strlen(text));

    mFile.Flush();
}