#include "MGUI_String.h"

#pragma warning(push)
#pragma warning(disable : 4996)

namespace Myway {

	static const int MGUI_StringDefaultSize = 128;

	MGUI_String::MGUI_String(const MGUI_String & _str)
	{
		mSize = _str.mSize;
		mStr = (wchar_t *)pool_alloc(sizeof(wchar_t) * mSize);
		wcscpy(mStr, _str.c_str());
	}

	MGUI_String::MGUI_String(const wchar_t * _str)
	{
		d_assert (_str != NULL);

		mSize = wcslen(_str) + 1;
		if (mSize < MGUI_StringDefaultSize)
			mSize = MGUI_StringDefaultSize;

		mStr = (wchar_t *)pool_alloc(sizeof(wchar_t) * mSize);
		wcscpy(mStr, _str);
	}

	MGUI_String::MGUI_String()
	{
		mSize = MGUI_StringDefaultSize;
		mStr = (wchar_t *)pool_alloc(sizeof(wchar_t) * mSize);
		mStr[0] = 0;
	}

	MGUI_String::~MGUI_String()
	{
		safe_pool_free(mStr);
	}

	bool MGUI_String::operator ==(const MGUI_String & _str) const
	{
		return wcscmp(mStr, _str.c_str()) == 0;
	}

	bool MGUI_String::operator !=(const MGUI_String & _str) const
	{
		return wcscmp(mStr, _str.c_str()) != 0;
	}

	void MGUI_String::operator +=(const MGUI_String & _str)
	{
		int needSize = Length() + _str.Length() + 1;

		if (mSize < needSize)
		{
			wchar_t * newStr = (wchar_t *)pool_alloc(sizeof(wchar_t) * needSize);

			wcscpy(newStr, mStr);
			wcscat(newStr, _str.c_str());

			safe_pool_free (mStr);
			mStr = newStr;
			mSize = needSize;
		}
		else
		{
			wcscat(mStr, _str.c_str());
		}
	}

	void MGUI_String::operator +=(wchar_t _char)
	{
		Insert(Length(), _char);
	}


	MGUI_String & MGUI_String::operator =(const MGUI_String & _str)
	{
		if (mSize < _str.Length() + 1)
		{
			safe_pool_free (mStr);

			mSize = _str.mSize;
			mStr = (wchar_t *)pool_alloc(sizeof(wchar_t) * mSize);
			wcscpy(mStr, _str.c_str());
		}
		else
		{
			wcscpy(mStr, _str.c_str());
		}

		return *this;
	}

	wchar_t & MGUI_String::operator [](int _i)
	{
		return mStr[_i];
	}

	const wchar_t & MGUI_String::operator [](int _i) const
	{
		return mStr[_i];
	}

	const wchar_t * MGUI_String::c_str() const
	{
		return mStr;
	}

	int MGUI_String::Length() const
	{
		return wcslen(mStr);
	}

	void MGUI_String::Remove(int _i, int _count)
	{
		d_assert (_i + _count <= Length());

		if (_count == 0)
			return ;

		const wchar_t * str = mStr + _i;

		for (int i = 0; i < _count; ++i)
		{
			mStr[i + _i] = 0;
			++str;
		}

		wcscat(mStr, str);
	}

	void MGUI_String::Insert(int _i, wchar_t _char)
	{
		d_assert (_i <= Length());

		int needSize = Length() + 1;

		if (mSize < needSize)
		{
			mSize += 64;

			wchar_t * newStr = (wchar_t *)pool_alloc(sizeof(wchar_t) * mSize);

			wcscpy(newStr, mStr);

			safe_pool_free (mStr);
			mStr = newStr;
		}

		for (int i = Length(); i >= _i; --i)
		{
			mStr[i + 1] = mStr[i];
		}

		mStr[_i] = _char;
	}

	void MGUI_String::Insert(int _i, const wchar_t * _str)
	{
		int len = wcslen(_str);

		for (int i = len - 1; i >= 0; --i)
		{
			Insert(_i, _str[i]);
		}
	}

}

#pragma warning(pop)
