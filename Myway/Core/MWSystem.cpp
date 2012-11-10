#include "MWSystem.h"
#include "MWTimer.h"
#include "MWException.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error not suppored.
#endif

using namespace Myway;

#define       CPUID_STD_FPU                      0x00000001       
#define       CPUID_STD_VME                      0x00000002       
#define       CPUID_STD_DEBUGEXT                 0x00000004       
#define       CPUID_STD_4MPAGE					 0x00000008       
#define       CPUID_STD_TSC                      0x00000010       
#define       CPUID_STD_MSR                      0x00000020       
#define       CPUID_STD_PAE                      0x00000040       
#define       CPUID_STD_MCHKXCP                  0x00000080       
#define       CPUID_STD_CMPXCHG8B				 0x00000100       
#define       CPUID_STD_APIC                     0x00000200       
#define       CPUID_STD_SYSENTER                 0x00000800       
#define       CPUID_STD_MTRR                     0x00001000       
#define       CPUID_STD_GPE                      0x00002000       
#define       CPUID_STD_MCHKARCH                 0x00004000       
#define       CPUID_STD_CMOV                     0x00008000       
#define       CPUID_STD_PAT                      0x00010000       
#define       CPUID_STD_PSE36                    0x00020000       
#define       CPUID_STD_MMX                      1 << 23       
#define       CPUID_STD_FXSAVE                   0x01000000       
#define       CPUID_STD_SSE                      1 << 25     
#define       CPUID_STD_SSE2                     1 << 26

static int uCpuFlags;
static int uPlatform;
static uint64 uTotalMemory;
static uint64 uPhysicalMemory;
static int uProcessorSpeed;

Timer System::InternalTime;

void System::Init()
{
	int result = 0;
	uCpuFlags			= 0;
	uPlatform			= 0;
	uTotalMemory		= 0;
	uPhysicalMemory		= 0;
	uProcessorSpeed		= 0;

#ifdef MW_PLATFORM_WIN32
   
	__asm
	{
		// verify cpuid
		pushfd;
		pop			eax;
		mov			ecx,		eax;
		xor			eax,		200000h;
		push		eax;
		popfd;
		pushfd;
		pop			eax;
		xor			eax,		ecx;
		jz			__end;				       //no cpuid
		mov			result,		1;

		// verify mmx 
		mov			eax,		1;
		cpuid;
		test		edx,		CPUID_STD_MMX;
		jz			__end;
		or			uCpuFlags,	SUPPORT_MMX;

		// verify sse
		test        edx,		CPUID_STD_SSE;
		jz			__end;
		or          uCpuFlags,	SUPPORT_SSE;
		
		// verify sse2
		test        edx,		CPUID_STD_SSE2;
		jz			__end;
		or			uCpuFlags,  SUPPORT_SSE2;

__end:
	}

	SYSTEM_INFO		SysInfo;
	MEMORYSTATUS	MemStatus;
	OSVERSIONINFO	OSVersion;

	MemStatus.dwLength = sizeof(MEMORYSTATUS);
	OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetSystemInfo(&SysInfo);
	GlobalMemoryStatus(&MemStatus);
	GetVersionEx(&OSVersion);

	uTotalMemory = uint64(MemStatus.dwAvailPhys) + uint64(MemStatus.dwAvailPageFile);
	uPhysicalMemory = uint64(MemStatus.dwTotalPhys);
	
	switch (OSVersion.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:
		{
			uPlatform = OS_WIN_95;
			if (OSVersion.dwMinorVersion == 0 && (LOWORD(OSVersion.dwBuildNumber)) > 950)
				uPlatform = OS_WIN_95_SR2;
			else if (OSVersion.dwMinorVersion == 10)
				uPlatform = OS_WIN_98;
			else if (OSVersion.dwMinorVersion > 10)
				uPlatform = OS_WIN_ME;
		}
		break;

	case VER_PLATFORM_WIN32_NT:
		{
			if (OSVersion.dwMajorVersion < 4)
				uPlatform = OS_WIN_NT;
			else if (OSVersion.dwMajorVersion == 4)
				uPlatform = OS_WIN_2K;
			else if (OSVersion.dwMajorVersion == 5)
				uPlatform = OS_WIN_XP;
			else
				uPlatform = OS_UNKNOWN;
		}
		break;

	default:
		uPlatform = OS_UNKNOWN;
	}


	Timer time;
	uint64 start, end;
	time.Start();
	start = CpuCount();
	Sleep(100);
	end = CpuCount();
	time.Stop();
	uProcessorSpeed = int(end - start) / (int)time.ElapsedMilliseconds();

    srand(timeGetTime());

    InternalTime.Start();

    EXCEPTION_COMMENT(result, "computer not support cpuid.");

#else

#error not suppored.

#endif
}

void System::DeInit()
{
    InternalTime.Stop();
}

int System::CpuFlags()
{
	return uCpuFlags;
}

int System::Platform()
{
	return uPlatform;
}

uint64 System::TotalMemory()
{
	return uTotalMemory;
}

uint64 System::PhysicalMemory()
{
	return uPhysicalMemory;
}

int System::ProcessorSpeed()
{
	return uProcessorSpeed;
}

uint64 System::CpuCount()
{
	__asm rdtsc;
}

uint64 System::AvailableMemory()
{
#ifdef MW_PLATFORM_WIN32

	MEMORYSTATUS MemStatus;
	MemStatus.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStatus);
	return uint64(MemStatus.dwAvailPhys);

#else

#error not suppored.

#endif
}

void System::Sleep(int milliseconds)
{
    ::Sleep(milliseconds);
}