#include <iostream>
#include <windows.h>

#include "MWCommand.h"

#pragma warning(push)
#pragma warning(disable : 4996)

namespace Myway
{

using namespace std;

Command cmd;

IMP_SLN(Command);
Command::Command()
{
    INIT_SLN;
    ::SetConsoleTitle("Myway Command");
}

Command::~Command()
{
    SHUT_SLN;
}

void Command::Print(const char * fmt, ...)
{
    const int TEXT_BUFFER_SIZE = 2048;
    char text[TEXT_BUFFER_SIZE];
    va_list arglist;

    va_start(arglist, fmt);
    vsprintf(text, fmt, arglist);
    va_end(arglist);

    cout << text;
}

void Command::SetOuputTextAttribute(int flag)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flag);
}

void Command::SetInputTextAttribute(int flag)
{
    SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), flag);
}

void Command::Wait()
{
    cin.get();
}

Command & Command::operator << (const char * str)
{
    cout << str;
    return *this;
}

Command & Command::operator << (char c)
{
    cout << c;
    return *this;
}

Command & Command::operator << (int i)
{
    cout << i;
    return *this;
}

Command & Command::operator << (float f)
{
    cout << f;
    return *this;
}

Command & Command::operator << (double d)
{
    cout << d;
    return *this;
}

Command & Command::operator >> (char * str)
{
    cin >> str;
    return *this;
}

Command & Command::operator >> (char & c)
{
    cin >> c;
    return *this;
}


Command & Command::operator >> (int & i)
{
    cin >> i;
    return *this;
}

Command & Command::operator >> (float & f)
{
    cin >> f;
    return *this;
}

Command & Command::operator >> (double & d)
{
    cin >> d;
    return *this;
}


#pragma warning(pop)

}
