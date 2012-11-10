#pragma once

#include "MWDIInputSystemEntry.h"
#include "MWInputSystem.h"
#include "MWDIMouse.h"
#include "MWDIKeyboard.h"
#include "MWDIJoystick.h"

namespace Myway
{

class DI_ENTRY DIInputSystem : public InputSystem
{
public: 
	DIInputSystem(HINSTANCE hInst, HWND hWnd);
	~DIInputSystem();

	virtual void Update();

protected:
	LPDIRECTINPUT8	m_pDISystem;

	DIKeyboard *	m_pKeyboard;
	DIMouse *		m_pMouse;
	DIJoystick *	m_pJoystick;
};

}
