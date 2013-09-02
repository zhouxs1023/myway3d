#pragma once

#include "MWSingleton.h"

namespace Myway
{

#define CMD_FOREGROUND_BLUE      0x0001 // text color contains blue.
#define CMD_FOREGROUND_GREEN     0x0002 // text color contains green.
#define CMD_FOREGROUND_RED       0x0004 // text color contains red.
#define CMD_FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define CMD_BACKGROUND_BLUE      0x0010 // background color contains blue.
#define CMD_BACKGROUND_GREEN     0x0020 // background color contains green.
#define CMD_BACKGROUND_RED       0x0040 // background color contains red.
#define CMD_BACKGROUND_INTENSITY 0x0080 // background color is intensified.

#define mcmd (*Command::Instance())
#define mendl '\n'

class MW_ENTRY Command : public AllocObj
{
    DECLARE_SINGLETON(Command);

public:
    Command();
    ~Command();

	void Init();
	void Shudown();

    void SetOuputTextAttribute(int flag);
    void SetInputTextAttribute(int flag);

    void Print(const char * fmt, ...);

    Command & operator << (const char * str);
    Command & operator << (char c);
    Command & operator << (int i);
    Command & operator << (float f);
    Command & operator << (double d);
};

}