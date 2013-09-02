#include "MWCursor.h"

namespace Myway {

	Cursor * Cursor::msInstance = NULL;


	Cursor::Cursor()
	{
		msInstance = this;

		mCursor = NULL;
		mLastCursor = NULL;

		Load(kArrow);
		Load(kIbeam);
		Load(kWait);
		Load(kCross);
		Load(kUpArrow);
		Load(kSize);
		Load(kSizeNWSE);
		Load(kSizeNESW);
		Load(kSizeWE);
		Load(kSizeNS);
		Load(kNo);
		Load(kAppStarting);
		Load(kHelp);
		Load(kHand);

		Bind(kArrow);
	}

	Cursor::~Cursor()
	{
		Map<int, HCURSOR>::Iterator whr = mCursorMap.Begin();
		Map<int, HCURSOR>::Iterator end = mCursorMap.End();

		while (whr != end)
		{
			::DestroyCursor(whr->second);
			++whr;
		}

		mCursorMap.Clear();

		msInstance = NULL;
	}

	void Cursor::Load(int _dwResId)
	{
		HCURSOR hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(_dwResId));

		mCursorMap.Insert(_dwResId, hCursor);
	}

	void Cursor::Bind(int _dwResId)
	{
		mLastCursor = mCursor;
		mCursor = mCursorMap[_dwResId];
		mCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(_dwResId));
	}

	void Cursor::Unbind()
	{
		mCursor = mLastCursor;
		mLastCursor = NULL;
	}

	void Cursor::Draw()
	{
		::SetCursor(mCursor);
	}

}