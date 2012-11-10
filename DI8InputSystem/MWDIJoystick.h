#pragma once

#include "MWDIInputSystemEntry.h"
#include "MWJoystick.h"

namespace Myway
{

class DI_ENTRY DIJoystick : public IJoystick
{
public:
	DIJoystick(LPDIRECTINPUT8 di, HWND hwnd);
	virtual ~DIJoystick();

	virtual bool KeyUp(JoystickKeyCode key);
	virtual bool KeyDown(JoystickKeyCode key);
	virtual bool KeyPressed(JoystickKeyCode key);

	virtual bool IsActive();

	//used to create the game controllers
	bool EnumDeviceCallBack(const DIDEVICEINSTANCE * inst, void * pData);

	void Update();


protected:
	LPDIRECTINPUTDEVICE8	m_device;
	DIJOYSTATE2				m_gcState;
	DIJOYSTATE2             m_oldState;
	char					m_name[256];
	unsigned long           m_numButtons;

	//left and right stick x and y positions
	/*long m_xGCPos;
	long m_yGCPos;
	long m_xGCPos2;
	long m_yGCPos2;*/

	HWND					m_hWnd;

	LPDIRECTINPUT8			m_pDISystem;

	bool					m_bActive;
};

}