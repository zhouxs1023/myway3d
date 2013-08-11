#pragma once

#include "MGUI_Types.h"

namespace Myway {

	class MGUI_ENTRY MGUI_String
	{
		DECLARE_ALLOC();

	public:
		MGUI_String(const MGUI_String & _str);
		MGUI_String(const wchar_t * _str);
		MGUI_String();
		~MGUI_String();

		bool operator ==(const MGUI_String & _str) const;
		bool operator !=(const MGUI_String & _str) const;
		void operator +=(const MGUI_String & _str);
		void operator +=(wchar_t _char);
		MGUI_String & operator =(const MGUI_String & _str);

		wchar_t & operator [](int _i);
		const wchar_t & operator [](int _i) const;

		const wchar_t * c_str() const;
		int Length() const;

		void Insert(int _i, wchar_t _char);
		void Insert(int _i, const wchar_t * _str);
		void Remove(int _i, int _count);

	protected:
		wchar_t * mStr;
		int mSize;
	};
}