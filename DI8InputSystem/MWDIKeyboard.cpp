#include "MWDIKeyboard.h"

namespace Myway
{

//keyboard
DIKeyboard::DIKeyboard(LPDIRECTINPUT8 input, HWND hwnd)
{
	m_device = NULL;
	ZeroMemory(m_keys, sizeof(char) * KEYS_SIZE);
	ZeroMemory(m_oldKeys, sizeof(char) * KEYS_SIZE);
	m_bActive = false;


	//init the keyboard
	if (input->CreateDevice(GUID_SysKeyboard, &m_device, NULL) == DI_OK)
	{
		if (SUCCEEDED(m_device->SetDataFormat(&c_dfDIKeyboard)))
		{
			//set cooperative level
			if (m_device->SetCooperativeLevel(hwnd, 
					DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) == DI_OK)
			{
				m_device->Acquire();
				m_bActive = true;
			}
		}
	}

	Memset8(m_keys, 0, sizeof(m_keys));
}

DIKeyboard::~DIKeyboard()
{
	if (m_device)
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = NULL;
	}
}

bool DIKeyboard::KeyUp(KeyCode key)
{
	return ((m_keys[key] & 0x80) != 0x80) && (m_keys[key] != m_oldKeys[key]);
}

bool DIKeyboard::KeyDown(KeyCode key)
{
	return ((m_keys[key] & 0x80) == 0x80) && (m_keys[key] != m_oldKeys[key]);
}

bool DIKeyboard::KeyPressed(KeyCode key)
{
	return ((m_keys[key] & 0x80) == 0x80) && (m_keys[key] == m_oldKeys[key]);
}

bool DIKeyboard::IsActive()
{
	return m_device != NULL;
}

void DIKeyboard::Update()
{
	if (m_device)
	{
		//save old state for input comparing
		Memcpy(m_oldKeys, m_keys, sizeof(m_keys));

		// if error getting device state, re-aquire
		HRESULT hr = S_OK;
		hr = m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys);
		if (FAILED(hr))
		{
			hr = m_device->Acquire();
			if (SUCCEEDED(hr))
			{
				hr = m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys);
			}
		}

		m_bActive = SUCCEEDED(hr);
	}

}

}