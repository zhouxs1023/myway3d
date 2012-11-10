#pragma once

#include "MWDIInputSystemEntry.h"
#include "MWKeyboard.h"

#define KEYS_SIZE			256

namespace Myway
{

class DI_ENTRY DIKeyboard : public IKeyboard
{
public:
	DIKeyboard(LPDIRECTINPUT8 di, HWND hWnd);
	virtual ~DIKeyboard();

	virtual bool KeyUp(KeyCode key);
	virtual bool KeyDown(KeyCode key);
	virtual bool KeyPressed(KeyCode key);

	virtual bool IsActive();

	void Update();

protected:
	LPDIRECTINPUTDEVICE8 m_device;
	char m_keys[KEYS_SIZE];
	char m_oldKeys[KEYS_SIZE];
	bool m_bActive;
};

}
