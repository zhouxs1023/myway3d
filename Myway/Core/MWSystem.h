#pragma once

#include "MWDefine.h"
#include "MWTimer.h"

namespace Myway
{

#define	SUPPORT_MMX				1
#define	SUPPORT_SSE				2
#define	SUPPORT_SSE2			4

enum OS_TYPE
{
	OS_UNKNOWN = 0,
	OS_WIN_95,
	OS_WIN_95_SR2,
	OS_WIN_98,
	OS_WIN_98_SR2,
	OS_WIN_ME,
	OS_WIN_NT,
	OS_WIN_2K,
	OS_WIN_XP
};


class MW_ENTRY System
{
public:
	/*struct OS_VERSION
	{
		int MajorVersion;
		int MinorVersion;
		int Build;
	};*/

    static void     Init();
    static void     DeInit();

	static int      CpuFlags();
	static int      Platform();
	static uint64   TotalMemory();
	static uint64   PhysicalMemory();
	static int      ProcessorSpeed();
	static uint64   CpuCount();
	static uint64   AvailableMemory();
    static void     Sleep(int milliseconds);

public:
    static Timer    InternalTime;
};

}