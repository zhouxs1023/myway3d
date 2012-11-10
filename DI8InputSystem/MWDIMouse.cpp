#include "MWDIMouse.h"

namespace Myway
{

DIMouse::DIMouse(LPDIRECTINPUT8 input, HWND hWnd)
: m_positionLocal(0, 0),
  m_positionGlobal(0, 0),
  m_positionUnit(0, 0),
  m_oldPositionLocal(0, 0),
  m_oldPositionGlobal(0, 0),
  m_oldPositionUnit(0, 0)
{
	m_bActive = false;
	m_device = NULL;
	m_hWnd = NULL;
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_oldMouseState, sizeof(DIMOUSESTATE));

	DWORD flags;

	if (input->CreateDevice(GUID_SysMouse, & m_device, NULL) == DI_OK)
	{
		if (m_device->SetDataFormat(&c_dfDIMouse) == DI_OK)
		{
			/*
			if (exclusive)
				flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
			else
				flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
			*/

			flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

			if (FAILED(m_device->SetCooperativeLevel(hWnd, flags)))
				m_device->Acquire();

			m_bActive = true;
		}
	}

	m_hWnd = hWnd;
}

DIMouse::~DIMouse()
{
	if (m_device)
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = NULL;
	}
}

bool DIMouse::KeyUp(MouseKeyCode key)
{
	const BYTE * cur = m_mouseState.rgbButtons;
	const BYTE * old = m_oldMouseState.rgbButtons;

	return ((cur[key] & 0x80) != 0x80) && (cur[key] != old[key]);
}

bool DIMouse::KeyDown(MouseKeyCode key)
{
	const BYTE * cur = m_mouseState.rgbButtons;
	const BYTE * old = m_oldMouseState.rgbButtons;

	return ((cur[key] & 0x80) == 0x80) && (cur[key] != old[key]);
}

bool DIMouse::KeyPressed(MouseKeyCode key)
{
	const BYTE * cur = m_mouseState.rgbButtons;
	const BYTE * old = m_oldMouseState.rgbButtons;

	return ((cur[key] & 0x80) == 0x80) && (cur[key] == old[key]);
}

bool DIMouse::MouseMoved()
{
    RECT rt;
    ::GetWindowRect(m_hWnd, &rt);

    if (m_positionGlobal.x >= rt.left && m_positionGlobal.x <= rt.right &&
        m_positionGlobal.y >= rt.top && m_positionGlobal.y <= rt.bottom)
    {
        return m_positionGlobal.x != m_oldPositionGlobal.x ||
               m_positionGlobal.y != m_oldPositionGlobal.y;
    }

    return false;
}


Point2i DIMouse::GetPosition()
{
	return m_positionLocal;
}

Point2i DIMouse::GetPositionGlobal()
{
	return m_positionGlobal;
}

Point2f DIMouse::GetPositionUnit()
{
	return m_positionUnit;
}

Point2i DIMouse::GetOldPosition()
{
    return m_oldPositionLocal;
}

Point2i DIMouse::GetOldPositionGlobal()
{
    return m_oldPositionGlobal;
}

Point2f DIMouse::GetOldPositionUnit()
{
    return m_oldPositionUnit;
}

Point2i DIMouse::GetPositionDiff()
{
    Point2i diff;

    diff.x = m_positionLocal.x - m_oldPositionLocal.x;
    diff.y = m_positionLocal.y - m_oldPositionLocal.y;

    return diff;
}

bool DIMouse::IsActive()
{
	return m_bActive;
}

void DIMouse::Update()
{
	if (m_device)
	{
		//save old state for input comparing
		Memcpy(&m_oldMouseState, &m_mouseState, sizeof(m_mouseState));

		HRESULT hr = S_OK;

		//if error getting device state, re-aquire
		hr = m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);

		if (FAILED(hr))
		{
			hr = m_device->Acquire();

			if (SUCCEEDED(hr))
				hr = m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
		}

		m_bActive = SUCCEEDED(hr);

		POINT pt;
		RECT rt;
		::GetCursorPos(&pt);
		::GetWindowRect(m_hWnd, &rt);

        m_oldPositionLocal = m_positionLocal;
        m_oldPositionGlobal = m_positionGlobal;
        m_oldPositionUnit = m_positionUnit;

		m_positionGlobal.x = pt.x;
		m_positionGlobal.y = pt.y;
		m_positionLocal.x = pt.x - rt.left;
		m_positionLocal.y = pt.y - rt.top;
		m_positionUnit.x = (float)(pt.x - rt.left) / (rt.right - rt.left);
		m_positionUnit.y = (float)(pt.y - rt.top) / (rt.bottom - rt.top);
	}
}

}