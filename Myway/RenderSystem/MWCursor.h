#pragma once

#include "MWCore.h"

namespace Myway {

	class MW_ENTRY Cursor
	{
		DECLARE_SINGLETON(Cursor);
		DECLARE_ALLOC();

	public:
		enum eSystemCursor {
			kArrow  = 32512,		// 箭头
			kIbeam  = 32513,		// 工字型 edit
			kWait = 32514,			// 等待沙漏
			kCross = 32515,			// 十字   ╋
			kUpArrow = 32516,		// 向上箭头 ↑
			kSize =  32646,			// 十字四向箭头
			kSizeNWSE = 32642,		// 北西、南东双向箭头
			kSizeNESW = 32643,		// 北东、南西双向箭头
			kSizeWE = 32644,		// 东西双向箭头
			kSizeNS = 32645,		// 南北双向箭头
			kNo = 32648,			// 圆中斜杠，no符号
			kAppStarting = 32650,	// ARROW+WAIT
			kHelp = 32651,			// ARROW+问号
			kHand = 32649,			// 手型
		};

	public:
		Cursor();
		~Cursor();

		void Load(int _dwResId);
		void Bind(int _dwResId);
		void Unbind();

		void Draw();

	protected:
		Map<int, HCURSOR> mCursorMap;
		HCURSOR mCursor;
		HCURSOR mLastCursor;
	};

}