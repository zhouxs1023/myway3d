#include "MWDIJoystick.h"

#pragma warning(push)
#pragma warning(disable : 4018)

namespace Myway
{

DIJoystick * gThis = NULL;

bool gJSEnumDeviceCallBack(const DIDEVICEINSTANCE * inst, void * pData)
{
	return gThis->EnumDeviceCallBack(inst, pData);
}

DIJoystick::DIJoystick(LPDIRECTINPUT8 di, HWND hwnd)
{
	m_bActive = false;

	m_device = NULL;
	ZeroMemory(&m_gcState, sizeof(DIJOYSTATE2));
	ZeroMemory(&m_oldState, sizeof(DIJOYSTATE2));
	m_name[0] = '\0';
	m_numButtons = 0;
	m_hWnd = NULL;
	
	//Save copies
	gThis = this;
	m_hWnd = hwnd;
	m_pDISystem = di;

	DIPROPRANGE range;
	DIDEVCAPS caps;

	m_pDISystem->EnumDevices(DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)gJSEnumDeviceCallBack, NULL, 
		DIEDFL_ATTACHEDONLY);

	if (m_device)
	{
		range.diph.dwSize = sizeof(DIPROPRANGE);
		range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		range.diph.dwHow = DIPH_BYOFFSET;
		range.lMin = -1000;
		range.lMax = 1000;
		range.diph.dwObj = DIJOFS_X;
		m_device->SetProperty(DIPROP_RANGE, &range.diph);
		range.diph.dwObj = DIJOFS_Y;
		m_device->SetProperty(DIPROP_RANGE, &range.diph);

		if (SUCCEEDED(m_device->GetCapabilities(&caps)))
			m_numButtons = caps.dwButtons;
		else
			m_numButtons = 4;

		m_bActive = true;
	}
}

DIJoystick::~DIJoystick()
{
	if (m_device)
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = NULL;
	}
}


bool DIJoystick::EnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void *pData)
{
	//set to the first device found
	if (SUCCEEDED(m_pDISystem->CreateDevice(inst->guidInstance, 
		&m_device, NULL)))
	{
		if (SUCCEEDED(m_device->SetDataFormat(&c_dfDIJoystick2)))
		{
			if (SUCCEEDED(m_device->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			{
				if (SUCCEEDED(m_device->Acquire()))
				{
					Strcpy(m_name, sizeof(m_name), (char*)inst->tszInstanceName);
					return DIENUM_STOP;
				}
			}
		}
	}

	//return continue tot try to init other connected devices
	return DIENUM_CONTINUE;
}

bool DIJoystick::KeyUp(JoystickKeyCode key)
{
	if (key < 0 || key >= m_numButtons)
		return false;

	const BYTE * cur = m_gcState.rgbButtons;
	const BYTE * old = m_oldState.rgbButtons;

	return ((cur[key] & 0x80) != 0x80) && (cur[key] != old[key]);
}

bool DIJoystick::KeyDown(JoystickKeyCode key)
{
	if (key < 0 || key >= m_numButtons)
		return false;

	const BYTE * cur = m_gcState.rgbButtons;
	const BYTE * old = m_oldState.rgbButtons;

	return ((cur[key] & 0x80) == 0x80) && (cur[key] != old[key]);
}

bool DIJoystick::KeyPressed(JoystickKeyCode key)
{
	if (key < 0 || key >= m_numButtons)
		return false;

	const BYTE * cur = m_gcState.rgbButtons;
	const BYTE * old = m_oldState.rgbButtons;

	return ((cur[key] & 0x80) == 0x80) && (cur[key] == old[key]);
}

bool DIJoystick::IsActive()
{
	return m_bActive;
}

void DIJoystick::Update()
{
	if (m_device)
	{
		m_device->Poll();

		//save old state for input comparing
		Memcpy(&m_oldState, &m_gcState, sizeof(m_gcState));

		HRESULT hr = S_OK;

		hr = m_device->GetDeviceState(sizeof(DIJOYSTATE2), &m_gcState);

		if (FAILED(hr))
		{
			hr = m_device->Acquire();

			if (SUCCEEDED(hr))
			{
				hr = m_device->GetDeviceState(sizeof(DIJOYSTATE2), &m_gcState);
			}
		}

		m_bActive = SUCCEEDED(hr);
	}
}

}

#pragma warning(pop)
