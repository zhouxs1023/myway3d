#include "MWDebug.h"
#include "MWMemory.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error "not support other platform"
#endif

#pragma warning(disable : 4996)

using namespace Myway;

int Myway::DisplayError(const char * title, 
						const char * text, 
						const char * description, 
						const char * file, 
						int line)
{
#ifdef MW_PLATFORM_WIN32

	HWND hwnd = GetActiveWindow();
	if (!hwnd)
		hwnd = GetLastActivePopup(hwnd);

	const int BUFFER_SIZE = 1024;
	const int MODULE_NAME_SIZE = 255;
	char buffer[BUFFER_SIZE];
	char program[MODULE_NAME_SIZE];

	if (!GetModuleFileName(NULL, program, MODULE_NAME_SIZE))
		Strcpy(program, MODULE_NAME_SIZE, "<unknow application>");

	_snprintf(buffer, BUFFER_SIZE, 
		      "%s\n\n"					\
              "Program:      %s\n"		\
              "File:         %s\n"		\
              "Line:         %d\n\n"	\
              "Error:        %s\n"		\
              "Description:  %s\n",
              title, program, file, line, text, description);

	int iRet = MessageBox(hwnd, buffer, "Myway Engine Run Time", 
		                  MB_TASKMODAL | MB_SETFOREGROUND | MB_ABORTRETRYIGNORE | MB_ICONERROR);
	switch (iRet)
	{
	case IDIGNORE:
		return DEBUG_IGNORE;

	case IDCONTINUE:
		return DEBUG_CONTINUE;

	case IDRETRY:
		return DEBUG_CONTINUE;

	case IDABORT:
		return DEBUG_BACKPOINT;

	default:
		ExitProcess(-1);
		return 0;
	}

#else

    #error "not support other platform"

#endif
}

void Myway::Output(const char * fmt, ...)
{
	const int TEXT_BUFFER_SIZE = 10240;
	char text[TEXT_BUFFER_SIZE];
	va_list arglist;

	va_start(arglist, fmt);
	vsprintf(text, fmt, arglist);
	va_end(arglist);

#ifdef MW_PLATFORM_WIN32

	OutputDebugString(text);

#else

    #error "not support other platform"

#endif
}

void Myway::EnableMemoryLeakCheck()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void Myway::BreakAllock(int time)
{
    _CrtSetBreakAlloc(time);
}