#include <iostream>
#include <windows.h>
#include <io.h>

#include "MWCommand.h"

#pragma warning(push)
#pragma warning(disable : 4996)

using namespace std;

namespace Myway
{

Command cmd;

IMP_SLN(Command);
Command::Command()
{
    INIT_SLN;
}

Command::~Command()
{
    SHUT_SLN;
}

void Command::Init()
{
	AllocConsole();
	//freopen("CONOUT$", "w+t", stdout);
	//freopen("CONIN$", "r+t", stdin);

	HANDLE hStdHandle;
	int fdConsole ;
	FILE *fp;
	AllocConsole();
	hStdHandle =(HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);
	fdConsole = _open_osfhandle((intptr_t)hStdHandle , 0x4000);
	fp = _fdopen( fdConsole , "w" );
	*stdout = *fp;

	::SetConsoleTitle("Myway Command");
}

void Command::Shudown()
{
	//fclose(stdout);
	//fclose(stdin);
	FreeConsole();
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


#pragma warning(pop)

}
