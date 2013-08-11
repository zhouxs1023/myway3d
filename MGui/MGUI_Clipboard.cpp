#include "MGUI_Clipboard.h"

namespace Myway {

	MGUI_Clipboard * MGUI_Clipboard::msInstance = NULL;

	MGUI_Clipboard::MGUI_Clipboard()
	{
		msInstance = this;
	}

	MGUI_Clipboard::~MGUI_Clipboard()
	{
		msInstance = NULL;
	}

	void MGUI_Clipboard::SetText(const MGUI_String & _text)
	{
		HWND hWnd = Engine::Instance()->GetDeviceProperty()->hWnd;

		if (OpenClipboard(hWnd))
		{
			int size = (_text.Length() + 1) * sizeof(wchar_t);

			EmptyClipboard();
			HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, size);
			wchar_t* chBuffer = hgBuffer ? (wchar_t*)GlobalLock(hgBuffer) : NULL;

			if (chBuffer)
			{
				memcpy(chBuffer, _text.c_str(), size);
				GlobalUnlock(hgBuffer);
				SetClipboardData(CF_UNICODETEXT, hgBuffer);
			}

			CloseClipboard();
		}
	}

	MGUI_String MGUI_Clipboard::GetText()
	{
		HWND hWnd = Engine::Instance()->GetDeviceProperty()->hWnd;

		MGUI_String text;

		if (OpenClipboard(hWnd))
		{
			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			wchar_t* chBuffer = hData ? (wchar_t*)GlobalLock(hData) : NULL;

			if (chBuffer)
			{
				text = chBuffer;
				GlobalUnlock(hData);
			}

			CloseClipboard();
		}

		return text;
	}
}